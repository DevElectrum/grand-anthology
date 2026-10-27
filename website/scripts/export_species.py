#!/usr/bin/env python3
"""Generate website-only Pokémon reference data from the configured C game source.

gcc performs the real C preprocessor's conditional and macro resolution. This
extractor parses balanced designated initializers rather than regex-matching
entire structs. It never edits the authoritative game sources or catalog.
"""
import ast
import json
import operator
from pathlib import Path
import re
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / "website/src/generated/pokemon-details.json"
CATALOG = ROOT / "data/catalog/world_acquisition.jsonl"
CONFIG = """
#define TRUE 1
#define FALSE 0
#include "config/general.h"
#include "config/overworld.h"
#include "config/battle.h"
#include "config/pokemon.h"
"""
STATS = ("baseHP", "baseAttack", "baseDefense", "baseSpAttack", "baseSpDefense", "baseSpeed")
OPERATORS = {
    ast.Add: operator.add, ast.Sub: operator.sub, ast.Mult: operator.mul,
    ast.FloorDiv: operator.floordiv, ast.Div: operator.truediv,
    ast.Mod: operator.mod, ast.BitOr: operator.or_, ast.BitAnd: operator.and_,
    ast.LShift: operator.lshift, ast.RShift: operator.rshift,
    ast.USub: operator.neg, ast.UAdd: operator.pos,
    ast.Gt: operator.gt, ast.GtE: operator.ge, ast.Lt: operator.lt,
    ast.LtE: operator.le, ast.Eq: operator.eq, ast.NotEq: operator.ne,
}


def preprocess(path):
    src = CONFIG + '\n#include "' + path + '"\n'
    command = ["gcc", "-E", "-P", "-undef", "-x", "c", "-",
               "-I", str(ROOT / "include"), "-I", str(ROOT / "src/data/pokemon"),
               "-I", str(ROOT / "src")]
    result = subprocess.run(command, input=src, text=True, capture_output=True, cwd=ROOT)
    if result.returncode:
        raise RuntimeError("C preprocessing failed for " + path + ":\n" + result.stderr[-5000:])
    return result.stdout


def closing(text, start, opener="{", closer="}"):
    depth = 0
    quoted = False
    escape = False
    for i in range(start, len(text)):
        c = text[i]
        if quoted:
            if escape:
                escape = False
            elif c == "\\":
                escape = True
            elif c == '"':
                quoted = False
            continue
        if c == '"':
            quoted = True
        elif c == opener:
            depth += 1
        elif c == closer:
            depth -= 1
            if depth == 0:
                return i
    raise ValueError("Unbalanced source initializer at " + str(start))


def split_fields(body):
    chunks = []
    start = 0
    square = curly = round_depth = 0
    quoted = escape = False
    for i, c in enumerate(body):
        if quoted:
            if escape:
                escape = False
            elif c == "\\":
                escape = True
            elif c == '"':
                quoted = False
            continue
        if c == '"':
            quoted = True
        elif c == "[":
            square += 1
        elif c == "]":
            square -= 1
        elif c == "{":
            curly += 1
        elif c == "}":
            curly -= 1
        elif c == "(":
            round_depth += 1
        elif c == ")":
            round_depth -= 1
        elif c == "," and square == curly == round_depth == 0:
            chunks.append(body[start:i].strip())
            start = i + 1
    chunks.append(body[start:].strip())
    return [x for x in chunks if x]


def structures(text, prefix):
    pattern = re.compile(r"\[(" + prefix + r"[A-Z0-9_]+)\]\s*=\s*\{")
    for match in pattern.finditer(text):
        begin = match.end() - 1
        end = closing(text, begin)
        yield match.group(1), text[begin + 1:end]


def fields(body):
    result = {}
    for item in split_fields(body):
        match = re.match(r"\.(\w+)\s*=\s*(.*)", item, re.S)
        if match:
            result[match.group(1)] = match.group(2).strip()
    return result


def literal_strings(expr):
    parts = re.findall(r'"(?:\\.|[^"\\])*"', expr, re.S)
    result = []
    for s in parts:
        try:
            result.append(ast.literal_eval(s))
        except (SyntaxError, ValueError):
            return None
    if not parts:
        return None
    return re.sub(r"\s+", " ", "".join(result)).strip()


