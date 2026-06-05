#include <gtest/gtest.h>
#include <m6502.h>

static void VerifyUnmodifiedFlagFromShifts(const mos6502::CPU& cpu, const mos6502::CPU& cpu_copy) {
    EXPECT_EQ(cpu.Flag.I, cpu_copy.Flag.I);
    EXPECT_EQ(cpu.Flag.D, cpu_copy.Flag.D);
    EXPECT_EQ(cpu.Flag.B, cpu_copy.Flag.B);
    EXPECT_EQ(cpu.Flag.V, cpu_copy.Flag.V);
}

class MOS6502ShiftsTests : public testing::Test {
    public:
        mos6502::Memory memory;
        mos6502::CPU cpu;
        virtual void SetUp() {
            cpu.Reset(0xFFFC, memory);
        };
        virtual void TearDown() {

        };

        struct ShiftTestData {
			mos6502::Byte operand;

			mos6502::Byte expect_result;
			bool expect_C;
			bool expect_Z;
			bool expect_N;
		};

        void TestShiftAccumulator(ShiftTestData data, mos6502::Byte instruction) {
            using namespace mos6502;
            
            // given
            memory[0xFFFC] = instruction;
            cpu.Flag.C = !data.expect_C;
            cpu.Flag.Z = !data.expect_Z;
            cpu.Flag.N = !data.expect_N;
            cpu.A = data.operand;
            CPU cpu_copy = cpu;

            constexpr s32 EXPECTED_CYCLES = 2;
            // when
            u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

            // then
            EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
            EXPECT_EQ(cpu.A, data.expect_result);
            EXPECT_EQ(cpu.Flag.C, data.expect_C);
            EXPECT_EQ(cpu.Flag.Z, data.expect_Z);
            EXPECT_EQ(cpu.Flag.N, data.expect_N);
            VerifyUnmodifiedFlagFromShifts(cpu, cpu_copy);
		};

        void TestShiftZeroPage(ShiftTestData data, mos6502::Byte instruction) {
            using namespace mos6502;
            
            // given
            memory[0xFFFC] = instruction;
            memory[0xFFFD] = 0x9B;
            memory[0x9B] = data.operand;
            cpu.Flag.C = !data.expect_C;
            cpu.Flag.Z = !data.expect_Z;
            cpu.Flag.N = !data.expect_N;
            CPU cpu_copy = cpu;

            constexpr s32 EXPECTED_CYCLES = 5;
            // when
            u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

            // then
            EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
            EXPECT_EQ(memory[0x9B], data.expect_result);
            EXPECT_EQ(cpu.Flag.C, data.expect_C);
            EXPECT_EQ(cpu.Flag.Z, data.expect_Z);
            EXPECT_EQ(cpu.Flag.N, data.expect_N);
            VerifyUnmodifiedFlagFromShifts(cpu, cpu_copy);
		};

        void TestShiftZeroPageX(ShiftTestData data, mos6502::Byte instruction) {
            using namespace mos6502;
            
            // given
            memory[0xFFFC] = instruction;
            memory[0xFFFD] = 0x3B;
            cpu.X = 0xA1;
            memory[0xDC] = data.operand;
            cpu.Flag.C = !data.expect_C;
            cpu.Flag.Z = !data.expect_Z;
            cpu.Flag.N = !data.expect_N;
            CPU cpu_copy = cpu;

            constexpr s32 EXPECTED_CYCLES = 6;
            // when
            u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

            // then
            EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
            EXPECT_EQ(memory[0xDC], data.expect_result);
            EXPECT_EQ(cpu.Flag.C, data.expect_C);
            EXPECT_EQ(cpu.Flag.Z, data.expect_Z);
            EXPECT_EQ(cpu.Flag.N, data.expect_N);
            VerifyUnmodifiedFlagFromShifts(cpu, cpu_copy);
		};

