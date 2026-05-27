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

TEST_F(MOS6502StatusFlagChangesTests, ADCImmediateCanAddZeroToZeroWithoutCarry) {
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
	ADCTestImmediate(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCImmediateCanAddZeroToZeroWithCarry) {
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
	ADCTestImmediate(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCImeddiateCanAddOneToFFAndCauseCarry) {
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
	ADCTestImmediate(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCImmediateCanAddTwoUnsignedNumbers) {
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
	ADCTestImmediate(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCImmediateCanAddNegativeAndPositiveNumbers) {
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
	ADCTestImmediate(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCImmediateCanSetNegativeFlagWhenResultIsNegative) {
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
	ADCTestImmediate(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCImmediateCanSetOverflowFlagWhenSignedNegativeAdditionFailed) {
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
	ADCTestImmediate(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCImmediateCanSetOverflowFlagWhenSignedPositiveAdditionFailed) {
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
	ADCTestImmediate(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCZeroPageCanAddZeroToZeroWithoutCarry) {
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
	ADCTestZeroPage(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCZeroPageCanAddZeroToZeroWithCarry) {
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
	ADCTestZeroPage(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCZeroPageCanAddOneToFFAndCauseCarry) {
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
	ADCTestZeroPage(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCZeroPageCanAddTwoUnsignedNumbers) {
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
	ADCTestZeroPage(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCZeroPageCanAddNegativeAndPositiveNumbers) {
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
	ADCTestZeroPage(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCZeroPageCanSetNegativeFlagWhenResultIsNegative) {
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
	ADCTestZeroPage(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCZeroPageCanSetOverflowFlagWhenSignedNegativeAdditionFailed) {
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
	ADCTestZeroPage(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCZeroPageCanSetOverflowFlagWhenSignedPositiveAdditionFailed) {
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
	ADCTestZeroPage(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCZeroPageXCanAddZeroToZeroWithoutCarry) {
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
	ADCTestZeroPageX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCZeroPageXCanAddZeroToZeroWithCarry) {
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
	ADCTestZeroPageX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCZeroPageXCanAddOneToFFAndCauseCarry) {
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
	ADCTestZeroPageX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCZeroPageXCanAddTwoUnsignedNumbers) {
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
	ADCTestZeroPageX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCZeroPageXCanAddNegativeAndPositiveNumbers) {
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
	ADCTestZeroPageX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCZeroPageXCanSetNegativeFlagWhenResultIsNegative) {
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
	ADCTestZeroPageX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCZeroPageXCanSetOverflowFlagWhenSignedNegativeAdditionFailed) {
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
	ADCTestZeroPageX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCZeroPageXCanSetOverflowFlagWhenSignedPositiveAdditionFailed) {
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
	ADCTestZeroPageX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteCanAddZeroToZeroWithoutCarry) {
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
	ADCTestAbsolute(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteCanAddZeroToZeroWithCarry) {
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
	ADCTestAbsolute(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteCanAddOneToFFAndCauseCarry) {
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
	ADCTestAbsolute(test);
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
	ADCTestAbsolute(test);
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
	ADCTestAbsolute(test);
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
	ADCTestAbsolute(test);
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
	ADCTestAbsolute(test);
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
	ADCTestAbsolute(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteXCanAddZeroToZeroWithoutCarry) {
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
	ADCTestAbsoluteX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteXCanAddZeroToZeroWithCarry) {
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
	ADCTestAbsoluteX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteXCanAddOneToFFAndCauseCarry) {
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
	ADCTestAbsoluteX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteXCanAddTwoUnsignedNumbers) {
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
	ADCTestAbsoluteX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteXCanAddNegativeAndPositiveNumbers) {
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
	ADCTestAbsoluteX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteXCanSetNegativeFlagWhenResultIsNegative) {
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
	ADCTestAbsoluteX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteXCanSetOverflowFlagWhenSignedNegativeAdditionFailed) {
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
	ADCTestAbsoluteX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteXCanSetOverflowFlagWhenSignedPositiveAdditionFailed) {
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
	ADCTestAbsoluteX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteXCanAddZeroToZeroWithoutCarryWithPageCrossing) {
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
	ADCTestAbsoluteXWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteXCanAddZeroToZeroWithCarryWithPageCrossing) {
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
	ADCTestAbsoluteXWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteXCanAddOneToFFAndCauseCarryWithPageCrossing) {
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
	ADCTestAbsoluteXWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteXCanAddTwoUnsignedNumbersWithPageCrossing) {
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
	ADCTestAbsoluteXWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteXCanAddNegativeAndPositiveNumbersWithPageCrossing) {
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
	ADCTestAbsoluteXWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteXCanSetNegativeFlagWhenResultIsNegativeWithPageCrossing) {
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
	ADCTestAbsoluteXWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteXCanSetOverflowFlagWhenSignedNegativeAdditionFailedWithPageCrossing) {
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
	ADCTestAbsoluteXWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteXCanSetOverflowFlagWhenSignedPositiveAdditionFailedWithPageCrossing) {
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
	ADCTestAbsoluteXWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteYCanAddZeroToZeroWithoutCarry) {
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
	ADCTestAbsoluteY(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteYCanAddZeroToZeroWithCarry) {
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
	ADCTestAbsoluteY(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteYCanAddOneToFFAndCauseCarry) {
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
	ADCTestAbsoluteY(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteYCanAddTwoUnsignedNumbers) {
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
	ADCTestAbsoluteY(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteYCanAddNegativeAndPositiveNumbers) {
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
	ADCTestAbsoluteY(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteYCanSetNegativeFlagWhenResultIsNegative) {
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
	ADCTestAbsoluteY(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteYCanSetOverflowFlagWhenSignedNegativeAdditionFailed) {
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
	ADCTestAbsoluteY(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteYCanSetOverflowFlagWhenSignedPositiveAdditionFailed) {
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
	ADCTestAbsoluteY(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteYCanAddZeroToZeroWithoutCarryWithPageCrossing) {
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
	ADCTestAbsoluteYWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteYCanAddZeroToZeroWithCarryWithPageCrossing) {
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
	ADCTestAbsoluteYWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteYCanAddOneToFFAndCauseCarryWithPageCrossing) {
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
	ADCTestAbsoluteYWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteYCanAddTwoUnsignedNumbersWithPageCrossing) {
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
	ADCTestAbsoluteYWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteYCanAddNegativeAndPositiveNumbersWithPageCrossing) {
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
	ADCTestAbsoluteYWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteYCanSetNegativeFlagWhenResultIsNegativeWithPageCrossing) {
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
	ADCTestAbsoluteYWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteYCanSetOverflowFlagWhenSignedNegativeAdditionFailedWithPageCrossing) {
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
	ADCTestAbsoluteYWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCAbsoluteYCanSetOverflowFlagWhenSignedPositiveAdditionFailedWithPageCrossing) {
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
	ADCTestAbsoluteYWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectXCanAddZeroToZeroWithoutCarry) {
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
	ADCTestIndirectX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectXCanAddZeroToZeroWithCarry) {
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
	ADCTestIndirectX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectXCanAddOneToFFAndCauseCarry) {
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
	ADCTestIndirectX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectXCanAddTwoUnsignedNumbers) {
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
	ADCTestIndirectX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectXCanAddNegativeAndPositiveNumbers) {
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
	ADCTestIndirectX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectXCanSetNegativeFlagWhenResultIsNegative) {
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
	ADCTestIndirectX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectXCanSetOverflowFlagWhenSignedNegativeAdditionFailed) {
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
	ADCTestIndirectX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectXCanSetOverflowFlagWhenSignedPositiveAdditionFailed) {
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
	ADCTestIndirectX(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectYCanAddZeroToZeroWithoutCarry) {
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
	ADCTestIndirectY(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectYCanAddZeroToZeroWithCarry) {
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
	ADCTestIndirectY(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectYCanAddOneToFFAndCauseCarry) {
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
	ADCTestIndirectY(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectYCanAddTwoUnsignedNumbers) {
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
	ADCTestIndirectY(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectYCanAddNegativeAndPositiveNumbers) {
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
	ADCTestIndirectY(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectYCanSetNegativeFlagWhenResultIsNegative) {
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
	ADCTestIndirectY(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectYCanSetOverflowFlagWhenSignedNegativeAdditionFailed) {
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
	ADCTestIndirectY(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectYCanSetOverflowFlagWhenSignedPositiveAdditionFailed) {
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
	ADCTestIndirectY(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectYCanAddZeroToZeroWithoutCarryWithPageCrossing) {
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
	ADCTestIndirectYWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectYCanAddZeroToZeroWithCarryWithPageCrossing) {
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
	ADCTestIndirectYWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectYCanAddOneToFFAndCauseCarryWithPageCrossing) {
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
	ADCTestIndirectYWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectYCanAddTwoUnsignedNumbersWithPageCrossing) {
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
	ADCTestIndirectYWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectYCanAddNegativeAndPositiveNumbersWithPageCrossing) {
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
	ADCTestIndirectYWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectYCanSetNegativeFlagWhenResultIsNegativeWithPageCrossing) {
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
	ADCTestIndirectYWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectYCanSetOverflowFlagWhenSignedNegativeAdditionFailedWithPageCrossing) {
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
	ADCTestIndirectYWithPageCrossing(test);
}

TEST_F(MOS6502StatusFlagChangesTests, ADCIndirectYCanSetOverflowFlagWhenSignedPositiveAdditionFailedWithPageCrossing) {
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
	ADCTestIndirectYWithPageCrossing(test);
}