#ifndef GUARD_XGBA_R8_RUNTIME_TEST_H
#define GUARD_XGBA_R8_RUNTIME_TEST_H

// Non-distributed, test-only cartridge code.
extern volatile u32 gXgbaR8FixtureStage;
int XgbaR8CreateTestPokemon(void);
int XgbaR8VerifyTestPokemon(void);

#endif
