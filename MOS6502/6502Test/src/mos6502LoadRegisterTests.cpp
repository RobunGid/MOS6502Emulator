#include <gtest/gtest.h>
#include <m6502.h>

class MOS6502LoadRegisterTests : public testing::Test {
	public:
		mos6502::Memory memory;
		mos6502::CPU cpu;
		virtual void SetUp() {
			cpu.Reset(0xFFFC, memory);
		}
		virtual void TearDown() {

		}
		void TestLoadRegisterImmediate(mos6502::Byte opcode, mos6502::Byte mos6502::CPU::*Register);
		void TestLoadRegisterZeroPage(mos6502::Byte opcodeToTest, mos6502::Byte mos6502::CPU::*RegisterToTest);
		void TestLoadRegisterZeroPageX(mos6502::Byte opcodeToTest, mos6502::Byte mos6502::CPU::*registerToTest);
		void TestLoadRegisterZeroPageY(mos6502::Byte opcodeToTest, mos6502::Byte mos6502::CPU::*registerToTest);
		void TestLoadRegisterAbsolute(mos6502::Byte opcodeToTest, mos6502::Byte mos6502::CPU::*registerToTest);
		void TestLoadRegisterAbsoluteX(mos6502::Byte opcodeToTest, mos6502::Byte mos6502::CPU::*registerToTest);
		void TestLoadRegisterAbsoluteY(mos6502::Byte opcodeToTest, mos6502::Byte mos6502::CPU::*registerToTest);
		void TestLoadRegisterAbsoluteXWithPageCrossing(mos6502::Byte opcodeToTest, mos6502::Byte mos6502::CPU::*registerToTest);
		void TestLoadRegisterAbsoluteYWithPageCrossing(mos6502::Byte opcodeToTest, mos6502::Byte mos6502::CPU::*registerToTest);
};

static void VerifyUnmodifiedFlagFromLoadRegister(const mos6502::CPU& cpu, const mos6502::CPU& cpu_copy) {
	EXPECT_EQ(cpu.C, cpu_copy.C);
	EXPECT_EQ(cpu.I, cpu_copy.I);
	EXPECT_EQ(cpu.D, cpu_copy.D);
	EXPECT_EQ(cpu.B, cpu_copy.B);
	EXPECT_EQ(cpu.V, cpu_copy.V);
}

TEST_F( MOS6502LoadRegisterTests, TheCPUDoesNothingWhenExecutesZeroCycles ) {
	using namespace mos6502;
	
	// given
	constexpr s32 NUM_CYCLES = 0;
	
	// when
	s32 CyclesUsed = cpu.Execute(NUM_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, 0);
}

TEST_F( MOS6502LoadRegisterTests, CPUCanExecuteMoreCyclesThanRequestedIfRequiredByInstructions ) {
	using namespace mos6502;
	
	// given
	memory[0xFFFC] = CPU::LDA_IM;
	memory[0xFFFD] = 0x84;
	constexpr s32 NUM_CYCLES = 1;

	// when
	s32 CyclesUsed = cpu.Execute(NUM_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, 2);
}

