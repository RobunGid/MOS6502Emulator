#include <gtest/gtest.h>
#include <m6502.h>

static void VerifyUnmodifiedFlagFromAddWithCarry(const mos6502::CPU& cpu, const mos6502::CPU& cpu_copy) {
	EXPECT_EQ(cpu.Flag.I, cpu_copy.Flag.I);
	EXPECT_EQ(cpu.Flag.D, cpu_copy.Flag.D);
	EXPECT_EQ(cpu.Flag.B, cpu_copy.Flag.B);
}

class MOS6502SubtractWithCarryTests : public testing::Test {
	public:
		mos6502::Memory memory;
		mos6502::CPU cpu;
		virtual void SetUp() {
			cpu.Reset(0xFFFC, memory);
		};
		virtual void TearDown() {

		};

		struct SBCTestData {
			bool carry;
			mos6502::Byte A_value;
			mos6502::Byte operand;
			mos6502::Byte result;

			bool expect_C;
			bool expect_Z;
			bool expect_N;
			bool expect_V;
		};

		void TestSBCImmediate(SBCTestData data) {
			using namespace mos6502;

			// given
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::SBC_IM;
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

		void TestSBCZeroPage(SBCTestData data) {
			using namespace mos6502;

			// given
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::SBC_ZP;
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

		void TestSBCZeroPageX(SBCTestData data) {
			using namespace mos6502;

			// given
			cpu.X = 0xB1;
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::SBC_ZP_X;
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

		void TestSBCIndirectX(SBCTestData data) {
			using namespace mos6502;

			// given
			cpu.X = 0xB1;
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::SBC_IND_X;
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

		void TestSBCIndirectY(SBCTestData data) {
			using namespace mos6502;

			// given
			cpu.Y = 0xB1;
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::SBC_IND_Y;
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

		void TestSBCIndirectYWithPageCrossing(SBCTestData data) {
			using namespace mos6502;

			// given
			cpu.Y = 0xF1;
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::SBC_IND_Y;
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

		void TestSBCAbsolute(SBCTestData data) {
			using namespace mos6502;

			// given
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::SBC_ABS;
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

		void TestSBCAbsoluteX(SBCTestData data) {
			using namespace mos6502;

			// given
			cpu.X = 0xA1;
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::SBC_ABS_X;
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

		void TestSBCAbsoluteXWithPageCrossing(SBCTestData data) {
			using namespace mos6502;

			// given
			cpu.X = 0xC1;
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::SBC_ABS_X;
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

		void TestSBCAbsoluteY(SBCTestData data) {
			using namespace mos6502;

			// given
			cpu.Y = 0xA1;
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::SBC_ABS_Y;
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

		void TestSBCAbsoluteYWithPageCrossing(SBCTestData data) {
			using namespace mos6502;

			// given
			cpu.Y = 0xC1;
			cpu.Flag.C = data.carry;
			cpu.Flag.Z = !data.expect_Z;
			cpu.Flag.N = !data.expect_N;
			cpu.Flag.V = !data.expect_V;
			cpu.A = data.A_value;
			memory[0xFFFC] = CPU::SBC_ABS_Y;
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

MOS6502SubtractWithCarryTests::SBCTestData testSBCDataSubtractZeroFromZero{
	.carry = true,
	.A_value = 0,
	.operand = 0,
	.result = 0x0,
	.expect_C = true,
	.expect_Z = true,
	.expect_N = false,
	.expect_V = false,
};

MOS6502SubtractWithCarryTests::SBCTestData testSBCSubtractOneFromZero{
	.carry = true,
	.A_value = 0x00,
	.operand = 0x01,
	.result = 0xFF,
	.expect_C = false,
	.expect_Z = false,
	.expect_N = true,
	.expect_V = false,
};

MOS6502SubtractWithCarryTests::SBCTestData testSBCDataSubtractOneFromZeroWithCarry{
	.carry = false,
	.A_value = 0x00,
	.operand = 0x01,
	.result = 0xFE,
	.expect_C = false,
	.expect_Z = false,
	.expect_N = true,
	.expect_V = false,
};

MOS6502SubtractWithCarryTests::SBCTestData testSBCDataSubtractWithOverflow{
	.carry = true,
	.A_value = 0x80,
	.operand = 0x01,
	.result = 0x7F,
	.expect_C = true,
	.expect_Z = false,
	.expect_N = false,
	.expect_V = true,
};

MOS6502SubtractWithCarryTests::SBCTestData testSBCDataSubtractPositiveAndNegativeNumbers{
	.carry = true,
	.A_value = 0x7F,
	.operand = 0xFF,
	.result = 0x80,
	.expect_C = false,
	.expect_Z = false,
	.expect_N = true,
	.expect_V = true,
};

MOS6502SubtractWithCarryTests::SBCTestData testSBCDataCanSubtractZeroFromZeroAndCarry{
	.carry = false,
	.A_value = 0x00,
	.operand = 0x00,
	.result = 0xFF,
	.expect_C = false,
	.expect_Z = false,
	.expect_N = true,
	.expect_V = false,
};

MOS6502SubtractWithCarryTests::SBCTestData testSBCDataCanSubtractTwoUnsignedNumbers {
	.carry = true,
	.A_value = 0x14,
	.operand = 0x03,
	.result = 0x11,
	.expect_C = true,
	.expect_Z = false,
	.expect_N = false,
	.expect_V = false,
};

MOS6502SubtractWithCarryTests::SBCTestData testSBCSubtractTwoNegativeNumbers {
	.carry = true,
	.A_value = 0xEC,
	.operand = 0xF2,
	.result = 0xFA,
	.expect_C = false,
	.expect_Z = false,
	.expect_N = true,
	.expect_V = false,
};

TEST_F(MOS6502SubtractWithCarryTests, SBCImmediateCanSubtractZeroFromZero) {
	TestSBCImmediate(testSBCDataSubtractZeroFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCImmediateCanSubtractOneFromZero) {
	TestSBCImmediate(testSBCSubtractOneFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCImeddiateCanSubtractOneFromZeroWithCarry) {
	TestSBCImmediate(testSBCDataSubtractOneFromZeroWithCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCImmediateCanSubtractWithOverflow) {
	TestSBCImmediate(testSBCDataSubtractWithOverflow);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCImmediateCanSubtractPositiveAndNegativeNumbers) {
	TestSBCImmediate(testSBCDataSubtractPositiveAndNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCImmediateaCanSubtractZeroFromZeroAndCarry) {
	TestSBCImmediate(testSBCDataCanSubtractZeroFromZeroAndCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCImmediateCanSubtractTwoUnsignedNumbers) {
	TestSBCImmediate(testSBCDataCanSubtractTwoUnsignedNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCImmediateCanSetOverflowFlagWhenSignedPositiveSubtractionFailed) {
	TestSBCImmediate(testSBCSubtractTwoNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCZeroPageCanSubtractZeroFromZero) {
	TestSBCZeroPage(testSBCDataSubtractZeroFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCZeroPageCanSubtractOneFromZero) {
	TestSBCZeroPage(testSBCSubtractOneFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCZeroPageCanSubtractOneFromZeroWithCarry) {
	TestSBCZeroPage(testSBCDataSubtractOneFromZeroWithCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCZeroPageCanSubtractWithOverflow) {
	TestSBCZeroPage(testSBCDataSubtractWithOverflow);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCZeroPageCanSubtractPositiveAndNegativeNumbers) {
	TestSBCZeroPage(testSBCDataSubtractPositiveAndNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCZeroPageaCanSubtractZeroFromZeroAndCarry) {
	TestSBCZeroPage(testSBCDataCanSubtractZeroFromZeroAndCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCZeroPageCanSubtractTwoUnsignedNumbers) {
	TestSBCZeroPage(testSBCDataCanSubtractTwoUnsignedNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCZeroPageCanSetOverflowFlagWhenSignedPositiveSubtractionFailed) {
	TestSBCZeroPage(testSBCSubtractTwoNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCZeroPageXCanSubtractZeroFromZero) {
	TestSBCZeroPageX(testSBCDataSubtractZeroFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCZeroPageXCanSubtractOneFromZero) {
	TestSBCZeroPageX(testSBCSubtractOneFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCZeroPageXCanSubtractOneFromZeroWithCarry) {
	TestSBCZeroPageX(testSBCDataSubtractOneFromZeroWithCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCZeroPageXCanSubtractWithOverflow) {
	TestSBCZeroPageX(testSBCDataSubtractWithOverflow);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCZeroPageXCanSubtractPositiveAndNegativeNumbers) {
	TestSBCZeroPageX(testSBCDataSubtractPositiveAndNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCZeroPageXaCanSubtractZeroFromZeroAndCarry) {
	TestSBCZeroPageX(testSBCDataCanSubtractZeroFromZeroAndCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCZeroPageXCanSubtractTwoUnsignedNumbers) {
	TestSBCZeroPageX(testSBCDataCanSubtractTwoUnsignedNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCZeroPageXCanSetOverflowFlagWhenSignedPositiveSubtractionFailed) {
	TestSBCZeroPageX(testSBCSubtractTwoNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteCanSubtractZeroFromZero) {
	TestSBCAbsolute(testSBCDataSubtractZeroFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteCanSubtractOneFromZero) {
	TestSBCAbsolute(testSBCSubtractOneFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteCanSubtractOneFromZeroWithCarry) {
	TestSBCAbsolute(testSBCDataSubtractOneFromZeroWithCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteCanSubtractWithOverflow) {
	TestSBCAbsolute(testSBCDataSubtractWithOverflow);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteCanSubtractPositiveAndNegativeNumbers) {
	TestSBCAbsolute(testSBCDataSubtractPositiveAndNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteaCanSubtractZeroFromZeroAndCarry) {
	TestSBCAbsolute(testSBCDataCanSubtractZeroFromZeroAndCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteCanSubtractTwoUnsignedNumbers) {
	TestSBCAbsolute(testSBCDataCanSubtractTwoUnsignedNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteCanSetOverflowFlagWhenSignedPositiveSubtractionFailed) {
	TestSBCAbsolute(testSBCSubtractTwoNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteXCanSubtractZeroFromZero) {
	TestSBCAbsoluteX(testSBCDataSubtractZeroFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteXCanSubtractOneFromZero) {
	TestSBCAbsoluteX(testSBCSubtractOneFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteXCanSubtractOneFromZeroWithCarry) {
	TestSBCAbsoluteX(testSBCDataSubtractOneFromZeroWithCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteXCanSubtractWithOverflow) {
	TestSBCAbsoluteX(testSBCDataSubtractWithOverflow);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteXCanSubtractPositiveAndNegativeNumbers) {
	TestSBCAbsoluteX(testSBCDataSubtractPositiveAndNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteXaCanSubtractZeroFromZeroAndCarry) {
	TestSBCAbsoluteX(testSBCDataCanSubtractZeroFromZeroAndCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteXCanSubtractTwoUnsignedNumbers) {
	TestSBCAbsoluteX(testSBCDataCanSubtractTwoUnsignedNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteXCanSetOverflowFlagWhenSignedPositiveSubtractionFailed) {
	TestSBCAbsoluteX(testSBCSubtractTwoNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteXCanSubtractZeroFromZeroWithPageCrossing) { 
	TestSBCAbsoluteXWithPageCrossing(testSBCDataSubtractZeroFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteXCanSubtractOneFromZeroWithPageCrossing) {
	TestSBCAbsoluteXWithPageCrossing(testSBCSubtractOneFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteXCanSubtractOneFromZeroWithCarryWithPageCrossing) {
	TestSBCAbsoluteXWithPageCrossing(testSBCDataSubtractOneFromZeroWithCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteXCanSubtractWithOverflowWithPageCrossing) {
	TestSBCAbsoluteXWithPageCrossing(testSBCDataSubtractWithOverflow);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteXCanSubtractPositiveAndNegativeNumbersWithPageCrossing) {
	TestSBCAbsoluteXWithPageCrossing(testSBCDataSubtractPositiveAndNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteXaCanSubtractZeroFromZeroAndCarryWithPageCrossing) {
	TestSBCAbsoluteXWithPageCrossing(testSBCDataCanSubtractZeroFromZeroAndCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteXCanSubtractTwoUnsignedNumbersWithPageCrossing) {
	TestSBCAbsoluteXWithPageCrossing(testSBCDataCanSubtractTwoUnsignedNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteXCanSetOverflowFlagWhenSignedPositiveSubtractionFailedWithPageCrossing) {
	TestSBCAbsoluteXWithPageCrossing(testSBCSubtractTwoNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteYCanSubtractZeroFromZero) {
	TestSBCAbsoluteY(testSBCDataSubtractZeroFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteYCanSubtractOneFromZero) {
	TestSBCAbsoluteY(testSBCSubtractOneFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteYCanSubtractOneFromZeroWithCarry) {
	TestSBCAbsoluteY(testSBCDataSubtractOneFromZeroWithCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteYCanSubtractWithOverflow) {
	TestSBCAbsoluteY(testSBCDataSubtractWithOverflow);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteYCanSubtractPositiveAndNegativeNumbers) {
	TestSBCAbsoluteY(testSBCDataSubtractPositiveAndNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteYaCanSubtractZeroFromZeroAndCarry) {
	TestSBCAbsoluteY(testSBCDataCanSubtractZeroFromZeroAndCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteYCanSubtractTwoUnsignedNumbers) {
	TestSBCAbsoluteY(testSBCDataCanSubtractTwoUnsignedNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteYCanSetOverflowFlagWhenSignedPositiveSubtractionFailed) {
	TestSBCAbsoluteY(testSBCSubtractTwoNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteYCanSubtractZeroFromZeroWithPageCrossing) {
	TestSBCAbsoluteYWithPageCrossing(testSBCDataSubtractZeroFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteYCanSubtractOneFromZeroWithPageCrossing) {
	TestSBCAbsoluteYWithPageCrossing(testSBCSubtractOneFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteYCanSubtractOneFromZeroWithCarryWithPageCrossing) {
	TestSBCAbsoluteYWithPageCrossing(testSBCDataSubtractOneFromZeroWithCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteYCanSubtractWithOverflowWithPageCrossing) {
	TestSBCAbsoluteYWithPageCrossing(testSBCDataSubtractWithOverflow);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteYCanSubtractPositiveAndNegativeNumbersWithPageCrossing) {
	TestSBCAbsoluteYWithPageCrossing(testSBCDataSubtractPositiveAndNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteYaCanSubtractZeroFromZeroAndCarryWithPageCrossing) {
	TestSBCAbsoluteYWithPageCrossing(testSBCDataCanSubtractZeroFromZeroAndCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteYCanSubtractTwoUnsignedNumbersWithPageCrossing) {
	TestSBCAbsoluteYWithPageCrossing(testSBCDataCanSubtractTwoUnsignedNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCAbsoluteYCanSetOverflowFlagWhenSignedPositiveSubtractionFailedWithPageCrossing) {
	TestSBCAbsoluteYWithPageCrossing(testSBCSubtractTwoNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectXCanSubtractZeroFromZero) {
	TestSBCIndirectX(testSBCDataSubtractZeroFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectXCanSubtractOneFromZero) {
	TestSBCIndirectX(testSBCSubtractOneFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectXCanSubtractOneFromZeroWithCarry) {
	TestSBCIndirectX(testSBCDataSubtractOneFromZeroWithCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectXCanSubtractWithOverflow) {
	TestSBCIndirectX(testSBCDataSubtractWithOverflow);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectXCanSubtractPositiveAndNegativeNumbers) {
	TestSBCIndirectX(testSBCDataSubtractPositiveAndNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectXaCanSubtractZeroFromZeroAndCarry) {
	TestSBCIndirectX(testSBCDataCanSubtractZeroFromZeroAndCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectXCanSubtractTwoUnsignedNumbers) {
	TestSBCIndirectX(testSBCDataCanSubtractTwoUnsignedNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectXCanSetOverflowFlagWhenSignedPositiveSubtractionFailed) {
	TestSBCIndirectX(testSBCSubtractTwoNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectYCanSubtractZeroFromZero) {
	TestSBCIndirectY(testSBCDataSubtractZeroFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectYCanSubtractOneFromZero) {
	TestSBCIndirectY(testSBCSubtractOneFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectYCanSubtractOneFromZeroWithCarry) {
	TestSBCIndirectY(testSBCDataSubtractOneFromZeroWithCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectYCanSubtractWithOverflow) {
	TestSBCIndirectY(testSBCDataSubtractWithOverflow);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectYCanSubtractPositiveAndNegativeNumbers) {
	TestSBCIndirectY(testSBCDataSubtractPositiveAndNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectYaCanSubtractZeroFromZeroAndCarry) {
	TestSBCIndirectY(testSBCDataCanSubtractZeroFromZeroAndCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectYCanSubtractTwoUnsignedNumbers) {
	TestSBCIndirectY(testSBCDataCanSubtractTwoUnsignedNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectYCanSetOverflowFlagWhenSignedPositiveSubtractionFailed) {
	TestSBCIndirectY(testSBCSubtractTwoNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectYCanSubtractZeroFromZeroWithPageCrossing) {
	TestSBCIndirectYWithPageCrossing(testSBCDataSubtractZeroFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectYCanSubtractOneFromZeroWithPageCrossing) {
	TestSBCIndirectYWithPageCrossing(testSBCSubtractOneFromZero);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectYCanSubtractOneFromZeroWithCarryWithPageCrossing) {
	TestSBCIndirectYWithPageCrossing(testSBCDataSubtractOneFromZeroWithCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectYCanSubtractWithOverflowWithPageCrossing) {
	TestSBCIndirectYWithPageCrossing(testSBCDataSubtractWithOverflow);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectYCanSubtractPositiveAndNegativeNumbersWithPageCrossing) {
	TestSBCIndirectYWithPageCrossing(testSBCDataSubtractPositiveAndNegativeNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectYaCanSubtractZeroFromZeroAndCarryWithPageCrossing) {
	TestSBCIndirectYWithPageCrossing(testSBCDataCanSubtractZeroFromZeroAndCarry);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectYCanSubtractTwoUnsignedNumbersWithPageCrossing) {
	TestSBCIndirectYWithPageCrossing(testSBCDataCanSubtractTwoUnsignedNumbers);
}

TEST_F(MOS6502SubtractWithCarryTests, SBCIndirectYCanSetOverflowFlagWhenSignedPositiveSubtractionFailedWithPageCrossing) {
	TestSBCIndirectYWithPageCrossing(testSBCSubtractTwoNegativeNumbers);
}