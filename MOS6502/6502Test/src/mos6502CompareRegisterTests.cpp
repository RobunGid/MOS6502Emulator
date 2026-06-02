#include <gtest/gtest.h>
#include <m6502.h>

static void VerifyUnmodifiedFlagFromComparing(const mos6502::CPU& cpu, const mos6502::CPU& cpu_copy) {
	EXPECT_EQ(cpu.Flag.I, cpu_copy.Flag.I);
	EXPECT_EQ(cpu.Flag.D, cpu_copy.Flag.D);
	EXPECT_EQ(cpu.Flag.B, cpu_copy.Flag.B);
	EXPECT_EQ(cpu.Flag.V, cpu_copy.Flag.V);
}

struct CMPTestData {
	mos6502::Byte A_value;
	mos6502::Byte operand;

	bool expect_C;
	bool expect_Z;
	bool expect_N;
};

class MOS6502CompareRegisterTests : public testing::Test {
	public:
		mos6502::Memory memory;
		mos6502::CPU cpu;
		virtual void SetUp() {
			cpu.Reset(0xFFFC, memory);
		};
		virtual void TearDown() {

		};

		void CMPTestImmediate(CMPTestData data) {
			using namespace mos6502;

			// given
			cpu.Flag.C = !data.expect_C;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::CMP_IM;
			memory[0xFFFD] = data.operand;
			constexpr s32 EXPECTED_CYCLES = 2;
			CPU cpu_copy = cpu;

			// when
			s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

			// then
			EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
			EXPECT_EQ(cpu.A, data.A_value);
			EXPECT_EQ(cpu.Flag.C, data.expect_C);
			EXPECT_EQ(cpu.Flag.Z, data.expect_Z);
			EXPECT_EQ(cpu.Flag.N, data.expect_N);
			VerifyUnmodifiedFlagFromComparing(cpu, cpu_copy);
		};

		void CMPTestZeroPage(CMPTestData data) {
			using namespace mos6502;

			// given
			cpu.Flag.C = !data.expect_C;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::CMP_ZP;
			memory[0xFFFD] = 0x74;
			memory[0x74] = data.operand;
			constexpr s32 EXPECTED_CYCLES = 3;
			CPU cpu_copy = cpu;

			// when
			s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

			// then
			EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
			EXPECT_EQ(cpu.A, data.A_value);
			EXPECT_EQ(cpu.Flag.C, data.expect_C);
			EXPECT_EQ(cpu.Flag.Z, data.expect_Z);
			EXPECT_EQ(cpu.Flag.N, data.expect_N);
			VerifyUnmodifiedFlagFromComparing(cpu, cpu_copy);
		};

		void CMPTestZeroPageX(CMPTestData data) {
			using namespace mos6502;

			// given
			cpu.Flag.C = !data.expect_C;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.A = data.A_value;
			cpu.X = 0x3F;
			memory[0xFFFC] = CPU::CMP_ZP_X;
			memory[0xFFFD] = 0x74;
			memory[0xB3] = data.operand;
			constexpr s32 EXPECTED_CYCLES = 4;
			CPU cpu_copy = cpu;

			// when
			s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

			// then
			EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
			EXPECT_EQ(cpu.A, data.A_value);
			EXPECT_EQ(cpu.Flag.C, data.expect_C);
			EXPECT_EQ(cpu.Flag.Z, data.expect_Z);
			EXPECT_EQ(cpu.Flag.N, data.expect_N);
			VerifyUnmodifiedFlagFromComparing(cpu, cpu_copy);
		};

		void CMPTestAbsolute(CMPTestData data) {
			using namespace mos6502;

			// given
			cpu.Flag.C = !data.expect_C;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::CMP_ABS;
			memory[0xFFFD] = 0xBC;
			memory[0xFFFE] = 0xA5;
			memory[0xA5BC] = data.operand;
			constexpr s32 EXPECTED_CYCLES = 4;
			CPU cpu_copy = cpu;

			// when
			s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

			// then
			EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
			EXPECT_EQ(cpu.A, data.A_value);
			EXPECT_EQ(cpu.Flag.C, data.expect_C);
			EXPECT_EQ(cpu.Flag.Z, data.expect_Z);
			EXPECT_EQ(cpu.Flag.N, data.expect_N);
			VerifyUnmodifiedFlagFromComparing(cpu, cpu_copy);
		};

