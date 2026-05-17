#include <gtest/gtest.h>
#include <m6502.h>

class MOS6502StoreRegisterTests : public testing::Test {
	public:
		mos6502::Memory memory;
		mos6502::CPU cpu;
		virtual void SetUp() {
			cpu.Reset(0xFFFC, memory);
		}
		virtual void TearDown() {

		}

};

TEST_F(MOS6502StoreRegisterTests, JSRCanJumpToASubroutineAndJumpBack) {
	using namespace mos6502;
	// given
	cpu.Reset( 0xFF00, memory );
	memory[0xFF00] = CPU::JSR;
	memory[0xFF01] = 0x00;
	memory[0xFF02] = 0x80;
	memory[0x8000] = CPU::RTS;
	memory[0xFF03] = CPU::LDA_IM;
	memory[0xFF04] = 0x2F;
	constexpr s32 EXPECTED_CYCLES = 6 + 6 + 2;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x2F);
}
