#include <gtest/gtest.h>
#include <m6502.h>

class MOS6502BranchTests : public testing::Test {
	public:
		mos6502::Memory memory;
		mos6502::CPU cpu;
		virtual void SetUp() {
			cpu.Reset(0xFFFC, memory);
		}
		virtual void TearDown() {

		}

};

TEST_F(MOS6502BranchTests, BEQCanBranchForwardWhenZeroIsSet) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = true;
	memory[0xFFFC] = CPU::BEQ;
	memory[0xFFFD] = 0x01;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PC, 0xFFFF);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
}

TEST_F(MOS6502BranchTests, BNECanBranchForwardWhenZeroIsUnset) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = false;
	memory[0xFFFC] = CPU::BNE;
	memory[0xFFFD] = 0x01;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PC, 0xFFFF);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
};

TEST_F(MOS6502BranchTests, BCSCanBranchForwardWhenCarryFlagIsSet) {
	using namespace mos6502;

	// given
	cpu.Flag.C = true;
	memory[0xFFFC] = CPU::BCS;
	memory[0xFFFD] = 0x01;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PC, 0xFFFF);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
};

TEST_F(MOS6502BranchTests, BCCCanBranchForwardWhenCarryIsUnset) {
	using namespace mos6502;

	// given
	cpu.Flag.C = false;
	memory[0xFFFC] = CPU::BCC;
	memory[0xFFFD] = 0x01;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PC, 0xFFFF);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
};

TEST_F(MOS6502BranchTests, BMICanBranchForwardWhenNegativeFlagIsSet) {
	using namespace mos6502;

	// given
	cpu.Flag.N = true;
	memory[0xFFFC] = CPU::BMI;
	memory[0xFFFD] = 0x01;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PC, 0xFFFF);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
};

TEST_F(MOS6502BranchTests, BPLCanBranchForwardWhenNegativeFlagIsUnset) {
	using namespace mos6502;

	// given
	cpu.Flag.N = false;
	memory[0xFFFC] = CPU::BPL;
	memory[0xFFFD] = 0x01;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PC, 0xFFFF);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
};

TEST_F(MOS6502BranchTests, BVCCanBranchForwardWhenOverflowFlagIsUnset) {
	using namespace mos6502;

	// given
	cpu.Flag.V = false;
	memory[0xFFFC] = CPU::BVC;
	memory[0xFFFD] = 0x01;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PC, 0xFFFF);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
};

TEST_F(MOS6502BranchTests, BVSCanBranchForwardWhenOverflowFlagIsSet) {
	using namespace mos6502;

	// given
	cpu.Flag.V = true;
	memory[0xFFFC] = CPU::BVS;
	memory[0xFFFD] = 0x01;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PC, 0xFFFF);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
};

TEST_F(MOS6502BranchTests, BEQDoesNotBranchForwardWhenZeroIsUnset) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = false;
	memory[0xFFFC] = CPU::BEQ;
	memory[0xFFFD] = 0x01;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PC, 0xFFFE);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
}

TEST_F(MOS6502BranchTests, BEQCanBranchForwardToNewPageWhenZeroIsSet) {
	using namespace mos6502;

	// given
	cpu.Reset(0xFEFD, memory);
	cpu.Flag.Z = true;
	memory[0xFEFD] = CPU::BEQ;
	memory[0xFEFE] = 0x01;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PC, 0xFF00);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
}

TEST_F(MOS6502BranchTests, BEQCanBranchBackwardWhenZeroIsSet) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = true;
	memory[0xFFFC] = CPU::BEQ;
	memory[0xFFFD] = 0xFE;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PC, 0xFFFC);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
}

TEST_F(MOS6502BranchTests, BEQDoesNotBranchBackwardWhenZeroIsUnset) {
	using namespace mos6502;

	// given
	cpu.Flag.Z = false;
	memory[0xFFFC] = CPU::BEQ;
	memory[0xFFFD] = 0xFE;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PC, 0xFFFE);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
}

TEST_F(MOS6502BranchTests, BEQCanBranchBackwardToNewPageWhenZeroIsSet) {
	using namespace mos6502;

	// given
	cpu.Reset(0xFEFF, memory);
	cpu.Flag.Z = true;
	memory[0xFEFF] = CPU::BEQ;
	memory[0xFF00] = 0xFE;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PC, 0xFEFF);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
};

TEST_F(MOS6502BranchTests, TestLoadAndExecuteProgramInfiniteLoop) {
	using namespace mos6502;

	// given
	cpu.Reset(0xFFCC, memory);
	/*
	loop
	lda #0
	beq loop
	*/
	memory[0xFFCC] = 0xA9;
	memory[0xFFCD] = 0x00;
	memory[0xFFCE] = 0xF0;
	memory[0xFFCF] = 0xFC;
	constexpr s32 EXPECTED_CYCLES = 2 + 3;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PC, 0xFFCC);
};
