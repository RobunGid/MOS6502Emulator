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

};

TEST_F(MOS6502StoreRegisterTests, JSRCanJumpToASubroutineAndJumpBack) {
	using namespace mos6502;
	// given
	cpu.Reset( 0xFF00, memory );
	memory[0xFF00] = CPU::JSR;
	memory[0xFF01] = 0x00;
	memory[0xFF02] = 0x80;
	memory[0x8000] = CPU::RTS;
	memory[0xFF03] = CPU::LDA_IM;
	memory[0xFF04] = 0x2F;
	constexpr s32 EXPECTED_CYCLES = 6 + 6 + 2;
	CPU cpu_copy = cpu;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x2F);
	EXPECT_EQ(cpu.SP, cpu_copy.SP);
}

TEST_F(MOS6502StoreRegisterTests, JSRNotAffectProcessorStatus) {
	using namespace mos6502;
	// given
	cpu.Reset( 0xFF00, memory );
	memory[0xFF00] = CPU::JSR;
	memory[0xFF01] = 0x00;
	memory[0xFF02] = 0x80;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU cpu_copy = cpu;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
	EXPECT_NE(cpu.SP, cpu_copy.SP);
	EXPECT_EQ(cpu.PC, 0x8000);
}

TEST_F(MOS6502StoreRegisterTests, RTSNotAffectProcessorStatus) {
	using namespace mos6502;
	// given
	cpu.Reset( 0xFF00, memory );
	memory[0xFF00] = CPU::JSR;
	memory[0xFF01] = 0x00;
	memory[0xFF02] = 0x80;
	memory[0x8000] = CPU::RTS;
	constexpr s32 EXPECTED_CYCLES = 6 + 6;
	CPU cpu_copy = cpu;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
	EXPECT_EQ(cpu.PC, 0xFF03);
}

TEST_F(MOS6502StoreRegisterTests, JMPAbsoluteCanJumpToNewProgramLocation) {
	using namespace mos6502;
	// given
	cpu.Reset( 0xFF00, memory );
	memory[0xFF00] = CPU::JMP_ABS;
	memory[0xFF01] = 0x00;
	memory[0xFF02] = 0x80;
	constexpr s32 EXPECTED_CYCLES = 3;
	CPU cpu_copy = cpu;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
	EXPECT_EQ(cpu.SP, cpu_copy.SP);
	EXPECT_EQ(cpu.PC, 0x8000);
}

TEST_F(MOS6502StoreRegisterTests, JMPIndirectCanJumpToNewProgramLocation) {
	using namespace mos6502;
	// given
	cpu.Reset( 0xFF00, memory );
	memory[0xFF00] = CPU::JMP_IND ;
	memory[0xFF01] = 0x00;
	memory[0xFF02] = 0x80;
	memory[0x8000] = 0x00;
	memory[0x8001] = 0x90;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU cpu_copy = cpu;

	// when
	u32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS, cpu_copy.PS);
	EXPECT_EQ(cpu.SP, cpu_copy.SP);
	EXPECT_EQ(cpu.PC, 0x9000);
}