def number_expr(expr):
    """Evaluate only a small, explicitly allowed arithmetic/ternary subset."""
    expr = expr.strip()
    while expr.startswith("("):
        try:
            if closing(expr, 0, "(", ")") == len(expr) - 1:
                expr = expr[1:-1].strip()
            else:
                break
        except ValueError:
            break
    if re.fullmatch(r"[-+]?\d+[uUlL]*", expr):
        return int(re.sub(r"[uUlL]+$", "", expr))
    if re.search(r"[A-Za-z_]", re.sub(r"\d+[uUlL]+", "0", expr)):
        return None
    expr = re.sub(r"(\d+)[uUlL]+\b", r"\1", expr)
    question = expr.find("?")
    if question != -1:
        depth = 0
        colon = -1
        for i in range(question + 1, len(expr)):
            if expr[i] == "?":
                depth += 1
            elif expr[i] == ":":
                if depth == 0:
                    colon = i
                    break
                depth -= 1
        if colon == -1:
            return None
        predicate = number_expr(expr[:question])
        return number_expr(expr[question+1:colon] if predicate else expr[colon+1:]) if predicate is not None else None
    try:
        def walk(node):
            if isinstance(node, ast.Constant) and type(node.value) in (int, float):
                return node.value
            if isinstance(node, ast.UnaryOp) and type(node.op) in OPERATORS:
                return OPERATORS[type(node.op)](walk(node.operand))
            if isinstance(node, ast.BinOp) and type(node.op) in OPERATORS:
                return OPERATORS[type(node.op)](walk(node.left), walk(node.right))
            if isinstance(node, ast.Compare) and len(node.ops) == 1 and type(node.ops[0]) in OPERATORS:
                return OPERATORS[type(node.ops[0])](walk(node.left), walk(node.comparators[0]))
            raise ValueError("Unsupported numeric expression")
        value = walk(ast.parse(expr, mode="eval").body)
        return int(value) if int(value) == value else None
    except (SyntaxError, ValueError, TypeError, ZeroDivisionError, OverflowError):
        return None


def symbols(expr, prefix):
    return re.findall(r"\b" + prefix + r"[A-Z0-9_]+\b", expr or "")


def readable(identifier):
    return identifier.replace("_", " ").title()


def evolution_list(value):
    if not value:
        return []
    output = []
    for part in re.findall(r"\{\s*(EVO_[A-Z0-9_]+)\s*,([^{}]*)\}", value):
        method, args = part
        bits = split_fields(args)
        if len(bits) < 2:
            continue
        target_symbols = symbols(bits[1], "SPECIES_")
        if not target_symbols:
            continue
        param = bits[0].strip()
        if method in ("EVO_LEVEL", "EVO_LEVEL_NINJASK", "EVO_LEVEL_SHEDINJA"):
            label = "Level " + str(number_expr(param) or param)
        elif method == "EVO_ITEM":
            label = "Use " + readable(param.replace("ITEM_", ""))
        elif method == "EVO_TRADE":
            label = "Trade"
        else:
            label = readable(method.replace("EVO_", ""))
            if param not in ("0", "FALSE", "TRUE"):
                label += " (" + readable(param.replace("ITEM_", "").replace("MOVE_", "")) + ")"
        output.append({"target": target_symbols[0], "method": method, "requirement": label})
    return output


def find_form_members():
    text = preprocess("form_species_tables.h")
    groups = {}
    for match in re.finditer(r"(s[A-Za-z0-9_]+FormSpeciesIdTable)\s*\[\s*\]\s*=\s*\{", text):
        start = match.end() - 1
        members = symbols(text[start+1:closing(text, start)], "SPECIES_")
        if members:
            for member in members:
                groups.setdefault(member, set()).update(members)
    return groups


