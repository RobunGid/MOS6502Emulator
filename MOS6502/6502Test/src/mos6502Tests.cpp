#include <gtest/gtest.h>
#include <main_6502.h>

class MOS6502Test1 : public testing::Test {
	public:
		Memory memory;
		CPU cpu;
		virtual void SetUp() {
			cpu.Reset(memory);
		}
		virtual void TearDown() {

		}
};

static void VerifyUnmodifiedFlagFromLDA(const CPU& cpu, const CPU& cpu_copy) {
	EXPECT_EQ(cpu.C, cpu_copy.C);
	EXPECT_EQ(cpu.I, cpu_copy.I);
	EXPECT_EQ(cpu.D, cpu_copy.D);
	EXPECT_EQ(cpu.B, cpu_copy.B);
	EXPECT_EQ(cpu.V, cpu_copy.V);
}

TEST_F( MOS6502Test1, LDAImmeddiateCanLoadAValueIntoTheARegister ) {
	// given
	memory[0xFFFC] = CPU::LDA_IM;
	memory[0xFFFD] = 0x84;

	// when
	CPU cpu_copy = cpu;
	s32 CyclesUsed = cpu.Execute(2, memory);

	// then
	EXPECT_EQ(cpu.A, 0x84);
	EXPECT_EQ(CyclesUsed, 2);
	EXPECT_FALSE(cpu.Z);
	EXPECT_TRUE(cpu.N);
	VerifyUnmodifiedFlagFromLDA(cpu, cpu_copy);

}

TEST_F( MOS6502Test1, LDAZeroPageCanLoadAValueIntoTheARegister ) {
	// given
	memory[0xFFFC] = CPU::LDA_ZP;
	memory[0xFFFD] = 0x42;
	memory[0x42] = 0x67;

	// when
	CPU cpu_copy = cpu;
	s32 CyclesUsed = cpu.Execute(3, memory);

	// then
	EXPECT_EQ(cpu.A, 0x67);
	EXPECT_EQ(CyclesUsed, 3);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLDA(cpu, cpu_copy);
}

TEST_F( MOS6502Test1, LDAZeroPageXCanLoadAValueIntoTheARegister ) {
	// given
	cpu.X = 0x05;
	memory[0xFFFC] = CPU::LDA_ZP_X;
	memory[0xFFFD] = 0x42;
	memory[0x47] = 0x67;

	// when
	CPU cpu_copy = cpu;
	s32 CyclesUsed = cpu.Execute(4, memory);

	// then
	EXPECT_EQ(cpu.A, 0x67);
	EXPECT_EQ(CyclesUsed, 4);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLDA(cpu, cpu_copy);
}

TEST_F( MOS6502Test1, LDAZeroPageXCanLoadAValueIntoTheARegisterWhenItWraps ) {
	// given
	cpu.X = 0xFF;
	memory[0xFFFC] = CPU::LDA_ZP_X;
	memory[0xFFFD] = 0x80;
	memory[0x7F] = 0x67;

	// when
	CPU cpu_copy = cpu;
	u32 CyclesUsed = cpu.Execute(4, memory);

	// then
	EXPECT_EQ(cpu.A, 0x67);
	EXPECT_EQ(CyclesUsed, 4);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLDA(cpu, cpu_copy);
}