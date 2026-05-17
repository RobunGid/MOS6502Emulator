#include <gtest/gtest.h>
#include <m6502.h>

class MOS6502StoreRegisterTests : public testing::Test {
	public:
		mos6502::Memory memory;
		mos6502::CPU cpu;
		virtual void SetUp() {
			cpu.Reset(0xFFFC, memory);
		}
		virtual void TearDown() {

		}
		void TestStoreRegisterZeroPage(mos6502::Byte opcodeToTest, mos6502::Byte mos6502::CPU::*registerToTest);
		void TestStoreRegisterZeroPageX(mos6502::Byte opcodeToTest, mos6502::Byte mos6502::CPU::*registerToTest);
		void TestStoreRegisterZeroPageY(mos6502::Byte opcodeToTest, mos6502::Byte mos6502::CPU::*registerToTest);
		void TestStoreRegisterAbsolute(mos6502::Byte opcodeToTest, mos6502::Byte mos6502::CPU::*registerToTest);
};

static void VerifyUnmodifiedFlagFromStoreRegister(const mos6502::CPU& cpu, const mos6502::CPU& cpu_copy) {
	EXPECT_EQ(cpu.Z, cpu_copy.Z);
	EXPECT_EQ(cpu.N, cpu_copy.N);
	EXPECT_EQ(cpu.C, cpu_copy.C);
	EXPECT_EQ(cpu.I, cpu_copy.I);
	EXPECT_EQ(cpu.D, cpu_copy.D);
	EXPECT_EQ(cpu.B, cpu_copy.B);
	EXPECT_EQ(cpu.V, cpu_copy.V);
}

void MOS6502StoreRegisterTests::TestStoreRegisterZeroPage(
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*registerToTest
) {
	using namespace mos6502;
	
	// given
	cpu.*registerToTest = 0x2F;
	memory[0xFFFC] = opcodeToTest;
	memory[0xFFFD] = 0x80;
	memory[0x0080] = 0x00;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 3;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(memory[0x0080], 0x2F);
	VerifyUnmodifiedFlagFromStoreRegister(cpu, cpu_copy);
}

void MOS6502StoreRegisterTests::TestStoreRegisterAbsolute(
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*registerToTest
) {
	using namespace mos6502;
	
	// given
	cpu.*registerToTest = 0x2F;
	memory[0xFFFC] = opcodeToTest;
	memory[0xFFFD] = 0x4A;
	memory[0xFFFE] = 0xB3;
	memory[0xB34A] = 0x00;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(memory[0xB34A], 0x2F);
	VerifyUnmodifiedFlagFromStoreRegister(cpu, cpu_copy);
}

void MOS6502StoreRegisterTests::TestStoreRegisterZeroPageX(
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*registerToTest
) {
	using namespace mos6502;
	
	// given
	cpu.X = 0x0F;
	cpu.*registerToTest = 0x2F;
	memory[0xFFFC] = opcodeToTest;
	memory[0xFFFD] = 0x40;
	memory[0x004F] = 0x00;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(memory[0x004F], 0x2F);
	VerifyUnmodifiedFlagFromStoreRegister(cpu, cpu_copy);
}

void MOS6502StoreRegisterTests::TestStoreRegisterZeroPageY(
	mos6502::Byte opcodeToTest,
	mos6502::Byte mos6502::CPU::*registerToTest
) {
	using namespace mos6502;
	
	// given
	cpu.Y = 0x0F;
	cpu.*registerToTest = 0x2F;
	memory[0xFFFC] = opcodeToTest;
	memory[0xFFFD] = 0x40;
	memory[0x004F] = 0x00;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(memory[0x004F], 0x2F);
	VerifyUnmodifiedFlagFromStoreRegister(cpu, cpu_copy);
}

TEST_F(MOS6502StoreRegisterTests, STAZeroPageCanStoreTheARegisterIntoMemory) {
	using namespace mos6502;
	TestStoreRegisterZeroPage(CPU::STA_ZP, &CPU::A);
}

TEST_F(MOS6502StoreRegisterTests, STXZeroPageCanStoreTheXRegisterIntoMemory) {
	using namespace mos6502;
	TestStoreRegisterZeroPage(CPU::STX_ZP, &CPU::X);
}

