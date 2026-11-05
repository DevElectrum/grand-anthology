#!/usr/bin/env python3
"""Independently audit exactly one real GUI C4 release after Save3."""
import sys
from xgba_a2_storage import audit, verified_collection_box_bytes, BOX_SLOTS


def main():
    if len(sys.argv) != 3:
        raise SystemExit("usage: xgba_r8_c4_gui_audit.py Save2.sav Save3.sav")
    before, after = sys.argv[1:]
    first, second = audit(before), audit(after)
    if first["counter"] != 2 or second["counter"] != 3:
        raise SystemExit("C4 requires actual Save2 -> Save3")
    a = verified_collection_box_bytes(before, first)
    b = verified_collection_box_bytes(after, second)
    if len(a) != 3360 * 80 or len(b) != len(a):
        raise SystemExit("Incorrect global collection size")
    slot = 55 * BOX_SLOTS + 3  # global Box 56, slot 4, zero indexed
    offset = slot * 80
    if a[offset:offset+80] == bytes(80):
        raise SystemExit("C4 precondition: unmarked boxed candidate missing")
    if b[offset:offset+80] != bytes(80):
        raise SystemExit("C4 failed: released Pokémon still present")
    if a[:offset] != b[:offset] or a[offset+80:] != b[offset+80:]:
        raise SystemExit("C4 corruption: unrelated protected record changed")
    print("R8_C4_INDEPENDENT_SAVE3_AUDIT_PASS changed_slots=1 unchanged_slots=3359")


if __name__ == "__main__":
    main()
