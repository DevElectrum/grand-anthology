#!/usr/bin/env python3
"""Resolve exact A2 runtime boot symbols from the linked game, never hardcode offsets."""
import argparse
from pathlib import Path
import subprocess

SYMBOLS = {
    "gMain": "A2_GMAIN",
    "CB2_Overworld": "A2_CB_OVERWORLD",
    "gSaveBlock1Ptr": "A2_SB1_PTR",
    # A2-only volatile pointer, stored by the live menu initialization.
    "gXgbaA2UsmStateProbe": "A2_USM_STATE_PTR",
    "gXgbaA2SaveDialogTrace": "A2_SAVE_DIALOG_TRACE",
    "gXgbaA2UsmTrace": "A2_USM_TRACE",
    "gXgbaA2GameSaveTrace": "A2_GAME_SAVE_TRACE",
    "gXgbaA2PagerTrace": "A2_PAGER_TRACE",
    "gXgbaA2TransactionTrace": "A2_TRANSACTION_TRACE",
    "gXgbaA2NativeWriteTrace": "A2_NATIVE_WRITE_TRACE",
    "gXgbaA2FlashWriteTrace": "A2_FLASH_WRITE_TRACE",
}

# These may be hidden by release LTO. Do not guess if unavailable.
OPTIONAL_SYMBOLS = {
    "gFlashMemoryPresent": "A2_FLASH_PRESENT",
    "gParties": "A2_R8_PARTIES",
    "gSaveBlock2Ptr": "A2_R8_SB2_PTR",
    "gSaveCounter": "A2_SAVE_COUNTER",
    "gSaveAttemptStatus": "A2_SAVE_ATTEMPT_STATUS",
    "gXgbaR8PcUiTrace": "A2_R8_PC_UI_TRACE",
    "gXgbaR8PcChooserTrace": "A2_R8_PC_CHOOSER_TRACE",
    "gXgbaR8PcC3DTrace": "A2_R8_PC_C3D_TRACE",
    "gXgbaR8PcC3DDepositDiagnostic": "A2_R8_PC_DEPOSIT_DIAG",
    "gXgbaR8FixtureStage": "A2_R8_C4_PROOF",
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
    usm = found["gXgbaA2UsmStateProbe"]
    if not ((0x02000000 <= usm < 0x02040000) or (0x03000000 <= usm < 0x03008000)):
        parser.error("live USM pointer symbol outside GBA RAM")
    trace = found["gXgbaA2SaveDialogTrace"]
    if not ((0x02000000 <= trace < 0x02040000) or (0x03000000 <= trace < 0x03008000)):
        parser.error("A2 Save callback trace outside GBA RAM")
    usm_trace = found["gXgbaA2UsmTrace"]
    if not ((0x02000000 <= usm_trace < 0x02040000) or (0x03000000 <= usm_trace < 0x03008000)):
        parser.error("A2 graphical menu trace outside GBA RAM")
    game_trace = found["gXgbaA2GameSaveTrace"]
    if not ((0x02000000 <= game_trace < 0x02040000) or (0x03000000 <= game_trace < 0x03008000)):
        parser.error("A2 save transaction trace outside GBA RAM")
    pager_trace = found["gXgbaA2PagerTrace"]
    if not ((0x02000000 <= pager_trace < 0x02040000) or (0x03000000 <= pager_trace < 0x03008000)):
        parser.error("A2 PC pager trace outside GBA RAM")
    transaction_trace = found["gXgbaA2TransactionTrace"]
    if not ((0x02000000 <= transaction_trace < 0x02040000) or (0x03000000 <= transaction_trace < 0x03008000)):
        parser.error("A2 transaction trace outside GBA RAM")
    for diagnostic in ("gXgbaA2NativeWriteTrace", "gXgbaA2FlashWriteTrace"):
        value = found[diagnostic]
        if not ((0x02000000 <= value < 0x02040000) or (0x03000000 <= value < 0x03008000)):
            parser.error(diagnostic + " outside GBA RAM")

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
