#!/usr/bin/env python3
"""Resolve exact A2 runtime boot symbols from the linked game, never hardcode offsets."""
import argparse
from pathlib import Path
import subprocess

SYMBOLS = {
    "gMain": "A2_GMAIN",
    "CB2_Overworld": "A2_CB_OVERWORLD",
    "gSaveBlock1Ptr": "A2_SB1_PTR",
}

# These may be hidden by release LTO. Do not guess if unavailable.
OPTIONAL_SYMBOLS = {
    "sUsmState": "A2_USM_STATE_PTR",
    "sUsmMemory": "A2_USM_MEMORY_PTR",
    "gFlashMemoryPresent": "A2_FLASH_PRESENT",
    "gSaveCounter": "A2_SAVE_COUNTER",
    "gSaveAttemptStatus": "A2_SAVE_ATTEMPT_STATUS",
}


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("elf")
    parser.add_argument("--github-env", required=True)
    args = parser.parse_args()
    result = subprocess.run(
        ["arm-none-eabi-nm", "-a", args.elf],
        capture_output=True, text=True, check=True,
    )
    found = {}
    for row in result.stdout.splitlines():
        parts = row.split()
        if len(parts) != 3:
            continue
        address, kind, name = parts
        if name not in SYMBOLS and name not in OPTIONAL_SYMBOLS or kind.upper() == "U":
            continue
        value = int(address, 16)
        if name in found and found[name] != value:
            parser.error("Ambiguous linked symbol: " + name)
        found[name] = value
    missing = [name for name in SYMBOLS if name not in found]
    if missing:
        parser.error("Linked game lacks required runtime symbols: " + ", ".join(missing))

    # Confirm every reference is in an expected GBA address range.
    if not 0x03000000 <= found["gMain"] < 0x03008000:
        parser.error("gMain is outside GBA IWRAM")
    if not 0x03000000 <= found["gSaveBlock1Ptr"] < 0x03008000:
        parser.error("save-block pointer is outside GBA IWRAM")
    if not 0x08000000 <= found["CB2_Overworld"] < 0x0A000000:
        parser.error("overworld code is outside native GBA ROM")

    output = Path(args.github_env)
    with output.open("a") as handle:
        for name, variable in SYMBOLS.items():
            value = hex(found[name])
            handle.write(f"{variable}={value}\n")
            print(f"{variable}={value}")
        for name, variable in OPTIONAL_SYMBOLS.items():
            if name not in found:
                print("Optional A2 runtime symbol unavailable: " + name)
                continue
            value = hex(found[name])
            handle.write(f"{variable}={value}\n")
            print(f"{variable}={value}")


if __name__ == "__main__":
    main()