def main():
    entries = [json.loads(line) for line in CATALOG.read_text().splitlines() if line]
    catalog = [item for item in entries if item.get("kind") == "pokemon"]
    dex_by_symbol = {item["symbol"]: item["dex"] for item in catalog}
    dex_by_national = {"NATIONAL_DEX_" + item["symbol"][8:]: item["dex"] for item in catalog}
    shared = preprocess("species_info/shared_dex_text.h")
    shared_descriptions = {}
    for match in re.finditer(r"\b(g[A-Za-z0-9_]+)\[\]\s*=\s*_\s*\(", shared):
        start = match.end() - 1
        shared_descriptions[match.group(1)] = literal_strings(shared[start+1:closing(shared, start, "(", ")")])

    ability_data = {}
    for name, body in structures(preprocess("data/abilities.h"), "ABILITY_"):
        values = fields(body)
        ability_data[name] = {
            "name": literal_strings(values.get("name", "")) or readable(name.replace("ABILITY_", "")),
            "description": literal_strings(values.get("description", "")),
        }
    family_text = "\n".join(preprocess(f"species_info/gen_{i}_families.h") for i in range(1, 10))
    details = {}
    for symbol, body in structures(family_text, "SPECIES_"):
        f = fields(body)
        nat = symbols(f.get("natDexNum", ""), "NATIONAL_DEX_")
        dex = dex_by_national.get(nat[0]) if nat else dex_by_symbol.get(symbol)
        raw_description = f.get("description", "")
        description = literal_strings(raw_description)
        if description is None:
            ref = re.search(r"\bg[A-Za-z0-9_]+PokedexText\b", raw_description)
            description = shared_descriptions.get(ref.group(0)) if ref else None
        base = {key: number_expr(f.get(key, "")) for key in STATS}
        abilities = symbols(f.get("abilities", ""), "ABILITY_")
        standard = [a for a in abilities[:2] if a != "ABILITY_NONE"]
        hidden = abilities[2] if len(abilities) > 2 and abilities[2] != "ABILITY_NONE" else None
        types = symbols(f.get("types", ""), "TYPE_")
        types = list(dict.fromkeys(t.replace("TYPE_", "").title() for t in types if t != "TYPE_NONE"))
        details[symbol] = {
            "symbol": symbol, "dex": dex,
            "name": literal_strings(f.get("speciesName", "")),
            "category": literal_strings(f.get("categoryName", "")),
            "description": description,
            "stats": base if all(isinstance(x, int) and x > 0 for x in base.values()) else None,
            "bst": sum(base.values()) if all(isinstance(x, int) and x > 0 for x in base.values()) else None,
            "types": types[:2],
            "abilities": [dict(id=a, **ability_data.get(a, {"name": readable(a[8:]), "description": None})) for a in standard],
            "hiddenAbility": (dict(id=hidden, **ability_data.get(hidden, {"name": readable(hidden[8:]), "description": None})) if hidden else None),
            "catchRate": number_expr(f.get("catchRate", "")),
            "height": number_expr(f.get("height", "")),
            "weight": number_expr(f.get("weight", "")),
            "genderRatio": f.get("genderRatio"),
            "eggGroups": [readable(x[10:]) for x in symbols(f.get("eggGroups", ""), "EGG_GROUP_") if x != "EGG_GROUP_NONE"],
            "evolutions": evolution_list(f.get("evolutions", "")),
            "mega": f.get("isMegaEvolution") in ("TRUE", "1"),
            "primal": f.get("isPrimalReversion") in ("TRUE", "1"),
            "source": "src/data/pokemon/species_info/",
        }
    groups = find_form_members()
    by_dex = {str(item["dex"]): [] for item in catalog}
    for symbol, info in details.items():
        if info["dex"] is None:
            for sibling in groups.get(symbol, set()):
                if sibling in details and details[sibling]["dex"] is not None:
                    info["dex"] = details[sibling]["dex"]
                    break
        if info["dex"] in range(1, 1026):
            by_dex[str(info["dex"])].append(symbol)
    for item in catalog:
        symbol = item["symbol"]
        if symbol not in details or not details[symbol]["stats"]:
            raise ValueError("Missing compiled source stats for National #" + str(item["dex"]) + ": " + symbol + " => " + str(details.get(symbol)))
        if details[symbol]["dex"] != item["dex"]:
            raise ValueError("National Dex mismatch for " + symbol)
    missing_descriptions = [item["symbol"] for item in catalog if not details[item["symbol"]]["description"]]
    if missing_descriptions:
        raise ValueError("Missing configured Pokédex descriptions: " + ", ".join(missing_descriptions[:16]))
    output = {"schema": 1, "species": details, "formsByDex": by_dex}
    OUT.parent.mkdir(parents=True, exist_ok=True)
    OUT.write_text(json.dumps(output, separators=(",", ":"), ensure_ascii=False))
    print(f"Extracted {len(details)} internal species forms, {len(catalog)} National species; generated website details.")


if __name__ == "__main__":
    main()