void MOS6502LoadRegisterTests::TestLoadRegisterImmediate(
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*RegisterToTest
) {
	using namespace mos6502;

	// given
	cpu.Z = true;
	cpu.N = false;
	memory[0xFFFC] = opcodeToTest;
	memory[0xFFFD] = 0x84;
	CPU cpu_copy = cpu;

	// when
	s32 CyclesUsed = cpu.Execute(2, memory);

	// then
	EXPECT_EQ(cpu.*RegisterToTest, 0x84);
	EXPECT_EQ(CyclesUsed, 2);
	EXPECT_FALSE(cpu.Z);
	EXPECT_TRUE(cpu.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

void MOS6502LoadRegisterTests::TestLoadRegisterZeroPage(
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*registerToTest
) {
	using namespace mos6502;

	// given
	cpu.Z = cpu.N = true;
	memory[0xFFFC] = opcodeToTest;
	memory[0xFFFD] = 0x42;
	memory[0x42] = 0x67;
	CPU cpu_copy = cpu;

	// when
	s32 CyclesUsed = cpu.Execute(3, memory);

	// then
	EXPECT_EQ(cpu.*registerToTest, 0x67);
	EXPECT_EQ(CyclesUsed, 3);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
};

void MOS6502LoadRegisterTests::TestLoadRegisterZeroPageX (
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*registerToTest
) {
	using namespace mos6502;
	// given
	cpu.Z = cpu.N = true;
	cpu.X = 0x05;
	memory[0xFFFC] = opcodeToTest;
	memory[0xFFFD] = 0x42; // 0x42 + 0x05 = 0x47
	memory[0x47] = 0x67;
	CPU cpu_copy = cpu;

	// when
	s32 CyclesUsed = cpu.Execute(4, memory);

	// then
	EXPECT_EQ(cpu.*registerToTest, 0x67);
	EXPECT_EQ(CyclesUsed, 4);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
};

void MOS6502LoadRegisterTests::TestLoadRegisterZeroPageY (
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*registerToTest
) {
	using namespace mos6502;
	// given
	cpu.Z = cpu.N = true;
	cpu.Y = 0x05;
	memory[0xFFFC] = opcodeToTest;
	memory[0xFFFD] = 0x42;
	memory[0x47] = 0x67;
	CPU cpu_copy = cpu;

	// when
	s32 CyclesUsed = cpu.Execute(4, memory);

	// then
	EXPECT_EQ(cpu.*registerToTest, 0x67);
	EXPECT_EQ(CyclesUsed, 4);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
};

void MOS6502LoadRegisterTests::TestLoadRegisterAbsolute (
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*registerToTest
) {
	using namespace mos6502;

	// given
	cpu.Z = cpu.N = true;
	memory[0xFFFC] = opcodeToTest;
	memory[0xFFFD] = 0x80;
	memory[0xFFFE] = 0x54; // address 0x5480
	memory[0x5480] = 0x37;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.*registerToTest, 0x37);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);	
};

void MOS6502LoadRegisterTests::TestLoadRegisterAbsoluteX(
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*registerToTest
) {
	using namespace mos6502;

	// given
	cpu.Z = cpu.N = true;
	cpu.X = 0x01;
	memory[0xFFFC] = opcodeToTest;
	memory[0xFFFD] = 0x80;
	memory[0xFFFE] = 0x54; // address 0x5480 + 0x01 = 0x5481
	memory[0x5481] = 0x37;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.*registerToTest, 0x37);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

void MOS6502LoadRegisterTests::TestLoadRegisterAbsoluteY(
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*registerToTest
) {
	using namespace mos6502;

	// given
	cpu.Z = cpu.N = true;
	cpu.Y = 0x01;
	memory[0xFFFC] = opcodeToTest;
	memory[0xFFFD] = 0x80;
	memory[0xFFFE] = 0x54; // address 0x5480 + 0x01 = 0x5481
	memory[0x5481] = 0x37;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.*registerToTest, 0x37);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

void MOS6502LoadRegisterTests::TestLoadRegisterAbsoluteXWithPageCrossing(
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*registerToTest
) {
	using namespace mos6502;

	// given
	cpu.Z = cpu.N = true;
	cpu.X = 0xFF;
	memory[0xFFFC] = opcodeToTest;
	memory[0xFFFD] = 0x02;
	memory[0xFFFE] = 0x54; // address 0x5402 + 0xFF = 0x5501 that crosses page boundary
	memory[0x5501] = 0x37; 
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 5;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.*registerToTest, 0x37);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