        void TestShiftAbsolute(ShiftTestData data, mos6502::Byte instruction) {
            using namespace mos6502;
            
            // given
            memory[0xFFFC] = instruction;
            memory[0xFFFD] = 0x67;
            memory[0xFFFE] = 0x9B;
            memory[0x9B67] = data.operand;
            cpu.Flag.C = !data.expect_C;
            cpu.Flag.Z = !data.expect_Z;
            cpu.Flag.N = !data.expect_N;
            CPU cpu_copy = cpu;

            constexpr s32 EXPECTED_CYCLES = 6;
            // when
            u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

            // then
            EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
            EXPECT_EQ(memory[0x9B67], data.expect_result);
            EXPECT_EQ(cpu.Flag.C, data.expect_C);
            EXPECT_EQ(cpu.Flag.Z, data.expect_Z);
            EXPECT_EQ(cpu.Flag.N, data.expect_N);
            VerifyUnmodifiedFlagFromShifts(cpu, cpu_copy);
		};

        void TestShiftAbsoluteX(ShiftTestData data, mos6502::Byte instruction) {
            using namespace mos6502;
            
            // given
            memory[0xFFFC] = instruction;
            memory[0xFFFD] = 0x98;
            memory[0xFFFE] = 0x3B;
            cpu.X = 0xA1;
            memory[0x3C39] = data.operand;
            cpu.Flag.C = !data.expect_C;
            cpu.Flag.Z = !data.expect_Z;
            cpu.Flag.N = !data.expect_N;
            CPU cpu_copy = cpu;

            constexpr s32 EXPECTED_CYCLES = 7;
            // when
            u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

            // then
            EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
            EXPECT_EQ(memory[0x3C39], data.expect_result);
            EXPECT_EQ(cpu.Flag.C, data.expect_C);
            EXPECT_EQ(cpu.Flag.Z, data.expect_Z);
            EXPECT_EQ(cpu.Flag.N, data.expect_N);
            VerifyUnmodifiedFlagFromShifts(cpu, cpu_copy);
		};
};

MOS6502ShiftsTests::ShiftTestData testASLDataValueOfOne{
	.operand = 0x01,
	.expect_result = 0x02,
	.expect_C = false,
	.expect_Z = false,
	.expect_N = false,
};

MOS6502ShiftsTests::ShiftTestData testASLDataPositiveValue{
	.operand = 0x5F,
	.expect_result = 0xBE,
	.expect_C = false,
	.expect_Z = false,
	.expect_N = true,
};

MOS6502ShiftsTests::ShiftTestData testASLDataNegativeValue{
	.operand = 0xC2,
	.expect_result = 0x84,
	.expect_C = true,
	.expect_Z = false,
	.expect_N = true,
};

MOS6502ShiftsTests::ShiftTestData testLSRDataValueOfOne{
	.operand = 0x01,
	.expect_result = 0x00,
	.expect_C = true,
	.expect_Z = true,
	.expect_N = false,
};

MOS6502ShiftsTests::ShiftTestData testLSRDataPositiveValue{
	.operand = 0x5F,
	.expect_result = 0x2F,
	.expect_C = true,
	.expect_Z = false,
	.expect_N = false,
};

MOS6502ShiftsTests::ShiftTestData testLSRDataNegativeValue{
	.operand = 0xC2,
	.expect_result = 0x61,
	.expect_C = false,
	.expect_Z = false,
	.expect_N = false,
};

MOS6502ShiftsTests::ShiftTestData testROLDataValueOfOneFromCarry {
	.operand = 0x00,
	.expect_result = 0x01,
	.expect_C = false,
	.expect_Z = false,
	.expect_N = false,
};

MOS6502ShiftsTests::ShiftTestData testROLDataValueOfOneIntoCarry {
	.operand = 0x80,
	.expect_result = 0x00,
	.expect_C = true,
	.expect_Z = true,
	.expect_N = false,
};

