#include <gtest/gtest.h>
#include <m6502.h>

static void VerifyUnmodifiedFlagFromAddWithCarry(const mos6502::CPU& cpu, const mos6502::CPU& cpu_copy) {
	EXPECT_EQ(cpu.Flag.I, cpu_copy.Flag.I);
	EXPECT_EQ(cpu.Flag.D, cpu_copy.Flag.D);
	EXPECT_EQ(cpu.Flag.B, cpu_copy.Flag.B);
}

class MOS6502AddWithCarryTests : public testing::Test {
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

		void ADCTestImmediate(ADCTestData data) {
			using namespace mos6502;

			// given
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::ADC_IM;
			memory[0xFFFD] = data.operand;
			constexpr s32 EXPECTED_CYCLES = 2;
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

		void ADCTestZeroPage(ADCTestData data) {
			using namespace mos6502;

			// given
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::ADC_ZP;
			memory[0xFFFD] = 0x37;
			memory[0x37] = data.operand;
			constexpr s32 EXPECTED_CYCLES = 3;
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

		void ADCTestZeroPageX(ADCTestData data) {
			using namespace mos6502;

			// given
			cpu.X = 0xB1;
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::ADC_ZP_X;
			memory[0xFFFD] = 0x37;
			memory[0xE8] = data.operand;
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

		void ADCTestIndirectX(ADCTestData data) {
			using namespace mos6502;

			// given
			cpu.X = 0xB1;
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::ADC_IND_X;
			memory[0xFFFD] = 0x37;
			memory[0xE8] = 0x3C;
			memory[0xE9] = 0x76;
			memory[0x763C] = data.operand;
			constexpr s32 EXPECTED_CYCLES = 6;
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

		void ADCTestIndirectY(ADCTestData data) {
			using namespace mos6502;

			// given
			cpu.Y = 0xB1;
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::ADC_IND_Y;
			memory[0xFFFD] = 0x37;
			memory[0x37] = 0x3C;
			memory[0x38] = 0x76;
			memory[0x76ED] = data.operand;
			constexpr s32 EXPECTED_CYCLES = 5;
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

		void ADCTestIndirectYWithPageCrossing(ADCTestData data) {
			using namespace mos6502;

			// given
			cpu.Y = 0xF1;
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::ADC_IND_Y;
			memory[0xFFFD] = 0x37;
			memory[0x37] = 0x3C;
			memory[0x38] = 0x76;
			memory[0x772D] = data.operand;
			constexpr s32 EXPECTED_CYCLES = 6;
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

		void ADCTestAbsolute(ADCTestData data) {
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

		void ADCTestAbsoluteX(ADCTestData data) {
			using namespace mos6502;

			// given
			cpu.X = 0xA1;
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::ADC_ABS_X;
			memory[0xFFFD] = 0x47;
			memory[0xFFFE] = 0x37;
			memory[0x37E8] = data.operand;
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

		void ADCTestAbsoluteXWithPageCrossing(ADCTestData data) {
			using namespace mos6502;

			// given
			cpu.X = 0xC1;
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::ADC_ABS_X;
			memory[0xFFFD] = 0x47;
			memory[0xFFFE] = 0x37;
			memory[0x3808] = data.operand;
			constexpr s32 EXPECTED_CYCLES = 5;
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

		void ADCTestAbsoluteY(ADCTestData data) {
			using namespace mos6502;

			// given
			cpu.Y = 0xA1;
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::ADC_ABS_Y;
			memory[0xFFFD] = 0x47;
			memory[0xFFFE] = 0x37;
			memory[0x37E8] = data.operand;
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

		void ADCTestAbsoluteYWithPageCrossing(ADCTestData data) {
			using namespace mos6502;

			// given
			cpu.Y = 0xC1;
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::ADC_ABS_Y;
			memory[0xFFFD] = 0x47;
			memory[0xFFFE] = 0x37;
			memory[0x3808] = data.operand;
			constexpr s32 EXPECTED_CYCLES = 5;
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

MOS6502AddWithCarryTests::ADCTestData testADCDataAddZeroToZeroWithoutCarry{
	.carry = false,
	.A_value = 0,
	.operand = 0,
	.result = 0x0,
	.expect_C = false,
	.expect_Z = true,
	.expect_N = false,
	.expect_V = false,
};

MOS6502AddWithCarryTests::ADCTestData testADCDataAddZeroToZeroWithCarry{
	.carry = true,
	.A_value = 0,
	.operand = 0,
	.result = 0x1,
	.expect_C = false,
	.expect_Z = false,
	.expect_N = false,
	.expect_V = false,
};

MOS6502AddWithCarryTests::ADCTestData testADCDataAddOneToFFAndCauseCarry{
	.carry = false,
	.A_value = 0xFF,
	.operand = 0x01,
	.result = 0x00,
	.expect_C = true,
	.expect_Z = true,
	.expect_N = false,
	.expect_V = false,
};

MOS6502AddWithCarryTests::ADCTestData testADCDataAddTwoUnsignedNumbers{
	.carry = false,
	.A_value = 0x48,
	.operand = 0x12,
	.result = 0x5A,
	.expect_C = false,
	.expect_Z = false,
	.expect_N = false,
	.expect_V = false,
};

MOS6502AddWithCarryTests::ADCTestData testADCDataAddNegativeAndPositiveNumbers{
	.carry = false,
	.A_value = 0x59,
	.operand = 0xEF,
	.result = 0x48,
	.expect_C = true,
	.expect_Z = false,
	.expect_N = false,
	.expect_V = false,
};

MOS6502AddWithCarryTests::ADCTestData testADCDataSetNegativeFlagWhenResultIsNegative{
	.carry = false,
	.A_value = 0x00,
	.operand = 0xFF,
	.result = 0xFF,
	.expect_C = false,
	.expect_Z = false,
	.expect_N = true,
	.expect_V = false,
};

MOS6502AddWithCarryTests::ADCTestData testADCDataSetOverflowFlagWhenSignedNegativeAdditionFailed {
	.carry = false,
	.A_value = 0x80,
	.operand = 0xFF,
	.result = 0x7F,
	.expect_C = true,
	.expect_Z = false,
	.expect_N = false,
	.expect_V = true,
};

MOS6502AddWithCarryTests::ADCTestData testADCDataSetOverflowFlagWhenSignedPositiveAdditionFailed {
	.carry = false,
	.A_value = 0x7F,
	.operand = 0x01,
	.result = 0x80,
	.expect_C = false,
	.expect_Z = false,
	.expect_N = true,
	.expect_V = true,
};

TEST_F(MOS6502AddWithCarryTests, ADCImmediateCanAddZeroToZeroWithoutCarry) {
	ADCTestImmediate(testADCDataAddZeroToZeroWithoutCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCImmediateCanAddZeroToZeroWithCarry) {
	ADCTestImmediate(testADCDataAddZeroToZeroWithCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCImeddiateCanAddOneToFFAndCauseCarry) {
	ADCTestImmediate(testADCDataAddOneToFFAndCauseCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCImmediateCanAddTwoUnsignedNumbers) {
	ADCTestImmediate(testADCDataAddTwoUnsignedNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCImmediateCanAddNegativeAndPositiveNumbers) {
	ADCTestImmediate(testADCDataAddNegativeAndPositiveNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCImmediateCanSetNegativeFlagWhenResultIsNegative) {
	ADCTestImmediate(testADCDataSetNegativeFlagWhenResultIsNegative);
}

TEST_F(MOS6502AddWithCarryTests, ADCImmediateCanSetOverflowFlagWhenSignedNegativeAdditionFailed) {
	ADCTestImmediate(testADCDataSetOverflowFlagWhenSignedNegativeAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCImmediateCanSetOverflowFlagWhenSignedPositiveAdditionFailed) {
	ADCTestImmediate(testADCDataSetOverflowFlagWhenSignedPositiveAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCZeroPageCanAddZeroToZeroWithoutCarry) {
	ADCTestZeroPage(testADCDataAddZeroToZeroWithoutCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCZeroPageCanAddZeroToZeroWithCarry) {
	ADCTestZeroPage(testADCDataAddZeroToZeroWithCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCZeroPageCanAddOneToFFAndCauseCarry) {
	ADCTestZeroPage(testADCDataAddOneToFFAndCauseCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCZeroPageCanAddTwoUnsignedNumbers) {
	ADCTestZeroPage(testADCDataAddTwoUnsignedNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCZeroPageCanAddNegativeAndPositiveNumbers) {
	ADCTestZeroPage(testADCDataAddNegativeAndPositiveNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCZeroPageCanSetNegativeFlagWhenResultIsNegative) {
	ADCTestZeroPage(testADCDataSetNegativeFlagWhenResultIsNegative);
}

TEST_F(MOS6502AddWithCarryTests, ADCZeroPageCanSetOverflowFlagWhenSignedNegativeAdditionFailed) {
	ADCTestZeroPage(testADCDataSetOverflowFlagWhenSignedNegativeAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCZeroPageCanSetOverflowFlagWhenSignedPositiveAdditionFailed) {
	ADCTestZeroPage(testADCDataSetOverflowFlagWhenSignedPositiveAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCZeroPageXCanAddZeroToZeroWithoutCarry) {
	ADCTestZeroPageX(testADCDataAddZeroToZeroWithoutCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCZeroPageXCanAddZeroToZeroWithCarry) {
	ADCTestZeroPageX(testADCDataAddZeroToZeroWithCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCZeroPageXCanAddOneToFFAndCauseCarry) {
	ADCTestZeroPageX(testADCDataAddOneToFFAndCauseCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCZeroPageXCanAddTwoUnsignedNumbers) {
	ADCTestZeroPageX(testADCDataAddTwoUnsignedNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCZeroPageXCanAddNegativeAndPositiveNumbers) {
	ADCTestZeroPageX(testADCDataAddNegativeAndPositiveNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCZeroPageXCanSetNegativeFlagWhenResultIsNegative) {
	ADCTestZeroPageX(testADCDataSetNegativeFlagWhenResultIsNegative);
}

TEST_F(MOS6502AddWithCarryTests, ADCZeroPageXCanSetOverflowFlagWhenSignedNegativeAdditionFailed) {
	ADCTestZeroPageX(testADCDataSetOverflowFlagWhenSignedNegativeAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCZeroPageXCanSetOverflowFlagWhenSignedPositiveAdditionFailed) {
	ADCTestZeroPageX(testADCDataSetOverflowFlagWhenSignedPositiveAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteCanAddZeroToZeroWithoutCarry) {
	ADCTestAbsolute(testADCDataAddZeroToZeroWithoutCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteCanAddZeroToZeroWithCarry) {
	ADCTestAbsolute(testADCDataAddZeroToZeroWithCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteCanAddOneToFFAndCauseCarry) {
	ADCTestAbsolute(testADCDataAddOneToFFAndCauseCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteCanAddTwoUnsignedNumbers) {
	ADCTestAbsolute(testADCDataAddTwoUnsignedNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteCanAddNegativeAndPositiveNumbers) {
	ADCTestAbsolute(testADCDataAddNegativeAndPositiveNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteCanSetNegativeFlagWhenResultIsNegative) {
	ADCTestAbsolute(testADCDataSetNegativeFlagWhenResultIsNegative);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteCanSetOverflowFlagWhenSignedNegativeAdditionFailed) {
	ADCTestAbsolute(testADCDataSetOverflowFlagWhenSignedNegativeAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteCanSetOverflowFlagWhenSignedPositiveAdditionFailed) {
	ADCTestAbsolute(testADCDataSetOverflowFlagWhenSignedPositiveAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteXCanAddZeroToZeroWithoutCarry) {
	ADCTestAbsoluteX(testADCDataAddZeroToZeroWithoutCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteXCanAddZeroToZeroWithCarry) {
	ADCTestAbsoluteX(testADCDataAddZeroToZeroWithCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteXCanAddOneToFFAndCauseCarry) {
	ADCTestAbsoluteX(testADCDataAddOneToFFAndCauseCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteXCanAddTwoUnsignedNumbers) {
	ADCTestAbsoluteX(testADCDataAddTwoUnsignedNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteXCanAddNegativeAndPositiveNumbers) {
	ADCTestAbsoluteX(testADCDataAddNegativeAndPositiveNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteXCanSetNegativeFlagWhenResultIsNegative) {
	ADCTestAbsoluteX(testADCDataSetNegativeFlagWhenResultIsNegative);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteXCanSetOverflowFlagWhenSignedNegativeAdditionFailed) {
	ADCTestAbsoluteX(testADCDataSetOverflowFlagWhenSignedNegativeAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteXCanSetOverflowFlagWhenSignedPositiveAdditionFailed) {
	ADCTestAbsoluteX(testADCDataSetOverflowFlagWhenSignedPositiveAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteXCanAddZeroToZeroWithoutCarryWithPageCrossing) {
	ADCTestAbsoluteXWithPageCrossing(testADCDataAddZeroToZeroWithoutCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteXCanAddZeroToZeroWithCarryWithPageCrossing) {
	ADCTestAbsoluteXWithPageCrossing(testADCDataAddZeroToZeroWithCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteXCanAddOneToFFAndCauseCarryWithPageCrossing) {
	ADCTestAbsoluteXWithPageCrossing(testADCDataAddOneToFFAndCauseCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteXCanAddTwoUnsignedNumbersWithPageCrossing) {
	ADCTestAbsoluteXWithPageCrossing(testADCDataAddTwoUnsignedNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteXCanAddNegativeAndPositiveNumbersWithPageCrossing) {
	ADCTestAbsoluteXWithPageCrossing(testADCDataAddNegativeAndPositiveNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteXCanSetNegativeFlagWhenResultIsNegativeWithPageCrossing) {
	ADCTestAbsoluteXWithPageCrossing(testADCDataSetNegativeFlagWhenResultIsNegative);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteXCanSetOverflowFlagWhenSignedNegativeAdditionFailedWithPageCrossing) {
	ADCTestAbsoluteXWithPageCrossing(testADCDataSetOverflowFlagWhenSignedNegativeAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteXCanSetOverflowFlagWhenSignedPositiveAdditionFailedWithPageCrossing) {
	ADCTestAbsoluteXWithPageCrossing(testADCDataSetOverflowFlagWhenSignedPositiveAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteYCanAddZeroToZeroWithoutCarry) {
	ADCTestAbsoluteY(testADCDataAddZeroToZeroWithoutCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteYCanAddZeroToZeroWithCarry) {
	ADCTestAbsoluteY(testADCDataAddZeroToZeroWithCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteYCanAddOneToFFAndCauseCarry) {
	ADCTestAbsoluteY(testADCDataAddOneToFFAndCauseCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteYCanAddTwoUnsignedNumbers) {
	ADCTestAbsoluteY(testADCDataAddTwoUnsignedNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteYCanAddNegativeAndPositiveNumbers) {
	ADCTestAbsoluteY(testADCDataAddNegativeAndPositiveNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteYCanSetNegativeFlagWhenResultIsNegative) {
	ADCTestAbsoluteY(testADCDataSetNegativeFlagWhenResultIsNegative);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteYCanSetOverflowFlagWhenSignedNegativeAdditionFailed) {
	ADCTestAbsoluteY(testADCDataSetOverflowFlagWhenSignedNegativeAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteYCanSetOverflowFlagWhenSignedPositiveAdditionFailed) {
	ADCTestAbsoluteY(testADCDataSetOverflowFlagWhenSignedPositiveAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteYCanAddZeroToZeroWithoutCarryWithPageCrossing) {
	ADCTestAbsoluteYWithPageCrossing(testADCDataAddZeroToZeroWithoutCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteYCanAddZeroToZeroWithCarryWithPageCrossing) {
	ADCTestAbsoluteYWithPageCrossing(testADCDataAddZeroToZeroWithCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteYCanAddOneToFFAndCauseCarryWithPageCrossing) {
	ADCTestAbsoluteYWithPageCrossing(testADCDataAddOneToFFAndCauseCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteYCanAddTwoUnsignedNumbersWithPageCrossing) {
	ADCTestAbsoluteYWithPageCrossing(testADCDataAddTwoUnsignedNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteYCanAddNegativeAndPositiveNumbersWithPageCrossing) {
	ADCTestAbsoluteYWithPageCrossing(testADCDataAddNegativeAndPositiveNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteYCanSetNegativeFlagWhenResultIsNegativeWithPageCrossing) {
	ADCTestAbsoluteYWithPageCrossing(testADCDataSetNegativeFlagWhenResultIsNegative);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteYCanSetOverflowFlagWhenSignedNegativeAdditionFailedWithPageCrossing) {
	ADCTestAbsoluteYWithPageCrossing(testADCDataSetOverflowFlagWhenSignedNegativeAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCAbsoluteYCanSetOverflowFlagWhenSignedPositiveAdditionFailedWithPageCrossing) {
	ADCTestAbsoluteYWithPageCrossing(testADCDataSetOverflowFlagWhenSignedPositiveAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectXCanAddZeroToZeroWithoutCarry) {
	ADCTestIndirectX(testADCDataAddZeroToZeroWithoutCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectXCanAddZeroToZeroWithCarry) {
	ADCTestIndirectX(testADCDataAddZeroToZeroWithCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectXCanAddOneToFFAndCauseCarry) {
	ADCTestIndirectX(testADCDataAddOneToFFAndCauseCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectXCanAddTwoUnsignedNumbers) {
	ADCTestIndirectX(testADCDataAddTwoUnsignedNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectXCanAddNegativeAndPositiveNumbers) {
	ADCTestIndirectX(testADCDataAddNegativeAndPositiveNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectXCanSetNegativeFlagWhenResultIsNegative) {
	ADCTestIndirectX(testADCDataSetNegativeFlagWhenResultIsNegative);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectXCanSetOverflowFlagWhenSignedNegativeAdditionFailed) {
	ADCTestIndirectX(testADCDataSetOverflowFlagWhenSignedNegativeAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectXCanSetOverflowFlagWhenSignedPositiveAdditionFailed) {
	ADCTestIndirectX(testADCDataSetOverflowFlagWhenSignedPositiveAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectYCanAddZeroToZeroWithoutCarry) {
	ADCTestIndirectY(testADCDataAddZeroToZeroWithoutCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectYCanAddZeroToZeroWithCarry) {
	ADCTestIndirectY(testADCDataAddZeroToZeroWithCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectYCanAddOneToFFAndCauseCarry) {
	ADCTestIndirectY(testADCDataAddOneToFFAndCauseCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectYCanAddTwoUnsignedNumbers) {
	ADCTestIndirectY(testADCDataAddTwoUnsignedNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectYCanAddNegativeAndPositiveNumbers) {
	ADCTestIndirectY(testADCDataAddNegativeAndPositiveNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectYCanSetNegativeFlagWhenResultIsNegative) {
	ADCTestIndirectY(testADCDataSetNegativeFlagWhenResultIsNegative);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectYCanSetOverflowFlagWhenSignedNegativeAdditionFailed) {
	ADCTestIndirectY(testADCDataSetOverflowFlagWhenSignedNegativeAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectYCanSetOverflowFlagWhenSignedPositiveAdditionFailed) {
	ADCTestIndirectY(testADCDataSetOverflowFlagWhenSignedPositiveAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectYCanAddZeroToZeroWithoutCarryWithPageCrossing) {
	ADCTestIndirectYWithPageCrossing(testADCDataAddZeroToZeroWithoutCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectYCanAddZeroToZeroWithCarryWithPageCrossing) {
	ADCTestIndirectYWithPageCrossing(testADCDataAddZeroToZeroWithCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectYCanAddOneToFFAndCauseCarryWithPageCrossing) {
	ADCTestIndirectYWithPageCrossing(testADCDataAddOneToFFAndCauseCarry);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectYCanAddTwoUnsignedNumbersWithPageCrossing) {
	ADCTestIndirectYWithPageCrossing(testADCDataAddTwoUnsignedNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectYCanAddNegativeAndPositiveNumbersWithPageCrossing) {
	ADCTestIndirectYWithPageCrossing(testADCDataAddNegativeAndPositiveNumbers);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectYCanSetNegativeFlagWhenResultIsNegativeWithPageCrossing) {
	ADCTestIndirectYWithPageCrossing(testADCDataSetNegativeFlagWhenResultIsNegative);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectYCanSetOverflowFlagWhenSignedNegativeAdditionFailedWithPageCrossing) {
	ADCTestIndirectYWithPageCrossing(testADCDataSetOverflowFlagWhenSignedNegativeAdditionFailed);
}

TEST_F(MOS6502AddWithCarryTests, ADCIndirectYCanSetOverflowFlagWhenSignedPositiveAdditionFailedWithPageCrossing) {
	ADCTestIndirectYWithPageCrossing(testADCDataSetOverflowFlagWhenSignedPositiveAdditionFailed);
}