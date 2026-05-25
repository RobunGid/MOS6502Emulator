#include <gtest/gtest.h>
#include <m6502.h>

class MOS6502SystemFunctionsTests : public testing::Test {
	public:
		mos6502::Memory memory;
		mos6502::CPU cpu;
		virtual void SetUp() {
			cpu.Reset(0xFFFC, memory);
		}
		virtual void TearDown() {

		}

};

TEST_F(MOS6502SystemFunctionsTests, NOPDoesNothing) {
	using namespace mos6502;

	// given
	memory[0xFFFC] = CPU::NOP;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
	EXPECT_EQ(cpu.PC, 0xFFFD);
	EXPECT_EQ(cpu.X, cpu_copy.X);
	EXPECT_EQ(cpu.Y, cpu_copy.Y);
	EXPECT_EQ(cpu.A, cpu_copy.A);
	EXPECT_EQ(cpu.SP, cpu_copy.SP);
}
