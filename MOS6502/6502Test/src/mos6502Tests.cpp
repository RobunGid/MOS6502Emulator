#include <gtest/gtest.h>
#include <m6502.h>

class MOS6502Test1 : public testing::Test {
	public:
		mos6502::Memory memory;
		mos6502::CPU cpu;
		virtual void SetUp() {
			cpu.Reset(memory);
		}
		virtual void TearDown() {

		}
};

static void VerifyUnmodifiedFlagFromLDA(const mos6502::CPU& cpu, const mos6502::CPU& cpu_copy) {
	EXPECT_EQ(cpu.C, cpu_copy.C);
	EXPECT_EQ(cpu.I, cpu_copy.I);
	EXPECT_EQ(cpu.D, cpu_copy.D);
	EXPECT_EQ(cpu.B, cpu_copy.B);
	EXPECT_EQ(cpu.V, cpu_copy.V);
}

TEST_F( MOS6502Test1, TheCPUDoesNothingWhenExecutesZeroCycles ) {
	using namespace mos6502;
	
	// given
	constexpr s32 NUM_CYCLES = 0;
	
	// when
	s32 CyclesUsed = cpu.Execute(NUM_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, 0);

}

TEST_F( MOS6502Test1, CPUCanExecuteMoreCyclesThanRequestedIfRequiredByInstructions ) {
	using namespace mos6502;
	
	// given
	memory[0xFFFC] = CPU::LDA_IM;
	memory[0xFFFD] = 0x84;
	constexpr s32 NUM_CYCLES = 1;

	// when
	s32 CyclesUsed = cpu.Execute(NUM_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, 2);
}

TEST_F( MOS6502Test1, LDAImmeddiateCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;

	// given
	memory[0xFFFC] = CPU::LDA_IM;
	memory[0xFFFD] = 0x84;
	CPU cpu_copy = cpu;

	// when
	s32 CyclesUsed = cpu.Execute(2, memory);

	// then
	EXPECT_EQ(cpu.A, 0x84);
	EXPECT_EQ(CyclesUsed, 2);
	EXPECT_FALSE(cpu.Z);
	EXPECT_TRUE(cpu.N);
	VerifyUnmodifiedFlagFromLDA(cpu, cpu_copy);
}

TEST_F( MOS6502Test1, LDAImmeddiateCanAffectZeroFlag ) {
	using namespace mos6502;
	
	// given
	cpu.A = 0xff;
	memory[0xFFFC] = CPU::LDA_IM;
	memory[0xFFFD] = 0x00;
	CPU cpu_copy = cpu;

	// when
	cpu.Execute(2, memory);

	// then
	EXPECT_TRUE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLDA(cpu, cpu_copy);
}

TEST_F( MOS6502Test1, LDAZeroPageCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;

	// given
	memory[0xFFFC] = CPU::LDA_ZP;
	memory[0xFFFD] = 0x42;
	memory[0x42] = 0x67;
	CPU cpu_copy = cpu;

	// when
	s32 CyclesUsed = cpu.Execute(3, memory);

	// then
	EXPECT_EQ(cpu.A, 0x67);
	EXPECT_EQ(CyclesUsed, 3);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLDA(cpu, cpu_copy);
}

TEST_F( MOS6502Test1, LDAZeroPageXCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;

	// given
	cpu.X = 0x05;
	memory[0xFFFC] = CPU::LDA_ZP_X;
	memory[0xFFFD] = 0x42;
	memory[0x47] = 0x67;
	CPU cpu_copy = cpu;

	// when
	s32 CyclesUsed = cpu.Execute(4, memory);

	// then
	EXPECT_EQ(cpu.A, 0x67);
	EXPECT_EQ(CyclesUsed, 4);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLDA(cpu, cpu_copy);
}

TEST_F( MOS6502Test1, LDAZeroPageXCanLoadAValueIntoTheARegisterWhenItWraps ) {
	using namespace mos6502;
	
	// given
	cpu.X = 0xFF;
	memory[0xFFFC] = CPU::LDA_ZP_X;
	memory[0xFFFD] = 0x80;
	memory[0x7F] = 0x67;
	CPU cpu_copy = cpu;

	// when
	u32 CyclesUsed = cpu.Execute(4, memory);

	// then
	EXPECT_EQ(cpu.A, 0x67);
	EXPECT_EQ(CyclesUsed, 4);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLDA(cpu, cpu_copy);
}

