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

        void TestASLAccumulator(ShiftTestData data) {
            using namespace mos6502;
            
            // given
            memory[0xFFFC] = CPU::ASL_ACC;
            cpu.Flag.C = true;
            cpu.Flag.Z = true;
            cpu.Flag.N = true;
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

        void TestASLZeroPage(ShiftTestData data) {
            using namespace mos6502;
            
            // given
            memory[0xFFFC] = CPU::ASL_ZP;
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

        void TestASLZeroPageX(ShiftTestData data) {
            using namespace mos6502;
            
            // given
            memory[0xFFFC] = CPU::ASL_ZP_X;
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

        void TestASLAbsolute(ShiftTestData data) {
            using namespace mos6502;
            
            // given
            memory[0xFFFC] = CPU::ASL_ABS;
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

        void TestASLAbsoluteX(ShiftTestData data) {
            using namespace mos6502;
            
            // given
            memory[0xFFFC] = CPU::ASL_ABS_X;
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


TEST_F(MOS6502ShiftsTests, ASLAccumulatorCanShiftValueOfOne) {
    TestASLAccumulator(testASLDataValueOfOne);
}

TEST_F(MOS6502ShiftsTests, ASLAccumulatorCanShiftPositiveValue) {
    TestASLAccumulator(testASLDataPositiveValue);
}

TEST_F(MOS6502ShiftsTests, ASLAccumulatorCanShiftNegativeValue) {
    TestASLAccumulator(testASLDataNegativeValue);
}

TEST_F(MOS6502ShiftsTests, ASLZeroPageCanShiftOne) {
    TestASLZeroPage(testASLDataValueOfOne);
}

TEST_F(MOS6502ShiftsTests, ASLZeroPageCanShiftPositiveValue) {
    TestASLZeroPage(testASLDataPositiveValue);
}

TEST_F(MOS6502ShiftsTests, ASLZeroPageCanShiftNegativeValue) {
    TestASLZeroPage(testASLDataNegativeValue);
}

TEST_F(MOS6502ShiftsTests, ASLZeroPageXCanShiftOne) {
    TestASLZeroPageX(testASLDataValueOfOne);
}

TEST_F(MOS6502ShiftsTests, ASLZeroPageXCanShiftPositiveValue) {
    TestASLZeroPageX(testASLDataPositiveValue);
}

TEST_F(MOS6502ShiftsTests, ASLZeroPageXCanShiftNegativeValue) {
    TestASLZeroPageX(testASLDataNegativeValue);
}

TEST_F(MOS6502ShiftsTests, ASLAbsoluteCanShiftOne) {
    TestASLAbsolute(testASLDataValueOfOne);
}

TEST_F(MOS6502ShiftsTests, ASLAbsoluteCanShiftPositiveValue) {
    TestASLAbsolute(testASLDataPositiveValue);
}

TEST_F(MOS6502ShiftsTests, ASLAbsoluteCanShiftNegativeValue) {
    TestASLAbsolute(testASLDataNegativeValue);
}

TEST_F(MOS6502ShiftsTests, ASLAbsoluteXCanShiftOne) {
    TestASLAbsoluteX(testASLDataValueOfOne);
}

TEST_F(MOS6502ShiftsTests, ASLAbsoluteXCanShiftPositiveValue) {
    TestASLAbsoluteX(testASLDataPositiveValue);
}

TEST_F(MOS6502ShiftsTests, ASLAbsoluteXCanShiftNegativeValue) {
    TestASLAbsoluteX(testASLDataNegativeValue);
}