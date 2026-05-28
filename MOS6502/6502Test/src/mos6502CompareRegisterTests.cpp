#include <gtest/gtest.h>
#include <m6502.h>

class MOS6502ComparyRegisterTests : public testing::Test {
	public:
		mos6502::Memory memory;
		mos6502::CPU cpu;
		virtual void SetUp() {
			cpu.Reset(0xFFFC, memory);
		};
		virtual void TearDown() {

		};
};

static void VerifyUnmodifiedFlagFromComparing(const mos6502::CPU& cpu, const mos6502::CPU& cpu_copy) {
	EXPECT_EQ(cpu.Flag.I, cpu_copy.Flag.I);
	EXPECT_EQ(cpu.Flag.D, cpu_copy.Flag.D);
	EXPECT_EQ(cpu.Flag.B, cpu_copy.Flag.B);
	EXPECT_EQ(cpu.Flag.V, cpu_copy.Flag.V);
}


TEST_F(MOS6502ComparyRegisterTests, CMPImmediateCanCompareTwoEqualValues) {
	using namespace mos6502;

	// given
	cpu.Flag.C = false;
	cpu.Flag.Z = false;
	cpu.Flag.N = true;
	cpu.A = 0x1A;
	memory[0xFFFC] = CPU::CMP_IM;
	memory[0xFFFD] = 0x1A;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x1A);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_TRUE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromComparing(cpu, cpu_copy);
}

TEST_F(MOS6502ComparyRegisterTests, CMPImmediateCanCompareTwoNotEqualValues) {
	using namespace mos6502;

	// given
	cpu.Flag.C = false;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.A = 0x30;
	memory[0xFFFC] = CPU::CMP_IM;
	memory[0xFFFD] = 0x1A;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x30);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromComparing(cpu, cpu_copy);
}

TEST_F(MOS6502ComparyRegisterTests, CMPImmediateCanCompareTwoNotEqualValues2) {
	using namespace mos6502;

	// given
	cpu.Flag.C = false;
	cpu.Flag.Z = true;
	cpu.Flag.N = true;
	cpu.A = 0x82;
	memory[0xFFFC] = CPU::CMP_IM;
	memory[0xFFFD] = 0x1A;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x82);
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_FALSE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromComparing(cpu, cpu_copy);
}

TEST_F(MOS6502ComparyRegisterTests, CMPImmediateCanCompareTwoNotEqualValues3) {
	using namespace mos6502;

	// given
	cpu.Flag.C = true;
	cpu.Flag.Z = true;
	cpu.Flag.N = false;
	cpu.A = 0x08;
	memory[0xFFFC] = CPU::CMP_IM;
	memory[0xFFFD] = 0x1A;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x08);
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_FALSE(cpu.Flag.Z);
	EXPECT_TRUE(cpu.Flag.N);
	VerifyUnmodifiedFlagFromComparing(cpu, cpu_copy);
}
