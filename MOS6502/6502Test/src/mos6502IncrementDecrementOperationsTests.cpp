#include <gtest/gtest.h>
#include <m6502.h>

class MOS6502IncrementDecrementOperationsTests : public testing::Test {
	public:
		mos6502::Memory memory;
		mos6502::CPU cpu;
		virtual void SetUp() {
			cpu.Reset(0xFFFC, memory);
		}
		virtual void TearDown() {

		}

};

static void VerifyUnmodifiedFlagFromLoadRegister(const mos6502::CPU& cpu, const mos6502::CPU& cpu_copy) {
	EXPECT_EQ(cpu.Flag.C, cpu_copy.Flag.C);
	EXPECT_EQ(cpu.Flag.I, cpu_copy.Flag.I);
	EXPECT_EQ(cpu.Flag.D, cpu_copy.Flag.D);
	EXPECT_EQ(cpu.Flag.B, cpu_copy.Flag.B);
	EXPECT_EQ(cpu.Flag.V, cpu_copy.Flag.V);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, INXCanIncrementXValue) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x00;
	memory[0xFFFC] = CPU::INX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.X, 0x01);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, INXCanIncrementXToZeroValue) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.X = 0xFF;
	memory[0xFFFC] = CPU::INX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.X, 0x00);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, INXCanIncrementXToNegativeValue) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	cpu.X = 0x80;
	memory[0xFFFC] = CPU::INX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.X, 0x81);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, INYCanIncrementYValue) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.Y = 0x00;
	memory[0xFFFC] = CPU::INY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.Y, 0x01);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, INYCanIncrementYToZeroValue) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.Y = 0xFF;
	memory[0xFFFC] = CPU::INY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.Y, 0x00);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, INYCanIncrementYToNegativeValue) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	cpu.Y = 0x80;
	memory[0xFFFC] = CPU::INY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.Y, 0x81);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, DEXCanDecrementXValue) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x02;
	memory[0xFFFC] = CPU::DEX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.X, 0x01);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, DEXCanDecrementXToZeroValue) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.X = 0x01;
	memory[0xFFFC] = CPU::DEX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.X, 0x00);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, DEXCanDecrementXToNegativeValue) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	cpu.X = 0x81;
	memory[0xFFFC] = CPU::DEX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.X, 0x80);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, DEYCanDecrementYValue) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.Y = 0x02;
	memory[0xFFFC] = CPU::DEY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.Y, 0x01);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, DEYCanDecrementYToZeroValue) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.Y = 0x01;
	memory[0xFFFC] = CPU::DEY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.Y, 0x00);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, DEYCanIncrementYToNegativeValue) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	cpu.Y = 0x81;
	memory[0xFFFC] = CPU::DEY;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.Y, 0x80);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, INCZeroPageCanIncrementValue) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	memory[0xFFFC] = CPU::INC_ZP;
	memory[0xFFFD] = 0x80;
	memory[0x80] = 0x00;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(memory[0x80], 0x01);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, DECZeroPageCanDecrementValue) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	memory[0xFFFC] = CPU::DEC_ZP;
	memory[0xFFFD] = 0x80;
	memory[0x80] = 0x02;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(memory[0x80], 0x01);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, INCZeroPageXCanIncrementValue) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	memory[0xFFFC] = CPU::INC_ZP_X;
	memory[0xFFFD] = 0x80;
	memory[0x90] = 0x00;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(memory[0x90], 0x01);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, DECZeroPageXCanDecrementValue) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	memory[0xFFFC] = CPU::DEC_ZP_X;
	memory[0xFFFD] = 0x80;
	memory[0x90] = 0x02;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(memory[0x90], 0x01);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, INCAbsoluteCanIncrementValue) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	memory[0xFFFC] = CPU::INC_ABS;
	memory[0xFFFD] = 0x30;
	memory[0xFFFE] = 0x80;
	memory[0x8030] = 0x00;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(memory[0x8030], 0x01);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, DECAbsoluteCanDecrementValue) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	memory[0xFFFC] = CPU::DEC_ABS;
	memory[0xFFFD] = 0x30;
	memory[0xFFFE] = 0x80;
	memory[0x8030] = 0x02;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(memory[0x8030], 0x01);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, INCAbsoluteXCanIncrementValue) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	memory[0xFFFC] = CPU::INC_ABS_X;
	memory[0xFFFD] = 0x30;
	memory[0xFFFE] = 0x80;
	memory[0x8040] = 0x00;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(memory[0x8040], 0x01);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, DECAbsoluteXCanDecrementValue) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.X = 0x10;
	memory[0xFFFC] = CPU::DEC_ABS_X;
	memory[0xFFFD] = 0x30;
	memory[0xFFFE] = 0x80;
	memory[0x8040] = 0x02;
	constexpr s32 EXPECTED_CYCLES = 7;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(memory[0x8040], 0x01);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F(MOS6502IncrementDecrementOperationsTests, TestLoadAndExecuteProgramIncrementingMemory) {
	using namespace mos6502;

	// when
	/*
	* = $1000
	lda #$00
	sta $42

	start
	inc $42
	ldx $42
	inx
	jmp start
	*/
	Byte prg[] = {
		0x00, 0x10, 0xa9, 0x00, 0x85, 0x42, 0xe6, 0x42, 
		0xa6, 0x42, 0xe8, 0x4c, 0x04, 0x10, 
	};

	Word startAddress = cpu.LoadProgram(prg, 14, memory);
	cpu.PC = startAddress;

	// then
	for (mos6502::s32 clock = 1000000; clock > 0;) {
		clock -= cpu.Execute(1, memory);
	}
};