void MOS6502LoadRegisterTests::TestLoadRegisterAbsoluteYWithPageCrossing(
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*registerToTest
) {
	using namespace mos6502;

	// given
	cpu.Z = cpu.N = true;
	cpu.Y = 0xFF;
	memory[0xFFFC] = opcodeToTest;
	memory[0xFFFD] = 0x02;
	memory[0xFFFE] = 0x54; // address 0x5402 + 0xFF = 0x5501 that crosses page boundary
	memory[0x5501] = 0x37; 
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU cpu_copy = cpu;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.*registerToTest, 0x37);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F( MOS6502LoadRegisterTests, LDAImmeddiateCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;
	TestLoadRegisterImmediate(CPU::LDA_IM, &CPU::A);
}

TEST_F( MOS6502LoadRegisterTests, LDXImmeddiateCanLoadAValueIntoTheXRegister ) {
	using namespace mos6502;
	TestLoadRegisterImmediate(CPU::LDX_IM, &CPU::X);
}

TEST_F( MOS6502LoadRegisterTests, LDYImmeddiateCanLoadAValueIntoTheYRegister ) {
	using namespace mos6502;
	TestLoadRegisterImmediate(CPU::LDY_IM, &CPU::Y);
}


TEST_F( MOS6502LoadRegisterTests, LDAImmeddiateCanAffectZeroFlag ) {
	using namespace mos6502;
	
	// given
	cpu.A = 0xff;
	memory[0xFFFC] = CPU::LDA_IM;
	memory[0xFFFD] = 0x00;
	CPU cpu_copy = cpu;

	// when
	cpu.Execute(2, memory);

	// then
	EXPECT_TRUE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F( MOS6502LoadRegisterTests, LDAZeroPageCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;
	TestLoadRegisterZeroPage(CPU::LDA_ZP, &CPU::A);
}

TEST_F( MOS6502LoadRegisterTests, LDXZeroPageCanLoadAValueIntoTheXRegister ) {
	using namespace mos6502;
	TestLoadRegisterZeroPage(CPU::LDX_ZP, &CPU::X);
}

TEST_F( MOS6502LoadRegisterTests, LDYZeroPageCanLoadAValueIntoTheYRegister ) {
	using namespace mos6502;
	TestLoadRegisterZeroPage(CPU::LDY_ZP, &CPU::Y);
}

TEST_F( MOS6502LoadRegisterTests, LDAZeroPageXCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;
	TestLoadRegisterZeroPageX(CPU::LDA_ZP_X, &CPU::A);
}

TEST_F( MOS6502LoadRegisterTests, LDXZeroPageYCanLoadAValueIntoTheXRegister ) {
	using namespace mos6502;
	TestLoadRegisterZeroPageY(CPU::LDX_ZP_Y, &CPU::X);
}

TEST_F( MOS6502LoadRegisterTests, LDYZeroPageXCanLoadAValueIntoTheYRegister ) {
	using namespace mos6502;
	TestLoadRegisterZeroPageX(CPU::LDY_ZP_X, &CPU::Y);
}


TEST_F( MOS6502LoadRegisterTests, LDAZeroPageXCanLoadAValueIntoTheARegisterWhenItWraps ) {
	using namespace mos6502;
	
	// given
	cpu.X = 0xFF;
	memory[0xFFFC] = CPU::LDA_ZP_X;
	memory[0xFFFD] = 0x80;
	memory[0x7F] = 0x67;
	CPU cpu_copy = cpu;

	// when
	u32 CyclesUsed = cpu.Execute(4, memory);

	// then
	EXPECT_EQ(cpu.A, 0x67);
	EXPECT_EQ(CyclesUsed, 4);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F( MOS6502LoadRegisterTests, LDAAbsoluteCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;
	TestLoadRegisterAbsolute(CPU::LDA_ABS, &CPU::A);
}

TEST_F( MOS6502LoadRegisterTests, LDXAbsoluteCanLoadAValueIntoTheXRegister ) {
	using namespace mos6502;
	TestLoadRegisterAbsolute(CPU::LDX_ABS, &CPU::X);
}

TEST_F( MOS6502LoadRegisterTests, LDYAbsoluteCanLoadAValueIntoTheYRegister ) {
	using namespace mos6502;
	TestLoadRegisterAbsolute(CPU::LDY_ABS, &CPU::Y);
}

TEST_F( MOS6502LoadRegisterTests, LDAAbsoluteXCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;
	TestLoadRegisterAbsoluteX(CPU::LDA_ABS_X, &CPU::A);
}

TEST_F( MOS6502LoadRegisterTests, LDYAbsoluteXCanLoadAValueIntoTheYRegister ) {
	using namespace mos6502;
	TestLoadRegisterAbsoluteX(CPU::LDY_ABS_X, &CPU::Y);
}

TEST_F( MOS6502LoadRegisterTests, LDAAbsoluteXCanLoadAValueIntoTheARegisterWithPageCrossing ) {
	using namespace mos6502;
	TestLoadRegisterAbsoluteXWithPageCrossing(CPU::LDA_ABS_X, &CPU::A);
}

TEST_F( MOS6502LoadRegisterTests, LDYAbsoluteXCanLoadAValueIntoTheYRegisterWithPageCrossing ) {
	using namespace mos6502;
	TestLoadRegisterAbsoluteXWithPageCrossing(CPU::LDY_ABS_X, &CPU::Y);
}

TEST_F( MOS6502LoadRegisterTests, LDAAbsoluteYCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;
	TestLoadRegisterAbsoluteY(CPU::LDA_ABS_Y, &CPU::A);
}

TEST_F( MOS6502LoadRegisterTests, LDXAbsoluteYCanLoadAValueIntoTheXRegister ) {
	using namespace mos6502;
	TestLoadRegisterAbsoluteY(CPU::LDX_ABS_Y, &CPU::X);
}

TEST_F( MOS6502LoadRegisterTests, LDAAbsoluteYCanLoadAValueIntoTheARegisterWithPageCrossing ) {
	using namespace mos6502;
	TestLoadRegisterAbsoluteYWithPageCrossing(CPU::LDA_ABS_Y, &CPU::A);
}

TEST_F( MOS6502LoadRegisterTests, LDXAbsoluteYCanLoadAValueIntoTheXRegisterWithPageCrossing ) {
	using namespace mos6502;
	TestLoadRegisterAbsoluteYWithPageCrossing(CPU::LDX_ABS_Y, &CPU::X);
}

TEST_F( MOS6502LoadRegisterTests, LDAIndirectXCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;

	// given
	cpu.X = 0x04;
	memory[0xFFFC] = CPU::LDA_IND_X;
	memory[0xFFFD] = 0x02;
	memory[0x0006] = 0x00; // 0x02 + 0x04 = 0x06
	memory[0x0007] = 0x80; 
	memory[0x8000] = 0x16; 
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU cpu_copy = cpu;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, 0x16);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F( MOS6502LoadRegisterTests, LDAIndirectYCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;

	// given
	cpu.Y = 0x04;
	memory[0xFFFC] = CPU::LDA_IND_Y;
	memory[0xFFFD] = 0x02;
	memory[0x0002] = 0x00; 
	memory[0x0003] = 0x80; 
	memory[0x8004] = 0x16;  // 0x8000 + 0x04 = 0x8004
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU cpu_copy = cpu;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, 0x16);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

TEST_F( MOS6502LoadRegisterTests, LDAIndirectYCanLoadAValueIntoTheARegisterWhenCrossesPageBoundary ) {
	using namespace mos6502;

	// given
	cpu.Y = 0xFF;
	memory[0xFFFC] = CPU::LDA_IND_Y;
	memory[0xFFFD] = 0x02;
	memory[0x0002] = 0x02;
	memory[0x0003] = 0x80; 
	memory[0x8101] = 0x16; // 0x8002 + 0xFF = 0x8101
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU cpu_copy = cpu;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, 0x16);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}