		void CMPTestAbsoluteX(CMPTestData data) {
			using namespace mos6502;

			// given
			cpu.Flag.C = !data.expect_C;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.A = data.A_value;
			cpu.X = 0x41;
			memory[0xFFFC] = CPU::CMP_ABS_X;
			memory[0xFFFD] = 0x9B;
			memory[0xFFFE] = 0xA2;
			memory[0xA2DC] = data.operand;
			constexpr s32 EXPECTED_CYCLES = 4;
			CPU cpu_copy = cpu;

			// when
			s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

			// then
			EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
			EXPECT_EQ(cpu.A, data.A_value);
			EXPECT_EQ(cpu.Flag.C, data.expect_C);
			EXPECT_EQ(cpu.Flag.Z, data.expect_Z);
			EXPECT_EQ(cpu.Flag.N, data.expect_N);
			VerifyUnmodifiedFlagFromComparing(cpu, cpu_copy);
		};

		void CMPTestAbsoluteXWithPageCrossing(CMPTestData data) {
			using namespace mos6502;

			// given
			cpu.Flag.C = !data.expect_C;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.A = data.A_value;
			cpu.X = 0xB1;
			memory[0xFFFC] = CPU::CMP_ABS_X;
			memory[0xFFFD] = 0x9B;
			memory[0xFFFE] = 0xA2;
			memory[0xA34C] = data.operand;
			constexpr s32 EXPECTED_CYCLES = 5;
			CPU cpu_copy = cpu;

			// when
			s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

			// then
			EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
			EXPECT_EQ(cpu.A, data.A_value);
			EXPECT_EQ(cpu.Flag.C, data.expect_C);
			EXPECT_EQ(cpu.Flag.Z, data.expect_Z);
			EXPECT_EQ(cpu.Flag.N, data.expect_N);
			VerifyUnmodifiedFlagFromComparing(cpu, cpu_copy);
		};

		void CMPTestAbsoluteY(CMPTestData data) {
			using namespace mos6502;

			// given
			cpu.Flag.C = !data.expect_C;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.A = data.A_value;
			cpu.Y = 0x41;
			memory[0xFFFC] = CPU::CMP_ABS_Y;
			memory[0xFFFD] = 0x9B;
			memory[0xFFFE] = 0xA2;
			memory[0xA2DC] = data.operand;
			constexpr s32 EXPECTED_CYCLES = 4;
			CPU cpu_copy = cpu;

			// when
			s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

			// then
			EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
			EXPECT_EQ(cpu.A, data.A_value);
			EXPECT_EQ(cpu.Flag.C, data.expect_C);
			EXPECT_EQ(cpu.Flag.Z, data.expect_Z);
			EXPECT_EQ(cpu.Flag.N, data.expect_N);
			VerifyUnmodifiedFlagFromComparing(cpu, cpu_copy);
		};

		void CMPTestAbsoluteYWithPageCrossing(CMPTestData data) {
			using namespace mos6502;

			// given
			cpu.Flag.C = !data.expect_C;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.A = data.A_value;
			cpu.Y = 0xB1;
			memory[0xFFFC] = CPU::CMP_ABS_Y;
			memory[0xFFFD] = 0x9B;
			memory[0xFFFE] = 0xA2;
			memory[0xA34C] = data.operand;
			constexpr s32 EXPECTED_CYCLES = 5;
			CPU cpu_copy = cpu;

			// when
			s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

			// then
			EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
			EXPECT_EQ(cpu.A, data.A_value);
			EXPECT_EQ(cpu.Flag.C, data.expect_C);
			EXPECT_EQ(cpu.Flag.Z, data.expect_Z);
			EXPECT_EQ(cpu.Flag.N, data.expect_N);
			VerifyUnmodifiedFlagFromComparing(cpu, cpu_copy);
		};

