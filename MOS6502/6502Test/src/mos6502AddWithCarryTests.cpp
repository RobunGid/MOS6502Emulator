#include <gtest/gtest.h>
#include <m6502.h>

static void VerifyUnmodifiedFlagFromAddWithCarry(const mos6502::CPU& cpu, const mos6502::CPU& cpu_copy) {
	EXPECT_EQ(cpu.Flag.I, cpu_copy.Flag.I);
	EXPECT_EQ(cpu.Flag.D, cpu_copy.Flag.D);
	EXPECT_EQ(cpu.Flag.B, cpu_copy.Flag.B);
}

class MOS6502StatusFlagChangesTests : public testing::Test {
	public:
		mos6502::Memory memory;
		mos6502::CPU cpu;
		virtual void SetUp() {
			cpu.Reset(0xFFFC, memory);
		};
		virtual void TearDown() {

		};

		struct ADCTestData {
			bool carry;
			mos6502::Byte A_value;
			mos6502::Byte operand;
			mos6502::Byte result;

			bool expect_C;
			bool expect_Z;
			bool expect_N;
			bool expect_V;
		};

		void TestAbsolute(ADCTestData data) {
			using namespace mos6502;

			// given
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::ADC_ABS;
			memory[0xFFFD] = 0x67;
			memory[0xFFFE] = 0x37;
			memory[0x3767] = data.operand;
			constexpr s32 EXPECTED_CYCLES = 4;
			CPU cpu_copy = cpu;

			// when
			s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

			// then
			EXPECT_EQ(cpu.A , data.result);
			EXPECT_EQ(cpu.Flag.C, data.expect_C);
			EXPECT_EQ(cpu.Flag.Z, data.expect_Z);
			EXPECT_EQ(cpu.Flag.N, data.expect_N);
			EXPECT_EQ(cpu.Flag.V, data.expect_V);
			VerifyUnmodifiedFlagFromAddWithCarry(cpu, cpu_copy);
			EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
		};
};

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteCanCanAddZeroToZeroWithoutCarry) {
	ADCTestData test{
		.carry = false,
		.A_value = 0,
		.operand = 0,
		.result = 0x0,
		.expect_C = false,
		.expect_Z = true,
		.expect_N = false,
		.expect_V = false,
	};
	TestAbsolute(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteCanCanAddZeroToZeroWithCarry) {
	ADCTestData test{
		.carry = true,
		.A_value = 0,
		.operand = 0,
		.result = 0x1,
		.expect_C = false,
		.expect_Z = false,
		.expect_N = false,
		.expect_V = false,
	};
	TestAbsolute(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteCanCanAddOneToFFAndCauseCarry) {
	ADCTestData test{
		.carry = false,
		.A_value = 0xFF,
		.operand = 0x01,
		.result = 0x00,
		.expect_C = true,
		.expect_Z = true,
		.expect_N = false,
		.expect_V = false,
	};
	TestAbsolute(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteCanAddTwoUnsignedNumbers) {
	ADCTestData test{
		.carry = false,
		.A_value = 0x48,
		.operand = 0x12,
		.result = 0x5A,
		.expect_C = false,
		.expect_Z = false,
		.expect_N = false,
		.expect_V = false,
	};
	TestAbsolute(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteCanAddNegativeAndPositiveNumbers) {
	ADCTestData test{
		.carry = false,
		.A_value = 0x59,
		.operand = 0xEF,
		.result = 0x48,
		.expect_C = true,
		.expect_Z = false,
		.expect_N = false,
		.expect_V = false,
	};
	TestAbsolute(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteCanSetNegativeFlagWhenResultIsNegative) {
	ADCTestData test{
		.carry = false,
		.A_value = 0x00,
		.operand = 0xFF,
		.result = 0xFF,
		.expect_C = false,
		.expect_Z = false,
		.expect_N = true,
		.expect_V = false,
	};
	TestAbsolute(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteCanSetOverflowFlagWhenSignedNegativeAdditionFailed) {
	ADCTestData test{
		.carry = false,
		.A_value = 0x80,
		.operand = 0xFF,
		.result = 0x7F,
		.expect_C = true,
		.expect_Z = false,
		.expect_N = false,
		.expect_V = true,
	};
	TestAbsolute(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteCanSetOverflowFlagWhenSignedPositiveAdditionFailed) {
	ADCTestData test{
		.carry = false,
		.A_value = 0x7F,
		.operand = 0x01,
		.result = 0x80,
		.expect_C = false,
		.expect_Z = false,
		.expect_N = true,
		.expect_V = true,
	};
	TestAbsolute(test);
}