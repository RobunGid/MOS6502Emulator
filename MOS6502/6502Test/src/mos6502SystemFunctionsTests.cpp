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

TEST_F(MOS6502SystemFunctionsTests, BRKCanLoadProgramCounterFromInterruptVector) {
	using namespace mos6502;

	// given
	memory[0xFFFC] = CPU::BRK;
	memory[0xFFFE] = 0x00;
	memory[0xFFFF] = 0x80;
	constexpr s32 EXPECTED_CYCLES = 7;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PC, 0x8000);
}

TEST_F(MOS6502SystemFunctionsTests, BRKCanLoadProgramCounterFromInterruptVectorAnotherAddress) {
	using namespace mos6502;

	// given
	memory[0xFFFC] = CPU::BRK;
	memory[0xFFFE] = 0x00;
	memory[0xFFFF] = 0x90;
	constexpr s32 EXPECTED_CYCLES = 7;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PC, 0x9000);
}

TEST_F(MOS6502SystemFunctionsTests, BRKCanSetBreakStatusFlag) {
	using namespace mos6502;

	// given
	memory[0xFFFC] = CPU::BRK;
	memory[0xFFFE] = 0x00;
	memory[0xFFFF] = 0x90;
	cpu.Flag.B = false;
	constexpr s32 EXPECTED_CYCLES = 7;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_TRUE(cpu.Flag.B);
}

TEST_F(MOS6502SystemFunctionsTests, BRKCanChangeStackPointer) {
	using namespace mos6502;

	// given
	memory[0xFFFC] = CPU::BRK;
	memory[0xFFFE] = 0x00;
	memory[0xFFFF] = 0x90;
	cpu.Flag.B = false;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.SP, cpu_copy.SP - 3);
}

TEST_F(MOS6502SystemFunctionsTests, BRKCanPushPCAndPSOntoStack) {
	using namespace mos6502;

	// given
	cpu.Reset(0xFF00, memory);
	memory[0xFF00] = CPU::BRK;
	memory[0xFFFE] = 0x00;
	memory[0xFFFF] = 0x90;
	cpu.Flag.B = false;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU cpu_copy = cpu;
	Byte oldSP = cpu_copy.SP;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(memory[(0x100 | oldSP)-0], 0xFF);
	EXPECT_EQ(memory[(0x100 | oldSP)-1], 0x01);
	EXPECT_EQ(memory[(0x100 | oldSP)-2], cpu_copy.PS);
}

TEST_F(MOS6502SystemFunctionsTests, RTICanReturnFromInterrupt) {
	using namespace mos6502;

	// given
	cpu.Reset(0xFF00, memory);
	memory[0xFF00] = CPU::BRK;
	memory[0xFFFE] = 0x00;
	memory[0xFFFF] = 0x80;
	memory[0x8000] = CPU::RTI;
	cpu.Flag.B = false;
	constexpr s32 EXPECTED_CYCLES = 7 + 6;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.SP, cpu_copy.SP);
	EXPECT_EQ(cpu.PC, 0xFF01);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
}