		void CMPTestIndirectX(CMPTestData data) {
			using namespace mos6502;

			// given
			cpu.Flag.C = !data.expect_C;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.A = data.A_value;
			cpu.X = 0x23;
			memory[0xFFFC] = CPU::CMP_IND_X;
			memory[0xFFFD] = 0x9B;
			memory[0xBE] = 0xD8;
			memory[0xBF] = 0x45;
			memory[0x45D8] = data.operand;
			constexpr s32 EXPECTED_CYCLES = 6;
			CPU cpu_copy = cpu;

			// when
			s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

			// then
			EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
			EXPECT_EQ(cpu.A, data.A_value);
			EXPECT_EQ(cpu.Flag.C, data.expect_C);
			EXPECT_EQ(cpu.Flag.Z, data.expect_Z);
			EXPECT_EQ(cpu.Flag.N, data.expect_N);
			VerifyUnmodifiedFlagFromComparing(cpu, cpu_copy);
		};

		void CMPTestIndirectY(CMPTestData data) {
			using namespace mos6502;

			// given
			cpu.Flag.C = !data.expect_C;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.A = data.A_value;
			cpu.Y = 0x23;
			memory[0xFFFC] = CPU::CMP_IND_Y;
			memory[0xFFFD] = 0x9B;
			memory[0x9B] = 0xD8;
			memory[0x9C] = 0x45;
			memory[0x45FB] = data.operand;
			constexpr s32 EXPECTED_CYCLES = 5;
			CPU cpu_copy = cpu;

			// when
			s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

			// then
			EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
			EXPECT_EQ(cpu.A, data.A_value);
			EXPECT_EQ(cpu.Flag.C, data.expect_C);
			EXPECT_EQ(cpu.Flag.Z, data.expect_Z);
			EXPECT_EQ(cpu.Flag.N, data.expect_N);
			VerifyUnmodifiedFlagFromComparing(cpu, cpu_copy);
		};

		void CMPTestIndirectYWithPageCrossing(CMPTestData data) {
			using namespace mos6502;

			// given
			cpu.Flag.C = !data.expect_C;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.A = data.A_value;
			cpu.Y = 0xA3;
			memory[0xFFFC] = CPU::CMP_IND_Y;
			memory[0xFFFD] = 0x9B;
			memory[0x9B] = 0xD8;
			memory[0x9C] = 0x45;
			memory[0x467B] = data.operand;
			constexpr s32 EXPECTED_CYCLES = 6;
			CPU cpu_copy = cpu;

			// when
			s32 cyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

			// then
			EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
			EXPECT_EQ(cpu.A, data.A_value);
			EXPECT_EQ(cpu.Flag.C, data.expect_C);
			EXPECT_EQ(cpu.Flag.Z, data.expect_Z);
			EXPECT_EQ(cpu.Flag.N, data.expect_N);
			VerifyUnmodifiedFlagFromComparing(cpu, cpu_copy);
		};
};

CMPTestData testEqualData{
	.A_value = 0x1A,
	.operand = 0x1A,
	.expect_C = true,
	.expect_Z = true,
	.expect_N = false,
};

CMPTestData testGreaterNoSign{
	.A_value = 0x30,
	.operand = 0x1A,
	.expect_C = true,
	.expect_Z = false,
	.expect_N = false,
};

CMPTestData testGreaterWithHighBit{
	.A_value = 0x82,
	.operand = 0x1A,
	.expect_C = true,
	.expect_Z = false,
	.expect_N = false,
};

CMPTestData testLessNegativeResult{
	.A_value = 0x08,
	.operand = 0x1A,
	.expect_C = false,
	.expect_Z = false,
	.expect_N = true,
};

// CMP Immediate

TEST_F(MOS6502CompareRegisterTests, CMPImmediateCanCompareTwoEqualValues) {
	CMPTestImmediate(testEqualData);
}

TEST_F(MOS6502CompareRegisterTests, CMPImmediateCanCompareGreaterNoSign) {
	CMPTestImmediate(testGreaterNoSign);
}

