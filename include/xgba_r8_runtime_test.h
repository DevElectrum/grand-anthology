#ifndef GUARD_XGBA_R8_RUNTIME_TEST_H
#define GUARD_XGBA_R8_RUNTIME_TEST_H

// Non-distributed, test-only cartridge code.
extern volatile u32 gXgbaR8FixtureStage;
extern volatile u32 gXgbaR8PcUiTrace;
extern volatile u32 gXgbaR8PcChooserTrace;
extern volatile u32 gXgbaR8PcC3DTrace;
bool8 XgbaR8C3DWithdrawProof(void);
bool8 XgbaR8C3DRemoteDepositProof(void);
int XgbaR8CreateTestPokemon(void);
int XgbaR8VerifyTestPokemon(void);
int XgbaR8VerifyTestPokemonAtCounter(u32 counter);

#endif
