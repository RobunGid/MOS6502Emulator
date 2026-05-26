#include <gtest/gtest.h>
#include <m6502.h>

enum class LogicalOperations {
	And, Eor, Or
};

/*
* = $1000
lda #$FF
start
sta $90
sta $8000
eor #$CC
jmp start
*/

static mos6502::Byte prg[] = {
	0x00, 0x10, 0xA9, 0xFF, 0x85, 0x90, 0x8D, 0x00, 
	0x80, 0x49, 0xCC, 0x4C, 0x02, 0x10, 
};

class MOS6502ProgramTests : public testing::Test {
	public:
		mos6502::Memory memory;
		mos6502::CPU cpu;

		virtual void SetUp() {
			cpu.Reset(0xFFFC, memory);
		}
		virtual void TearDown() {

		}
};

TEST_F(MOS6502ProgramTests, TestLoadProgramIntoMemory) {
	using namespace mos6502;

	// when
	cpu.LoadProgram(prg, 14, memory);

	// then
	EXPECT_EQ(memory[0x0FFF], 0x00);
	EXPECT_EQ(memory[0x1000], 0xA9);
	EXPECT_EQ(memory[0x1001], 0xFF);
	EXPECT_EQ(memory[0x1002], 0x85);
	// ...
	EXPECT_EQ(memory[0x1009], 0x4C);
	EXPECT_EQ(memory[0x100A], 0x02);
	EXPECT_EQ(memory[0x100B], 0x10);
};

TEST_F(MOS6502ProgramTests, TestLoadAndExecuteProgram) {
	using namespace mos6502;

	// when
	Word startAddress = cpu.LoadProgram(prg, 14, memory);
	cpu.PC = startAddress;

	// then
	for (mos6502::s32 clock = 10000; clock > 0;) {
		clock -= cpu.Execute(20, memory);
	}
};

TEST_F(MOS6502ProgramTests, TestLoadAndExecuteFibbonaciCalculateProgram) {
	using namespace mos6502;
	mos6502::Byte prg[] = {
		0x00, 0x10, 0xa9, 0x00, 0x85, 0x00, 0xa9, 0x01, 
		0x85, 0x01, 0xa9, 0x0a, 0x85, 0x03, 0xa5, 0x00, 
		0x18, 0x65, 0x01, 0x85, 0x02, 0xa5, 0x01, 0x85, 
		0x00, 0xa5, 0x02, 0x85, 0x01, 0xc6, 0x03, 0xd0, 
		0xed, 0xea, 0x4c, 0x1f, 0x10, 
	};

	// when
	Word startAddress = cpu.LoadProgram(prg, 37, memory);
	cpu.PC = startAddress;

	// then
	for (mos6502::s32 clock = 1000; clock > 0;) {
		clock -= cpu.Execute(1, memory);
		cpu.PrintStatus();
		printf("0x01 = %x\n", memory[0x01]);
		printf("current ins = %x\n", memory[cpu.PC]);
	}
};