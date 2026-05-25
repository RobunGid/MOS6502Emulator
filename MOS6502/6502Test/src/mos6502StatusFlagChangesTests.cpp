#include <gtest/gtest.h>
#include <m6502.h>

class MOS6502StatusFlagChangesTests : public testing::Test {
	public:
		mos6502::Memory memory;
		mos6502::CPU cpu;
		virtual void SetUp() {
			cpu.Reset(0xFFFC, memory);
		}
		virtual void TearDown() {

		}

};

TEST_F(MOS6502StatusFlagChangesTests, CLCCanClearCarryFlag) {
	using namespace mos6502;

	// given
	cpu.Flag.C = true;
	memory[0xFFFC] = CPU::CLC;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_FALSE(cpu.Flag.C);
	EXPECT_EQ(cpu.Flag.Z, cpu_copy.Flag.Z);
	EXPECT_EQ(cpu.Flag.I, cpu_copy.Flag.I);
	EXPECT_EQ(cpu.Flag.D, cpu_copy.Flag.D);
	EXPECT_EQ(cpu.Flag.B, cpu_copy.Flag.B);
	EXPECT_EQ(cpu.Flag.V, cpu_copy.Flag.V);
	EXPECT_EQ(cpu.Flag.N, cpu_copy.Flag.N);
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
}

TEST_F(MOS6502StatusFlagChangesTests, SECCanSetCarryFlag) {
	using namespace mos6502;

	// given
	cpu.Flag.C = false;
	memory[0xFFFC] = CPU::SEC;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_TRUE(cpu.Flag.C);
	EXPECT_EQ(cpu.Flag.Z, cpu_copy.Flag.Z);
	EXPECT_EQ(cpu.Flag.I, cpu_copy.Flag.I);
	EXPECT_EQ(cpu.Flag.D, cpu_copy.Flag.D);
	EXPECT_EQ(cpu.Flag.B, cpu_copy.Flag.B);
	EXPECT_EQ(cpu.Flag.V, cpu_copy.Flag.V);
	EXPECT_EQ(cpu.Flag.N, cpu_copy.Flag.N);
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
}

TEST_F(MOS6502StatusFlagChangesTests, CLВCanClearDecimalFlag) {
	using namespace mos6502;

	// given
	cpu.Flag.D = true;
	memory[0xFFFC] = CPU::CLD;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_FALSE(cpu.Flag.D);
	EXPECT_EQ(cpu.Flag.Z, cpu_copy.Flag.Z);
	EXPECT_EQ(cpu.Flag.I, cpu_copy.Flag.I);
	EXPECT_EQ(cpu.Flag.C, cpu_copy.Flag.C);
	EXPECT_EQ(cpu.Flag.B, cpu_copy.Flag.B);
	EXPECT_EQ(cpu.Flag.V, cpu_copy.Flag.V);
	EXPECT_EQ(cpu.Flag.N, cpu_copy.Flag.N);
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
}

TEST_F(MOS6502StatusFlagChangesTests, SEDCanSetDecimalFlag) {
	using namespace mos6502;

	// given
	cpu.Flag.D = false;
	memory[0xFFFC] = CPU::SED;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_TRUE(cpu.Flag.D);
	EXPECT_EQ(cpu.Flag.Z, cpu_copy.Flag.Z);
	EXPECT_EQ(cpu.Flag.I, cpu_copy.Flag.I);
	EXPECT_EQ(cpu.Flag.C, cpu_copy.Flag.C);
	EXPECT_EQ(cpu.Flag.B, cpu_copy.Flag.B);
	EXPECT_EQ(cpu.Flag.V, cpu_copy.Flag.V);
	EXPECT_EQ(cpu.Flag.N, cpu_copy.Flag.N);
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
}

TEST_F(MOS6502StatusFlagChangesTests, CLICanClearInterruptDisableFlag) {
	using namespace mos6502;

	// given
	cpu.Flag.I = true;
	memory[0xFFFC] = CPU::CLI;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_FALSE(cpu.Flag.I);
	EXPECT_EQ(cpu.Flag.Z, cpu_copy.Flag.Z);
	EXPECT_EQ(cpu.Flag.D, cpu_copy.Flag.D);
	EXPECT_EQ(cpu.Flag.C, cpu_copy.Flag.C);
	EXPECT_EQ(cpu.Flag.B, cpu_copy.Flag.B);
	EXPECT_EQ(cpu.Flag.V, cpu_copy.Flag.V);
	EXPECT_EQ(cpu.Flag.N, cpu_copy.Flag.N);
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
}

TEST_F(MOS6502StatusFlagChangesTests, SEICanSetInterruptDisableFlag) {
	using namespace mos6502;

	// given
	cpu.Flag.I = false;
	memory[0xFFFC] = CPU::SEI;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_TRUE(cpu.Flag.I);
	EXPECT_EQ(cpu.Flag.Z, cpu_copy.Flag.Z);
	EXPECT_EQ(cpu.Flag.D, cpu_copy.Flag.D);
	EXPECT_EQ(cpu.Flag.C, cpu_copy.Flag.C);
	EXPECT_EQ(cpu.Flag.B, cpu_copy.Flag.B);
	EXPECT_EQ(cpu.Flag.V, cpu_copy.Flag.V);
	EXPECT_EQ(cpu.Flag.N, cpu_copy.Flag.N);
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
}

TEST_F(MOS6502StatusFlagChangesTests, CLVCanClearOverflowFlag) {
	using namespace mos6502;

	// given
	cpu.Flag.V = true;
	memory[0xFFFC] = CPU::CLV;
	constexpr s32 EXPECTED_CYCLES = 2;
	CPU cpu_copy = cpu;

	// when
	s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_FALSE(cpu.Flag.V);
	EXPECT_EQ(cpu.Flag.Z, cpu_copy.Flag.Z);
	EXPECT_EQ(cpu.Flag.D, cpu_copy.Flag.D);
	EXPECT_EQ(cpu.Flag.C, cpu_copy.Flag.C);
	EXPECT_EQ(cpu.Flag.B, cpu_copy.Flag.B);
	EXPECT_EQ(cpu.Flag.I, cpu_copy.Flag.I);
	EXPECT_EQ(cpu.Flag.N, cpu_copy.Flag.N);
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
}