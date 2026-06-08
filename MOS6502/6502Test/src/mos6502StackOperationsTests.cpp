#include <gtest/gtest.h>
#include <m6502.h>

class MOS6502StackOperationsTests : public testing::Test {
	public:
		mos6502::Memory memory;
		mos6502::CPU cpu;
		virtual void SetUp() {
			cpu.Reset(0xFFFC, memory);
		}
		virtual void TearDown() {

		}
};

TEST_F(MOS6502StackOperationsTests, TSXCanTransferZeroStackPointerToXRegister) {
	using namespace mos6502;
	// given
	cpu.Reset(0xFF00, memory);
	cpu.Flag.Z = cpu.Flag.N = true;
	cpu.X = 0x00;
	cpu.SP = 0x01;
	memory[0xFF00] = CPU::TSX;

	constexpr s32 EXPECTED_CYCLES = 2;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.X, 0x01);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(MOS6502StackOperationsTests, TSXCanTransferZeroStackPointerToXRegisterSettingFlags) {
	using namespace mos6502;
	// given
	cpu.Reset(0xFF00, memory);
	cpu.Flag.Z = cpu.Flag.N = true;
	cpu.X = 0x00;
	cpu.SP = 0x00;
	memory[0xFF00] = CPU::TSX;

	constexpr s32 EXPECTED_CYCLES = 2;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.X, 0x00);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
}

TEST_F(MOS6502StackOperationsTests, TSXCanTransferNegativeStackPointerToXRegisterSettingFlags) {
	using namespace mos6502;
	// given
	cpu.Reset(0xFF00, memory);
	cpu.Flag.Z = cpu.Flag.N = true;
	cpu.X = 0x00;
	cpu.SP = 0x80;
	memory[0xFF00] = CPU::TSX;

	constexpr s32 EXPECTED_CYCLES = 2;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.X, 0x80);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
}

TEST_F(MOS6502StackOperationsTests, TXSCanTransferXRegisterToStackPointer) {
	using namespace mos6502;
	// given
	cpu.Reset(0xFF00, memory);
	cpu.X = 0x9B;
	cpu.SP = 0x00;
	memory[0xFF00] = CPU::TXS;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 2;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.SP, 0x9B);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
}

TEST_F(MOS6502StackOperationsTests, PHACanPushARegisterOntoStack) {
	using namespace mos6502;
	// given
	cpu.Reset(0xFF00, memory);
	cpu.A = 0x42;
	memory[0xFF00] = CPU::PHA;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 3;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
	EXPECT_EQ(memory[cpu.SPToAddress()+1], 0x42);
}

TEST_F(MOS6502StackOperationsTests, PHPCanPushProcessorStatusRegisterOntoStack) {
	using namespace mos6502;
	// given
	cpu.Reset(0xFF00, memory);
	cpu.PS = 0xCC;
	memory[0xFF00] = CPU::PHP;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 3;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
	const Byte flagsOnStack = 0xCC | CPU::UnusedBitMask | CPU::BreakCommandBitMask;
	EXPECT_EQ(memory[cpu.SPToAddress()+1], flagsOnStack);
	EXPECT_EQ(cpu.SP, 0xFE);
}

TEST_F(MOS6502StackOperationsTests, PHPALwaysSetsBits4And5OnTheStack) {
	using namespace mos6502;
	// given
	cpu.Reset(0xFF00, memory);
	cpu.PS = 0x00;
	memory[0xFF00] = CPU::PHP;
	constexpr s32 EXPECTED_CYCLES = 3;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	Word addressPSOnStack = cpu.SPToAddress() + 1;
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	/*
	https://web.archive.org/web/20241124184248/https://www.nesdev.org/wiki/Status_flags

	*/
	const Byte flagsOnStack = CPU::UnusedBitMask | CPU::BreakCommandBitMask;
	EXPECT_EQ(memory[addressPSOnStack], flagsOnStack);
}

TEST_F(MOS6502StackOperationsTests, PLACanPullValueFromStackIntoARegister) {
	using namespace mos6502;
	// given
	cpu.Reset(0xFF00, memory);
	cpu.A = 0x0;
	cpu.SP = 0xFE;
	memory[0xFF00] = CPU::PLA;
	memory[0x01FF] = 0x67;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x67);
	EXPECT_EQ(cpu.SP, 0xFF);
}

TEST_F(MOS6502StackOperationsTests, PLACanPullZeroValueFromStackIntoARegister) {
	using namespace mos6502;
	// given
	cpu.Reset(0xFF00, memory);
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.A = 0x42;
	cpu.SP = 0xFE;
	memory[0xFF00] = CPU::PLA;
	memory[0x01FF] = 0x0;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x00);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_EQ(cpu.SP, 0xFF);
}

TEST_F(MOS6502StackOperationsTests, PLACanPullNegativeValueFromStackIntoARegister) {
	using namespace mos6502;
	// given
	cpu.Reset(0xFF00, memory);
	cpu.Flag.N = false;
	cpu.Flag.Z = true;
	cpu.A = 0x42;
	cpu.SP = 0xFE;
	memory[0xFF00] = CPU::PLA;
	memory[0x01FF] = 0x80;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x80);
	EXPECT_TRUE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_EQ(cpu.SP, 0xFF);
}

TEST_F(MOS6502StackOperationsTests, PLPCanPullValueFromStackIntoProcessorStatusRegister) {
	using namespace mos6502;
	// given
	cpu.Reset(0xFF00, memory);
	cpu.PS = 0x00;
	cpu.SP = 0xFE;
	memory[0xFF00] = CPU::PLP;
	memory[0x01FF] = 0x67 | CPU::BreakCommandBitMask | CPU::UnusedBitMask;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS, 0x67 & ~CPU::BreakCommandBitMask & ~CPU::UnusedBitMask);
}

TEST_F(MOS6502StackOperationsTests, PLPIgnoreUnusedAndBreakBits) {
	using namespace mos6502;
	// given
	cpu.Reset(0xFF00, memory);
	cpu.PS = CPU::BreakCommandBitMask | CPU::UnusedBitMask;
	cpu.SP = 0xFE;
	memory[0xFF00] = CPU::PLP;
	memory[0x01FF] = CPU::BreakCommandBitMask | CPU::UnusedBitMask;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS, 0x00);
}