MOS6502ShiftsTests::ShiftTestData testROLDataValueOfZero {
	.operand = 0x00,
	.expect_result = 0x01,
	.expect_C = false,
	.expect_Z = false,
	.expect_N = false,
};

MOS6502ShiftsTests::ShiftTestData testROLDataValueToNegativeResult {
	.operand = 0x73,
	.expect_result = 0xE7,
	.expect_C = false,
	.expect_Z = false,
	.expect_N = true,
};

TEST_F(MOS6502ShiftsTests, ASLAccumulatorCanShiftValueOfOne) {
    TestShiftAccumulator(testASLDataValueOfOne, mos6502::CPU::ASL_ACC);
}

TEST_F(MOS6502ShiftsTests, ASLAccumulatorCanShiftPositiveValue) {
    TestShiftAccumulator(testASLDataPositiveValue, mos6502::CPU::ASL_ACC);
}

TEST_F(MOS6502ShiftsTests, ASLAccumulatorCanShiftNegativeValue) {
    TestShiftAccumulator(testASLDataNegativeValue, mos6502::CPU::ASL_ACC);
}

TEST_F(MOS6502ShiftsTests, ASLZeroPageCanShiftOne) {
    TestShiftZeroPage(testASLDataValueOfOne, mos6502::CPU::ASL_ZP);
}

TEST_F(MOS6502ShiftsTests, ASLZeroPageCanShiftPositiveValue) {
    TestShiftZeroPage(testASLDataPositiveValue, mos6502::CPU::ASL_ZP);
}

TEST_F(MOS6502ShiftsTests, ASLZeroPageCanShiftNegativeValue) {
    TestShiftZeroPage(testASLDataNegativeValue, mos6502::CPU::ASL_ZP);
}

TEST_F(MOS6502ShiftsTests, ASLZeroPageXCanShiftOne) {
    TestShiftZeroPageX(testASLDataValueOfOne, mos6502::CPU::ASL_ZP_X);
}

TEST_F(MOS6502ShiftsTests, ASLZeroPageXCanShiftPositiveValue) {
    TestShiftZeroPageX(testASLDataPositiveValue, mos6502::CPU::ASL_ZP_X);
}

TEST_F(MOS6502ShiftsTests, ASLZeroPageXCanShiftNegativeValue) {
    TestShiftZeroPageX(testASLDataNegativeValue, mos6502::CPU::ASL_ZP_X);
}

TEST_F(MOS6502ShiftsTests, ASLAbsoluteCanShiftOne) {
    TestShiftAbsolute(testASLDataValueOfOne, mos6502::CPU::ASL_ABS);
}

TEST_F(MOS6502ShiftsTests, ASLAbsoluteCanShiftPositiveValue) {
    TestShiftAbsolute(testASLDataPositiveValue, mos6502::CPU::ASL_ABS);
}

TEST_F(MOS6502ShiftsTests, ASLAbsoluteCanShiftNegativeValue) {
    TestShiftAbsolute(testASLDataNegativeValue, mos6502::CPU::ASL_ABS);
}

TEST_F(MOS6502ShiftsTests, ASLAbsoluteXCanShiftOne) {
    TestShiftAbsoluteX(testASLDataValueOfOne, mos6502::CPU::ASL_ABS_X);
}

TEST_F(MOS6502ShiftsTests, ASLAbsoluteXCanShiftPositiveValue) {
    TestShiftAbsoluteX(testASLDataPositiveValue, mos6502::CPU::ASL_ABS_X);
}

TEST_F(MOS6502ShiftsTests, ASLAbsoluteXCanShiftNegativeValue) {
    TestShiftAbsoluteX(testASLDataNegativeValue, mos6502::CPU::ASL_ABS_X);
}

TEST_F(MOS6502ShiftsTests, LSRccumulatorCanShiftValueOfOne) {
    TestShiftAccumulator(testLSRDataValueOfOne, mos6502::CPU::LSR_ACC);
}

