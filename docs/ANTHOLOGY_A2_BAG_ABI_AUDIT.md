# A2 global bag ABI reconnaissance (read-only, 2026-11-01 local)

This is preparation for the A2.5 bag gate, NOT an implementation or approval to change the native SaveBlock1 layout.

## Existing invariants

- `include/global.h`: `struct SaveBlock1` contains the one global `struct Bag`, global `pcItems` and the same party/progression across regions. The comments document prior expansion: Items 30 to 60 slots, Key Items 30 to 99, item PC 50 to 150; total Bag 1,140 bytes, PC item storage 600 bytes. The native SaveBlock1 payload is already pinned to 14,752 bytes. Increasing its embedded arrays blindly shifts every subsequent field, breaks native Flash sectors and risks trainer/progression corruption.
- `include/global.h` `struct Bag`: five arrays of `struct ItemSlot`, one for each pocket, using `enum Item` plus 16-bit quantity.
- `src/item.c` `SetBagItemsPointers`: `gBagPockets` points directly into `gSaveBlock1Ptr->bag`, with capacities `BAG_ITEMS_COUNT`, `BAG_KEYITEMS_COUNT`, `BAG_POKEBALLS_COUNT`, `BAG_TMHM_COUNT` and `BAG_BERRIES_COUNT`. Menus, scripts, trades, item receipts and sorting use these pointers: changing only UI limits or one access function is not sufficient.
- `include/item.h` `struct BagPocket`: capacity occupies 10 bits; a future pocket capacity cannot exceed 1,023 without changing this ABI.
- `src/item.c` generic bag pocket quantities are XOR-encrypted with `gSaveBlock2Ptr->encryptionKey`; item PC quantities are unencrypted. Addition, removal, compact, move, quantity checking and encryption-key rotation must all agree with any extension.
- Key-class duplicate ownership checks include Pokémon held items, not just bag/PC. This is relevant to collection-wide bulk release and all-global unique items.
- Do not create region-specific bags. The single travelling trainer and item inventory are deliberate.

## Design restrictions for the future implementation

1. Complete native+PC journal save/restart test before introducing bag pages. Existing A2 metadata has only the 56-byte payload containing native fingerprint, native counter/slot, active PC page and eight PC generations. Any extra bag generation requires a versioned new metadata schema or a safely pinned record, not an untracked sidecar.
2. Keep native `SaveBlock1` fields and their byte positions invariant. If expansion exceeds the existing slots, consider a bounded extension-owned overflow record with a small index/cache and explicit migration, not a second enlarged `struct SaveBlock1`.
3. Bag operations need atomic preflight for global release held-item returns. Run the complete item's maximum capacity simulation *before deleting any Pokémon*, then commit all selected changes in one coherent native+extended save generation.
4. All operations must support interruption/reboot with the same cohort semantics as PC pages. Failed reads and uncommitted bag generations cannot be represented as an empty inventory.
5. Test cross-region item uniqueness, stacks, sorting, pocket overflow, full bag, PC item transfer, gift/held items, quantity encryption after trainer-key rotation, old-save migration and a second save/restart.
6. Do not change these ABI-sensitive pieces until A2.2/A2.3 gates are green. In particular no `BAG_*_COUNT` expansion in the native struct as a shortcut.

Relevant sources: `include/global.h`, `include/item.h`, `src/item.c`, `src/save.c`, `src/xgba_a2_transaction.c`, `include/xgba_a2_storage_format.h`.
