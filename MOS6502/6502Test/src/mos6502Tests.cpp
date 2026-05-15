#include <gtest/gtest.h>
#include <m6502.h>

class MOS6502Test1 : public testing::Test {
	public:
		mos6502::Memory memory;
		mos6502::CPU cpu;
		virtual void SetUp() {
			cpu.Reset(memory);
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

TEST_F( MOS6502Test1, TheCPUDoesNothingWhenExecutesZeroCycles ) {
	using namespace mos6502;
	
	// given
	constexpr s32 NUM_CYCLES = 0;
	
	// when
	s32 CyclesUsed = cpu.Execute(NUM_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, 0);
}

TEST_F( MOS6502Test1, CPUCanExecuteMoreCyclesThanRequestedIfRequiredByInstructions ) {
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

void MOS6502Test1::TestLoadRegisterImmediate(
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*RegisterToTest
) {
	using namespace mos6502;

	// given
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

void MOS6502Test1::TestLoadRegisterZeroPage(
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*registerToTest
) {
	using namespace mos6502;

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

void MOS6502Test1::TestLoadRegisterZeroPageX (
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*registerToTest
) {
	using namespace mos6502;
	// given
	cpu.X = 0x05;
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

void MOS6502Test1::TestLoadRegisterZeroPageY (
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*registerToTest
) {
	using namespace mos6502;
	// given
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

void MOS6502Test1::TestLoadRegisterAbsolute (
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*registerToTest
) {
	using namespace mos6502;

	// given
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

void MOS6502Test1::TestLoadRegisterAbsoluteX(
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*registerToTest
) {
	using namespace mos6502;

	// given
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

void MOS6502Test1::TestLoadRegisterAbsoluteY(
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*registerToTest
) {
	using namespace mos6502;

	// given
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

void MOS6502Test1::TestLoadRegisterAbsoluteXWithPageCrossing(
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*registerToTest
) {
	using namespace mos6502;

	// given
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

void MOS6502Test1::TestLoadRegisterAbsoluteYWithPageCrossing(
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*registerToTest
) {
	using namespace mos6502;

	// given
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

TEST_F( MOS6502Test1, LDAImmeddiateCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;
	TestLoadRegisterImmediate(CPU::LDA_IM, &CPU::A);
}

TEST_F( MOS6502Test1, LDXImmeddiateCanLoadAValueIntoTheXRegister ) {
	using namespace mos6502;
	TestLoadRegisterImmediate(CPU::LDX_IM, &CPU::X);
}

TEST_F( MOS6502Test1, LDYImmeddiateCanLoadAValueIntoTheYRegister ) {
	using namespace mos6502;
	TestLoadRegisterImmediate(CPU::LDY_IM, &CPU::Y);
}


TEST_F( MOS6502Test1, LDAImmeddiateCanAffectZeroFlag ) {
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

TEST_F( MOS6502Test1, LDAZeroPageCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;
	TestLoadRegisterZeroPage(CPU::LDA_ZP, &CPU::A);
}

TEST_F( MOS6502Test1, LDXZeroPageCanLoadAValueIntoTheXRegister ) {
	using namespace mos6502;
	TestLoadRegisterZeroPage(CPU::LDX_ZP, &CPU::X);
}

TEST_F( MOS6502Test1, LDYZeroPageCanLoadAValueIntoTheYRegister ) {
	using namespace mos6502;
	TestLoadRegisterZeroPage(CPU::LDY_ZP, &CPU::Y);
}

TEST_F( MOS6502Test1, LDAZeroPageXCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;
	TestLoadRegisterZeroPageX(CPU::LDA_ZP_X, &CPU::A);
}

TEST_F( MOS6502Test1, LDXZeroPageYCanLoadAValueIntoTheXRegister ) {
	using namespace mos6502;
	TestLoadRegisterZeroPageY(CPU::LDX_ZP_Y, &CPU::X);
}

TEST_F( MOS6502Test1, LDYZeroPageXCanLoadAValueIntoTheYRegister ) {
	using namespace mos6502;
	TestLoadRegisterZeroPageX(CPU::LDY_ZP_X, &CPU::Y);
}


TEST_F( MOS6502Test1, LDAZeroPageXCanLoadAValueIntoTheARegisterWhenItWraps ) {
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

TEST_F( MOS6502Test1, LDAAbsoluteCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;
	TestLoadRegisterAbsolute(CPU::LDA_ABS, &CPU::A);
}

TEST_F( MOS6502Test1, LDXAbsoluteCanLoadAValueIntoTheXRegister ) {
	using namespace mos6502;
	TestLoadRegisterAbsolute(CPU::LDX_ABS, &CPU::X);
}

TEST_F( MOS6502Test1, LDYAbsoluteCanLoadAValueIntoTheYRegister ) {
	using namespace mos6502;
	TestLoadRegisterAbsolute(CPU::LDY_ABS, &CPU::Y);
}

TEST_F( MOS6502Test1, LDAAbsoluteXCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;
	TestLoadRegisterAbsoluteX(CPU::LDA_ABS_X, &CPU::A);
}

TEST_F( MOS6502Test1, LDYAbsoluteXCanLoadAValueIntoTheYRegister ) {
	using namespace mos6502;
	TestLoadRegisterAbsoluteX(CPU::LDY_ABS_X, &CPU::Y);
}

TEST_F( MOS6502Test1, LDAAbsoluteXCanLoadAValueIntoTheARegisterWithPageCrossing ) {
	using namespace mos6502;
	TestLoadRegisterAbsoluteXWithPageCrossing(CPU::LDA_ABS_X, &CPU::A);
}

TEST_F( MOS6502Test1, LDYAbsoluteXCanLoadAValueIntoTheYRegisterWithPageCrossing ) {
	using namespace mos6502;
	TestLoadRegisterAbsoluteYWithPageCrossing(CPU::LDA_ABS_Y, &CPU::Y);
}

TEST_F( MOS6502Test1, LDAAbsoluteYCanLoadAValueIntoTheARegister ) {
	using namespace mos6502;
	TestLoadRegisterAbsoluteY(CPU::LDA_ABS_Y, &CPU::A);
}

TEST_F( MOS6502Test1, LDXAbsoluteYCanLoadAValueIntoTheXRegister ) {
	using namespace mos6502;
	TestLoadRegisterAbsoluteY(CPU::LDX_ABS_Y, &CPU::X);
}

TEST_F( MOS6502Test1, LDAAbsoluteYCanLoadAValueIntoTheARegisterWithPageCrossing ) {
	using namespace mos6502;
	TestLoadRegisterAbsoluteYWithPageCrossing(CPU::LDA_ABS_Y, &CPU::A);
}

TEST_F( MOS6502Test1, LDXAbsoluteYCanLoadAValueIntoTheXRegisterWithPageCrossing ) {
	using namespace mos6502;
	TestLoadRegisterAbsoluteYWithPageCrossing(CPU::LDX_ABS_Y, &CPU::X);
}

TEST_F( MOS6502Test1, LDAIndirectXCanLoadAValueIntoTheARegister ) {
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

TEST_F( MOS6502Test1, LDAIndirectYCanLoadAValueIntoTheARegister ) {
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

TEST_F( MOS6502Test1, LDAIndirectYCanLoadAValueIntoTheARegisterWhenCrossesPageBoundary ) {
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