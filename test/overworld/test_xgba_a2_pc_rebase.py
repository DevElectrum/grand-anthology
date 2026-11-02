"""A2 pager must follow the copied PokemonStorage after ASLR rebase."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]


class TestXgbaA2PcRebase(unittest.TestCase):
    def test_rebind_only_after_completed_pc_copy(self):
        source = (ROOT / "src/load_save.c").read_text()
        fn = source[source.index("void MoveSaveBlocks_ResetHeap(void)"):source.index("u32 UseContinueGameWarp(void)")]
        completed_copy = fn.index("*gPokemonStoragePtr = *pokemonStorageCopy;")
        rebind = fn.index("XgbaA2GameRebindRelocatedPc();")
        heap_reset = fn.index("InitHeap(gHeap, HEAP_SIZE);")
        self.assertLess(completed_copy, rebind)
        self.assertLess(rebind, heap_reset)
        self.assertIn("#if defined(XGBA_A2) && XGBA_A2", fn)

    def test_rebind_preserves_cohort_and_fatal_lockout(self):
        source = (ROOT / "src/save.c").read_text()
        start = source.index("void XgbaA2GameRebindRelocatedPc(void)")
        end = source.index("bool8 XgbaA2GameSwitchPage(u8 page)", start)
        body = source[start:end]
        self.assertIn("if (!sA2Ready || !sA2Pager.ready || sA2Pager.fatal", body)
        self.assertIn("sA2Pager.resident = gPokemonStoragePtr;", body)
        self.assertNotIn("sA2Pager.staged[", body)
        self.assertNotIn("sA2Pager.committed[", body)
        self.assertNotIn("sA2Committed =", body)
        self.assertNotIn("sA2Pager.fatal =", body)
        self.assertNotIn("XgbaA2JournalStage", body)
        self.assertNotIn("XgbaA2JournalRead", body)

    def test_native_writer_strict_pointer_guard_stays(self):
        source = (ROOT / "src/save.c").read_text()
        self.assertIn("if (pc != gPokemonStoragePtr)", source)
        header = (ROOT / "include/xgba_a2_game.h").read_text()
        self.assertIn("void XgbaA2GameRebindRelocatedPc(void);", header)


if __name__ == "__main__":
    unittest.main()