TEST_F(MOS6502CompareRegisterTests, CMPImmediateCanCompareGreaterWithHighBit) {
	CMPTestImmediate(testGreaterWithHighBit);
}

TEST_F(MOS6502CompareRegisterTests, CMPImmediateCanCompareLessNegativeResult) {
	CMPTestImmediate(testLessNegativeResult);
}

// CMP Zero Page

TEST_F(MOS6502CompareRegisterTests, CMPZeroPageCanCompareTwoEqualValues) {
	CMPTestZeroPage(testEqualData);
}

TEST_F(MOS6502CompareRegisterTests, CMPZeroPageCanCompareGreaterNoSign) {
	CMPTestZeroPage(testGreaterNoSign);
}

TEST_F(MOS6502CompareRegisterTests, CMPZeroPageCanCompareGreaterWithHighBit) {
	CMPTestZeroPage(testGreaterWithHighBit);
}

TEST_F(MOS6502CompareRegisterTests, CMPZeroPageCanCompareLessNegativeResult) {
	CMPTestZeroPage(testLessNegativeResult);
}

// CMP Zero Page X

TEST_F(MOS6502CompareRegisterTests, CMPZeroPageXCanCompareTwoEqualValues) {
	CMPTestZeroPageX(testEqualData);
}

TEST_F(MOS6502CompareRegisterTests, CMPZeroPageXCanCompareGreaterNoSign) {
	CMPTestZeroPageX(testGreaterNoSign);
}

TEST_F(MOS6502CompareRegisterTests, CMPZeroPageXCanCompareGreaterWithHighBit) {
	CMPTestZeroPageX(testGreaterWithHighBit);
}

TEST_F(MOS6502CompareRegisterTests, CMPZeroPageXCanCompareLessNegativeResult) {
	CMPTestZeroPageX(testLessNegativeResult);
}

// CMP Absolute

TEST_F(MOS6502CompareRegisterTests, CMPAbsoluteCanCompareTwoEqualValues) {
	CMPTestAbsolute(testEqualData);
}

TEST_F(MOS6502CompareRegisterTests, CMPAbsoluteCanCompareGreaterNoSign) {
	CMPTestAbsolute(testGreaterNoSign);
}

TEST_F(MOS6502CompareRegisterTests, CMPAbsoluteCanCompareGreaterWithHighBit) {
	CMPTestAbsolute(testGreaterWithHighBit);
}

TEST_F(MOS6502CompareRegisterTests, CMPAbsoluteCanCompareLessNegativeResult) {
	CMPTestAbsolute(testLessNegativeResult);
}

// CMP Absolute X

TEST_F(MOS6502CompareRegisterTests, CMPAbsoluteXCanCompareTwoEqualValues) {
	CMPTestAbsoluteX(testEqualData);
}

TEST_F(MOS6502CompareRegisterTests, CMPAbsoluteXCanCompareGreaterNoSign) {
	CMPTestAbsoluteX(testGreaterNoSign);
}

TEST_F(MOS6502CompareRegisterTests, CMPAbsoluteXCanCompareGreaterWithHighBit) {
	CMPTestAbsoluteX(testGreaterWithHighBit);
}

TEST_F(MOS6502CompareRegisterTests, CMPAbsoluteXCanCompareLessNegativeResult) {
	CMPTestAbsoluteX(testLessNegativeResult);
}

TEST_F(MOS6502CompareRegisterTests, CMPAbsoluteXCanCompareTwoEqualValuesWithPageCrossing) {
	CMPTestAbsoluteXWithPageCrossing(testEqualData);
}

TEST_F(MOS6502CompareRegisterTests, CMPAbsoluteXCanCompareGreaterNoSignWithPageCrossing) {
	CMPTestAbsoluteXWithPageCrossing(testGreaterNoSign);
}

TEST_F(MOS6502CompareRegisterTests, CMPAbsoluteXCanCompareGreaterWithHighBitWithPageCrossing) {
	CMPTestAbsoluteXWithPageCrossing(testGreaterWithHighBit);
}

TEST_F(MOS6502CompareRegisterTests, CMPAbsoluteXCanCompareLessNegativeResultWithPageCrossing) {
	CMPTestAbsoluteXWithPageCrossing(testLessNegativeResult);
}