TEST_F( MOS6502Test1, LDAAbsoluteCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;
	
	// given
	memory[0xFFFC] = CPU::LDA_ABS;
	memory[0xFFFD] = 0x80;
	memory[0xFFFE] = 0x54; // address 0x5480
	memory[0x5480] = 0x37;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, 0x37);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLDA(cpu, cpu_copy);
}

TEST_F( MOS6502Test1, LDAAbsoluteXCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;

	// given
	cpu.X = 0x01;
	memory[0xFFFC] = CPU::LDA_ABS_X;
	memory[0xFFFD] = 0x80;
	memory[0xFFFE] = 0x54; // address 0x5480 + 0x01 = 0x5481
	memory[0x5481] = 0x37;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, 0x37);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLDA(cpu, cpu_copy);
}

TEST_F( MOS6502Test1, LDAAbsoluteXCanLoadAValueIntoTheARegisterWithCrossPageBoundary ) {
	using namespace mos6502;

	// given
	cpu.X = 0xFF;
	memory[0xFFFC] = CPU::LDA_ABS_X;
	memory[0xFFFD] = 0x02;
	memory[0xFFFE] = 0x54; // address 0x5402 + 0xFF = 0x5501 that crosses page boundary
	memory[0x5501] = 0x37; 
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU cpu_copy = cpu;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, 0x37);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLDA(cpu, cpu_copy);
}

TEST_F( MOS6502Test1, LDAAbsoluteYCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;

	// given
	cpu.Y = 0x01;
	memory[0xFFFC] = CPU::LDA_ABS_Y;
	memory[0xFFFD] = 0x80;
	memory[0xFFFE] = 0x54; // address 0x5480 + 0x01 = 0x5481
	memory[0x5481] = 0x37;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, 0x37);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLDA(cpu, cpu_copy);
}

TEST_F( MOS6502Test1, LDAAbsoluteYCanLoadAValueIntoTheARegisterWithCrossPageBoundary ) {
	using namespace mos6502;

	// given
	cpu.Y = 0xFF;
	memory[0xFFFC] = CPU::LDA_ABS_Y;
	memory[0xFFFD] = 0x02;
	memory[0xFFFE] = 0x54; // address 0x5402 + 0xFF = 0x5501 that crosses page boundary
	memory[0x5501] = 0x37; 
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU cpu_copy = cpu;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, 0x37);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLDA(cpu, cpu_copy);
}

TEST_F( MOS6502Test1, LDAIndirectXCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;

	// given
	cpu.X = 0x04;
	memory[0xFFFC] = CPU::LDA_IND_X;
	memory[0xFFFD] = 0x02;
	memory[0x0006] = 0x00; // 0x02 + 0x04 = 0x06
	memory[0x0007] = 0x80; 
	memory[0x8000] = 0x16; 
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU cpu_copy = cpu;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, 0x16);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLDA(cpu, cpu_copy);
}

TEST_F( MOS6502Test1, LDAIndirectYCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;

	// given
	cpu.Y = 0x04;
	memory[0xFFFC] = CPU::LDA_IND_Y;
	memory[0xFFFD] = 0x02;
	memory[0x0002] = 0x00; 
	memory[0x0003] = 0x80; 
	memory[0x8004] = 0x16;  // 0x8000 + 0x04 = 0x8004
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU cpu_copy = cpu;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, 0x16);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLDA(cpu, cpu_copy);
}

TEST_F( MOS6502Test1, LDAIndirectYCanLoadAValueIntoTheARegisterWhenCrossesPageBoundary ) {
	using namespace mos6502;

	// given
	cpu.Y = 0xFF;
	memory[0xFFFC] = CPU::LDA_IND_Y;
	memory[0xFFFD] = 0x02;
	memory[0x0002] = 0x02;
	memory[0x0003] = 0x80; 
	memory[0x8101] = 0x16; // 0x8002 + 0xFF = 0x8101
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU cpu_copy = cpu;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, 0x16);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLDA(cpu, cpu_copy);
}