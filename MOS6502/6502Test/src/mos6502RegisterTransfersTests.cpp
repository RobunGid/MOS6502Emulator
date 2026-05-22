#include <gtest/gtest.h>
#include <m6502.h>

class MOS6502RegisterTransfersTests : public testing::Test {
	public:
		mos6502::Memory memory;
		mos6502::CPU cpu;
		virtual void SetUp() {
			cpu.Reset(0xFFFC, memory);
		}
		virtual void TearDown() {

		}
		void TestTransferRegister(
			mos6502::Byte opcode, 
			mos6502::Byte mos6502::CPU::*SourceRegister,
			mos6502::Byte mos6502::CPU::*DestinationRegister,
			mos6502::Byte value
		);
};

static void VerifyUnmodifiedFlagFromLoadRegister(const mos6502::CPU& cpu, const mos6502::CPU& cpu_copy) {
	EXPECT_EQ(cpu.Flag.C, cpu_copy.Flag.C);
	EXPECT_EQ(cpu.Flag.I, cpu_copy.Flag.I);
	EXPECT_EQ(cpu.Flag.D, cpu_copy.Flag.D);
	EXPECT_EQ(cpu.Flag.B, cpu_copy.Flag.B);
	EXPECT_EQ(cpu.Flag.V, cpu_copy.Flag.V);
}

void MOS6502RegisterTransfersTests::TestTransferRegister(
	mos6502::Byte opcode, 
	mos6502::Byte mos6502::CPU::*SourceRegister,
	mos6502::Byte mos6502::CPU::*DestinationRegister,
	mos6502::Byte value
) {
	using namespace mos6502;
	// given
	cpu.*SourceRegister = value;
	cpu.*DestinationRegister = 0xFF;
	cpu.Flag.Z = value != 0x00;
	cpu.Flag.N = (value & 0b10000000) == 0;

	memory[0xFFFC] = opcode;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 2;

	// when
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.*SourceRegister, value);
	EXPECT_EQ(cpu.*DestinationRegister, value);
	if (value == 0) {
		EXPECT_TRUE(cpu.Flag.Z);
	} else {
		EXPECT_FALSE(cpu.Flag.Z);
	};
	if ((value & 0b10000000) > 0) {
		EXPECT_TRUE(cpu.Flag.N);
	} else {
		EXPECT_FALSE(cpu.Flag.N);
	};
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
};

TEST_F( MOS6502RegisterTransfersTests, TAXCanTransferARegisterToXRegisterPositiveValue ) {
	using namespace mos6502;
	TestTransferRegister(CPU::TAX, &CPU::A, &CPU::X, 0x42);
}

TEST_F( MOS6502RegisterTransfersTests, TAXCanTransferARegisterToXRegisterZeroValue ) {
	using namespace mos6502;
	TestTransferRegister(CPU::TAX, &CPU::A, &CPU::X, 0x00);
}

TEST_F( MOS6502RegisterTransfersTests, TAXCanTransferARegisterToXRegisterNegativeValue ) {
	using namespace mos6502;
	TestTransferRegister(CPU::TAX, &CPU::A, &CPU::X, 0x80);
}

TEST_F( MOS6502RegisterTransfersTests, TAYCanTransferARegisterToYRegisterPositiveValue ) {
	using namespace mos6502;
	TestTransferRegister(CPU::TAY, &CPU::A, &CPU::Y, 0x42);
}

TEST_F( MOS6502RegisterTransfersTests, TAYCanTransferARegisterToYRegisterZeroValue ) {
	using namespace mos6502;
	TestTransferRegister(CPU::TAY, &CPU::A, &CPU::Y, 0x00);
}

TEST_F( MOS6502RegisterTransfersTests, TAYCanTransferARegisterToYRegisterNegativeValue ) {
	using namespace mos6502;
	TestTransferRegister(CPU::TAY, &CPU::A, &CPU::Y, 0x80);
}

TEST_F( MOS6502RegisterTransfersTests, TXACanTransferXRegisterToARegisterPositiveValue ) {
	using namespace mos6502;
	TestTransferRegister(CPU::TXA, &CPU::X, &CPU::A, 0x42);
}

TEST_F( MOS6502RegisterTransfersTests, TXACanTransferXRegisterToARegisterZeroValue ) {
	using namespace mos6502;
	TestTransferRegister(CPU::TXA, &CPU::X, &CPU::A, 0x00);
}

TEST_F( MOS6502RegisterTransfersTests, TXACanTransferXRegisterToARegisterNegativeValue ) {
	using namespace mos6502;
	TestTransferRegister(CPU::TXA, &CPU::X, &CPU::A, 0x80);
}

TEST_F( MOS6502RegisterTransfersTests, TYACanTransferYRegisterToARegisterPositiveValue ) {
	using namespace mos6502;
	TestTransferRegister(CPU::TYA, &CPU::Y, &CPU::A, 0x42);
}

TEST_F( MOS6502RegisterTransfersTests, TYACanTransferYRegisterToARegisterZeroValue ) {
	using namespace mos6502;
	TestTransferRegister(CPU::TYA, &CPU::Y, &CPU::A, 0x00);
}

TEST_F( MOS6502RegisterTransfersTests, TYACanTransferYRegisterToARegisterNegativeValue ) {
	using namespace mos6502;
	TestTransferRegister(CPU::TYA, &CPU::Y, &CPU::A, 0x80);
}