// CMP Absolute Y

TEST_F(MOS6502CompareRegisterTests, CMPAbsoluteYCanCompareTwoEqualValues) {
	CMPTestAbsoluteY(testEqualData);
}

TEST_F(MOS6502CompareRegisterTests, CMPAbsoluteYCanCompareGreaterNoSign) {
	CMPTestAbsoluteY(testGreaterNoSign);
}

TEST_F(MOS6502CompareRegisterTests, CMPAbsoluteYCanCompareGreaterWithHighBit) {
	CMPTestAbsoluteY(testGreaterWithHighBit);
}

TEST_F(MOS6502CompareRegisterTests, CMPAbsoluteYCanCompareLessNegativeResult) {
	CMPTestAbsoluteY(testLessNegativeResult);
}

TEST_F(MOS6502CompareRegisterTests, CMPAbsoluteYCanCompareTwoEqualValuesWithPageCrossing) {
	CMPTestAbsoluteYWithPageCrossing(testEqualData);
}

TEST_F(MOS6502CompareRegisterTests, CMPAbsoluteYCanCompareGreaterNoSignWithPageCrossing) {
	CMPTestAbsoluteYWithPageCrossing(testGreaterNoSign);
}

TEST_F(MOS6502CompareRegisterTests, CMPAbsoluteYCanCompareGreaterWithHighBitWithPageCrossing) {
	CMPTestAbsoluteYWithPageCrossing(testGreaterWithHighBit);
}

TEST_F(MOS6502CompareRegisterTests, CMPAbsoluteYCanCompareLessNegativeResultWithPageCrossing) {
	CMPTestAbsoluteYWithPageCrossing(testLessNegativeResult);
}

// CMP Indirect X

TEST_F(MOS6502CompareRegisterTests, CMPIndirectXCanCompareTwoEqualValues) {
	CMPTestIndirectX(testEqualData);
}

TEST_F(MOS6502CompareRegisterTests, CMPIndirectXCanCompareGreaterNoSign) {
	CMPTestIndirectX(testGreaterNoSign);
}

TEST_F(MOS6502CompareRegisterTests, CMPIndirectXCanCompareGreaterWithHighBit) {
	CMPTestIndirectX(testGreaterWithHighBit);
}

TEST_F(MOS6502CompareRegisterTests, CMPIndirectXCanCompareLessNegativeResult) {
	CMPTestIndirectX(testLessNegativeResult);
}

// CMP Indirect Y

TEST_F(MOS6502CompareRegisterTests, CMPIndirectYCanCompareTwoEqualValues) {
	CMPTestIndirectY(testEqualData);
}

TEST_F(MOS6502CompareRegisterTests, CMPIndirectYCanCompareGreaterNoSign) {
	CMPTestIndirectY(testGreaterNoSign);
}

TEST_F(MOS6502CompareRegisterTests, CMPIndirectYCanCompareGreaterWithHighBit) {
	CMPTestIndirectY(testGreaterWithHighBit);
}

TEST_F(MOS6502CompareRegisterTests, CMPIndirectYCanCompareLessNegativeResult) {
	CMPTestIndirectY(testLessNegativeResult);
}

TEST_F(MOS6502CompareRegisterTests, CMPIndirectYCanCompareTwoEqualValuesWithPageCrossing) {
	CMPTestIndirectYWithPageCrossing(testEqualData);
}

TEST_F(MOS6502CompareRegisterTests, CMPIndirectYCanCompareGreaterNoSignWithPageCrossing) {
	CMPTestIndirectYWithPageCrossing(testGreaterNoSign);
}

TEST_F(MOS6502CompareRegisterTests, CMPIndirectYCanCompareGreaterWithHighBitWithPageCrossing) {
	CMPTestIndirectYWithPageCrossing(testGreaterWithHighBit);
}

TEST_F(MOS6502CompareRegisterTests, CMPIndirectYCanCompareLessNegativeResultWithPageCrossing) {
	CMPTestIndirectYWithPageCrossing(testLessNegativeResult);
}