TEST_F(MOS6502ShiftsTests, LSRAccumulatorCanShiftPositiveValue) {
    TestShiftAccumulator(testLSRDataPositiveValue, mos6502::CPU::LSR_ACC);
}

TEST_F(MOS6502ShiftsTests, LSRAccumulatorCanShiftNegativeValue) {
    TestShiftAccumulator(testLSRDataNegativeValue, mos6502::CPU::LSR_ACC);
}

TEST_F(MOS6502ShiftsTests, LSRZeroPageCanShiftOne) {
    TestShiftZeroPage(testLSRDataValueOfOne, mos6502::CPU::LSR_ZP);
}

TEST_F(MOS6502ShiftsTests, LSRZeroPageCanShiftPositiveValue) {
    TestShiftZeroPage(testLSRDataPositiveValue, mos6502::CPU::LSR_ZP);
}

TEST_F(MOS6502ShiftsTests, LSRZeroPageCanShiftNegativeValue) {
    TestShiftZeroPage(testLSRDataNegativeValue, mos6502::CPU::LSR_ZP);
}

TEST_F(MOS6502ShiftsTests, LSRZeroPageXCanShiftOne) {
    TestShiftZeroPageX(testLSRDataValueOfOne, mos6502::CPU::LSR_ZP_X);
}

TEST_F(MOS6502ShiftsTests, LSRZeroPageXCanShiftPositiveValue) {
    TestShiftZeroPageX(testLSRDataPositiveValue, mos6502::CPU::LSR_ZP_X);
}

TEST_F(MOS6502ShiftsTests, LSRZeroPageXCanShiftNegativeValue) {
    TestShiftZeroPageX(testLSRDataNegativeValue, mos6502::CPU::LSR_ZP_X);
}

TEST_F(MOS6502ShiftsTests, LSRAbsoluteCanShiftOne) {
    TestShiftAbsolute(testLSRDataValueOfOne, mos6502::CPU::LSR_ABS);
}

TEST_F(MOS6502ShiftsTests, LSRAbsoluteCanShiftPositiveValue) {
    TestShiftAbsolute(testLSRDataPositiveValue, mos6502::CPU::LSR_ABS);
}

TEST_F(MOS6502ShiftsTests, LSRAbsoluteCanShiftNegativeValue) {
    TestShiftAbsolute(testLSRDataNegativeValue, mos6502::CPU::LSR_ABS);
}

TEST_F(MOS6502ShiftsTests, LSRAbsoluteXCanShiftOne) {
    TestShiftAbsoluteX(testLSRDataValueOfOne, mos6502::CPU::LSR_ABS_X);
}

TEST_F(MOS6502ShiftsTests, LSRAbsoluteXCanShiftPositiveValue) {
    TestShiftAbsoluteX(testLSRDataPositiveValue, mos6502::CPU::LSR_ABS_X);
}

TEST_F(MOS6502ShiftsTests, LSRAbsoluteXCanShiftNegativeValue) {
    TestShiftAbsoluteX(testLSRDataNegativeValue, mos6502::CPU::LSR_ABS_X);
}

TEST_F(MOS6502ShiftsTests, ROLAccumulatorCanShiftValueOfOneFromCarry) {
    TestShiftAccumulator(testROLDataValueOfOneFromCarry, mos6502::CPU::ROL_ACC);
}

TEST_F(MOS6502ShiftsTests, ROLAccumulatorCanShiftValueOfOneIntoCarry) {
    TestShiftAccumulator(testROLDataValueOfOneIntoCarry, mos6502::CPU::ROL_ACC);
}

TEST_F(MOS6502ShiftsTests, ROLAccumulatorCanShiftValueOfZero) {
    TestShiftAccumulator(testROLDataValueOfZero, mos6502::CPU::ROL_ACC);
}

TEST_F(MOS6502ShiftsTests, ROLAccumulatorCanShiftValueToNegativeResult) {
    TestShiftAccumulator(testROLDataValueToNegativeResult, mos6502::CPU::ROL_ACC);
}