TEST_F(MOS6502StoreRegisterTests, STYZeroPageCanStoreTheYRegisterIntoMemory) {
	using namespace mos6502;
	TestStoreRegisterZeroPage(CPU::STY_ZP, &CPU::Y);
}

TEST_F(MOS6502StoreRegisterTests, STAAbsoluteCanStoreTheARegisterIntoMemory) {
	using namespace mos6502;
	TestStoreRegisterAbsolute(CPU::STA_ABS, &CPU::A);
}

TEST_F(MOS6502StoreRegisterTests, STXAbsoluteCanStoreTheXRegisterIntoMemory) {
	using namespace mos6502;
	TestStoreRegisterAbsolute(CPU::STX_ABS, &CPU::X);
}

TEST_F(MOS6502StoreRegisterTests, STYAbsoluteCanStoreTheYRegisterIntoMemory) {
	using namespace mos6502;
	TestStoreRegisterAbsolute(CPU::STY_ABS, &CPU::Y);
}

TEST_F(MOS6502StoreRegisterTests, STAZeroPageXCanStoreTheARegisterIntoMemory) {
	using namespace mos6502;
	TestStoreRegisterZeroPageX(CPU::STA_ZP_X, &CPU::A);
}

TEST_F(MOS6502StoreRegisterTests, STYZeroPageXCanStoreTheARegisterIntoMemory) {
	using namespace mos6502;
	TestStoreRegisterZeroPageX(CPU::STY_ZP_X, &CPU::Y);
}

TEST_F(MOS6502StoreRegisterTests, STXZeroPageYCanStoreTheARegisterIntoMemory) {
	using namespace mos6502;
	TestStoreRegisterZeroPageY(CPU::STX_ZP_Y, &CPU::X);
}

TEST_F(MOS6502StoreRegisterTests, STAAbsoluteXCanStoreTheARegisterIntoMemory) {
	using namespace mos6502;

	// given
	cpu.X = 0x0F;
	cpu.A = 0x42;
	memory[0xFFFC] = CPU::STA_ABS_X;
	memory[0xFFFD] = 0x40;
	memory[0xFFFE] = 0x8D;
	memory[0x8D4F] = 0x00;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 5;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(memory[0x8D4F], 0x42);
	VerifyUnmodifiedFlagFromStoreRegister(cpu, cpu_copy);
}

TEST_F(MOS6502StoreRegisterTests, STAAbsoluteYCanStoreTheARegisterIntoMemory) {
	using namespace mos6502;

	// given
	cpu.Y = 0x0F;
	cpu.A = 0x42;
	memory[0xFFFC] = CPU::STA_ABS_Y;
	memory[0xFFFD] = 0x40;
	memory[0xFFFE] = 0x8D;
	memory[0x8D4F] = 0x00;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 5;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(memory[0x8D4F], 0x42);
	VerifyUnmodifiedFlagFromStoreRegister(cpu, cpu_copy);
}

TEST_F(MOS6502StoreRegisterTests, STAIndirectXCanStoreTheARegisterIntoMemory) {
	using namespace mos6502;

	// given
	cpu.X = 0x0F;
	cpu.A = 0x42;
	memory[0xFFFC] = CPU::STA_IND_X;
	memory[0xFFFD] = 0x40;
	memory[0x004F] = 0x76;
	memory[0x0050] = 0x45;
	memory[0x4576] = 0x00;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 6;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(memory[0x4576], 0x42);
	VerifyUnmodifiedFlagFromStoreRegister(cpu, cpu_copy);
}

TEST_F(MOS6502StoreRegisterTests, STAIndirectYCanStoreTheARegisterIntoMemory) {
	using namespace mos6502;

	// given
	cpu.Y = 0x0F;
	cpu.A = 0x42;
	memory[0xFFFC] = CPU::STA_IND_Y;
	memory[0xFFFD] = 0x40;
	memory[0x0040] = 0x76;
	memory[0x0041] = 0x45;
	memory[0x4585] = 0x00;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 6;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(memory[0x4585], 0x42);
	VerifyUnmodifiedFlagFromStoreRegister(cpu, cpu_copy);
}