TEST_F(MOS6502ShiftsTests, ROLZeroPageCanShiftValueOfOneFromCarry) {
    TestShiftZeroPage(testROLDataValueOfOneFromCarry, mos6502::CPU::ROL_ZP);
}

TEST_F(MOS6502ShiftsTests, ROLZeroPageCanShiftValueOfOneIntoCarry) {
    TestShiftZeroPage(testROLDataValueOfOneIntoCarry, mos6502::CPU::ROL_ZP);
}

TEST_F(MOS6502ShiftsTests, ROLZeroPageCanShiftValueOfZero) {
    TestShiftZeroPage(testROLDataValueOfZero, mos6502::CPU::ROL_ZP);
}

TEST_F(MOS6502ShiftsTests, ROLZeroPageCanShiftValueToNegativeResult) {
    TestShiftZeroPage(testROLDataValueToNegativeResult, mos6502::CPU::ROL_ZP);
}

TEST_F(MOS6502ShiftsTests, ROLZeroPageXCanShiftValueOfOneFromCarry) {
    TestShiftZeroPageX(testROLDataValueOfOneFromCarry, mos6502::CPU::ROL_ZP_X);
}

TEST_F(MOS6502ShiftsTests, ROLZeroPageXCanShiftValueOfOneIntoCarry) {
    TestShiftZeroPageX(testROLDataValueOfOneIntoCarry, mos6502::CPU::ROL_ZP_X);
}

TEST_F(MOS6502ShiftsTests, ROLZeroPageXCanShiftValueOfZero) {
    TestShiftZeroPageX(testROLDataValueOfZero, mos6502::CPU::ROL_ZP_X);
}

TEST_F(MOS6502ShiftsTests, ROLZeroPageXCanShiftValueToNegativeResult) {
    TestShiftZeroPageX(testROLDataValueToNegativeResult, mos6502::CPU::ROL_ZP_X);
}
TEST_F(MOS6502ShiftsTests, ROLAbsoluteCanShiftValueOfOneFromCarry) {
    TestShiftAbsolute(testROLDataValueOfOneFromCarry, mos6502::CPU::ROL_ABS);
}

TEST_F(MOS6502ShiftsTests, ROLAbsoluteCanShiftValueOfOneIntoCarry) {
    TestShiftAbsolute(testROLDataValueOfOneIntoCarry, mos6502::CPU::ROL_ABS);
}

TEST_F(MOS6502ShiftsTests, ROLAbsoluteCanShiftValueOfZero) {
    TestShiftAbsolute(testROLDataValueOfZero, mos6502::CPU::ROL_ABS);
}

TEST_F(MOS6502ShiftsTests, ROLAbsoluteCanShiftValueToNegativeResult) {
    TestShiftAbsolute(testROLDataValueToNegativeResult, mos6502::CPU::ROL_ABS);
}

TEST_F(MOS6502ShiftsTests, ROLAbsoluteXCanShiftValueOfOneFromCarry) {
    TestShiftAbsoluteX(testROLDataValueOfOneFromCarry, mos6502::CPU::ROL_ABS_X);
}

TEST_F(MOS6502ShiftsTests, ROLAbsoluteXCanShiftValueOfOneIntoCarry) {
    TestShiftAbsoluteX(testROLDataValueOfOneIntoCarry, mos6502::CPU::ROL_ABS_X);
}

TEST_F(MOS6502ShiftsTests, ROLAbsoluteXCanShiftValueOfZero) {
    TestShiftAbsoluteX(testROLDataValueOfZero, mos6502::CPU::ROL_ABS_X);
}

TEST_F(MOS6502ShiftsTests, ROLAbsoluteXCanShiftValueToNegativeResult) {
    TestShiftAbsoluteX(testROLDataValueToNegativeResult, mos6502::CPU::ROL_ABS_X);
}