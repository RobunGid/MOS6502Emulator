#include <gtest/gtest.h>
#include <m6502.h>

enum class LogicalOperations {
	And, Eor, Or
};

class MOS6502LogicalOperationsTests : public testing::Test {
	public:
		mos6502::Memory memory;
		mos6502::CPU cpu;
		virtual void SetUp() {
			cpu.Reset(0xFFFC, memory);
		}
		virtual void TearDown() {

		}
		mos6502::Byte AffectLogicalOperation(mos6502::Byte A, mos6502::Byte B, LogicalOperations logicalOperation);
		void TestLogicalOperationOnARegisterImmediate(LogicalOperations logicalOperation);
		void TestLogicalOperationOnARegisterZeroPage(LogicalOperations logicalOperation);
		void TestLogicalOperationOnARegisterZeroPageX(LogicalOperations logicalOperation);
		void TestLogicalOperationOnARegisterAbsolute(LogicalOperations logicalOperation);
		void TestLogicalOperationOnARegisterAbsoluteX(LogicalOperations logicalOperation);
		void TestLogicalOperationOnARegisterAbsoluteY(LogicalOperations logicalOperation);
		void TestLogicalOperationOnARegisterAbsoluteXWithPageCrossing(LogicalOperations logicalOperation);
		void TestLogicalOperationOnARegisterAbsoluteYWithPageCrossing(LogicalOperations logicalOperation);
		void TestLogicalOperationOnARegisterIndirectX(LogicalOperations logicalOperation);
		void TestLogicalOperationOnARegisterIndirectY(LogicalOperations logicalOperation);
		void TestLogicalOperationOnARegisterIndirectYWithPageCrossing(LogicalOperations logicalOperation);
};

static void VerifyUnmodifiedFlagFromLoadRegister(const mos6502::CPU& cpu, const mos6502::CPU& cpu_copy) {
	EXPECT_EQ(cpu.Flag.C, cpu_copy.Flag.C);
	EXPECT_EQ(cpu.Flag.I, cpu_copy.Flag.I);
	EXPECT_EQ(cpu.Flag.D, cpu_copy.Flag.D);
	EXPECT_EQ(cpu.Flag.B, cpu_copy.Flag.B);
	EXPECT_EQ(cpu.Flag.V, cpu_copy.Flag.V);
};

mos6502::Byte MOS6502LogicalOperationsTests::AffectLogicalOperation(mos6502::Byte A, mos6502::Byte B, LogicalOperations logicalOperation) {
	switch (logicalOperation) {
		case LogicalOperations::And:
			return A & B;
			break;
		case LogicalOperations::Or:
			return A | B;
			break;
		case LogicalOperations::Eor:
			return A ^ B;
			break;
	}

	throw -1; // invalid logical operation
}

void MOS6502LogicalOperationsTests::TestLogicalOperationOnARegisterImmediate(
	LogicalOperations logicalOperation
) {
	using namespace mos6502;

	// given
	cpu.A = 0xCC;
	switch (logicalOperation) {
		case LogicalOperations::And:
			memory[0xFFFC] = CPU::AND_IM;
			break;
		case LogicalOperations::Or:
			memory[0xFFFC] = CPU::ORA_IM;
			break;
		case LogicalOperations::Eor:
			memory[0xFFFC] = CPU::EOR_IM;
			break;
	}
	memory[0xFFFD] = 0x84;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 2;

	// when
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, AffectLogicalOperation(0xCC, 0x84, logicalOperation));
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	switch (logicalOperation) {
		case LogicalOperations::And:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
		case LogicalOperations::Or:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
		case LogicalOperations::Eor:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_FALSE(cpu.Flag.N);
			break;
	}
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
}

void MOS6502LogicalOperationsTests::TestLogicalOperationOnARegisterZeroPage(
	LogicalOperations logicalOperation
) {
	using namespace mos6502;

	// given
	cpu.A = 0xCC;
	switch (logicalOperation) {
		case LogicalOperations::And:
			memory[0xFFFC] = CPU::AND_ZP;
			break;
		case LogicalOperations::Or:
			memory[0xFFFC] = CPU::ORA_ZP;
			break;
		case LogicalOperations::Eor:
			memory[0xFFFC] = CPU::EOR_ZP;
			break;
	}
	memory[0xFFFD] = 0x42;
	memory[0x42] = 0x67;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 3;

	// when
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, AffectLogicalOperation(0xCC, 0x67, logicalOperation));
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	switch (logicalOperation) {
		case LogicalOperations::And:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_FALSE(cpu.Flag.N);
			break;
		case LogicalOperations::Or:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
		case LogicalOperations::Eor:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
	}
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
};

void MOS6502LogicalOperationsTests::TestLogicalOperationOnARegisterZeroPageX(
	LogicalOperations logicalOperation
) {
	using namespace mos6502;

	// given
	cpu.A = 0xCC;
	cpu.X = 0x0F;
	switch (logicalOperation) {
		case LogicalOperations::And:
			memory[0xFFFC] = CPU::AND_ZP_X;
			break;
		case LogicalOperations::Or:
			memory[0xFFFC] = CPU::ORA_ZP_X;
			break;
		case LogicalOperations::Eor:
			memory[0xFFFC] = CPU::EOR_ZP_X;
			break;
	}
	memory[0xFFFD] = 0x42;
	memory[0x51] = 0x67;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, AffectLogicalOperation(0xCC, 0x67, logicalOperation));
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	switch (logicalOperation) {
		case LogicalOperations::And:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_FALSE(cpu.Flag.N);
			break;
		case LogicalOperations::Or:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
		case LogicalOperations::Eor:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
	}
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
};

void MOS6502LogicalOperationsTests::TestLogicalOperationOnARegisterAbsolute(
	LogicalOperations logicalOperation
) {
	using namespace mos6502;

	// given
	cpu.A = 0xCC;
	switch (logicalOperation) {
		case LogicalOperations::And:
			memory[0xFFFC] = CPU::AND_ABS;
			break;
		case LogicalOperations::Or:
			memory[0xFFFC] = CPU::ORA_ABS;
			break;
		case LogicalOperations::Eor:
			memory[0xFFFC] = CPU::EOR_ABS;
			break;
	}
	memory[0xFFFD] = 0x42;
	memory[0xFFFE] = 0x56;
	memory[0x5642] = 0x67;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, AffectLogicalOperation(0xCC, 0x67, logicalOperation));
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	switch (logicalOperation) {
		case LogicalOperations::And:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_FALSE(cpu.Flag.N);
			break;
		case LogicalOperations::Or:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
		case LogicalOperations::Eor:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
	}
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
};

void MOS6502LogicalOperationsTests::TestLogicalOperationOnARegisterAbsoluteX(
	LogicalOperations logicalOperation
) {
	using namespace mos6502;

	// given
	cpu.A = 0xCC;
	cpu.X = 0x0F;
	switch (logicalOperation) {
		case LogicalOperations::And:
			memory[0xFFFC] = CPU::AND_ABS_X;
			break;
		case LogicalOperations::Or:
			memory[0xFFFC] = CPU::ORA_ABS_X;
			break;
		case LogicalOperations::Eor:
			memory[0xFFFC] = CPU::EOR_ABS_X;
			break;
	}
	memory[0xFFFD] = 0x42;
	memory[0xFFFE] = 0x56;
	memory[0x5651] = 0x67;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, AffectLogicalOperation(0xCC, 0x67, logicalOperation));
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	switch (logicalOperation) {
		case LogicalOperations::And:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_FALSE(cpu.Flag.N);
			break;
		case LogicalOperations::Or:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
		case LogicalOperations::Eor:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
	}
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
};

void MOS6502LogicalOperationsTests::TestLogicalOperationOnARegisterAbsoluteY(
	LogicalOperations logicalOperation
) {
	using namespace mos6502;

	// given
	cpu.A = 0xCC;
	cpu.Y = 0x0F;
	switch (logicalOperation) {
		case LogicalOperations::And:
			memory[0xFFFC] = CPU::AND_ABS_Y;
			break;
		case LogicalOperations::Or:
			memory[0xFFFC] = CPU::ORA_ABS_Y;
			break;
		case LogicalOperations::Eor:
			memory[0xFFFC] = CPU::EOR_ABS_Y;
			break;
	}
	memory[0xFFFD] = 0x42;
	memory[0xFFFE] = 0x56;
	memory[0x5651] = 0x67;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, AffectLogicalOperation(0xCC, 0x67, logicalOperation));
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	switch (logicalOperation) {
		case LogicalOperations::And:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_FALSE(cpu.Flag.N);
			break;
		case LogicalOperations::Or:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
		case LogicalOperations::Eor:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
	}
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
};

void MOS6502LogicalOperationsTests::TestLogicalOperationOnARegisterAbsoluteXWithPageCrossing(
	LogicalOperations logicalOperation
) {
	using namespace mos6502;

	// given
	cpu.A = 0xCC;
	cpu.X = 0x0F;
	switch (logicalOperation) {
		case LogicalOperations::And:
			memory[0xFFFC] = CPU::AND_ABS_X;
			break;
		case LogicalOperations::Or:
			memory[0xFFFC] = CPU::ORA_ABS_X;
			break;
		case LogicalOperations::Eor:
			memory[0xFFFC] = CPU::EOR_ABS_X;
			break;
	}
	memory[0xFFFD] = 0xFF;
	memory[0xFFFE] = 0x56;
	memory[0x570E] = 0x67;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 5;

	// when
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, AffectLogicalOperation(0xCC, 0x67, logicalOperation));
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	switch (logicalOperation) {
		case LogicalOperations::And:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_FALSE(cpu.Flag.N);
			break;
		case LogicalOperations::Or:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
		case LogicalOperations::Eor:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
	}
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
};

void MOS6502LogicalOperationsTests::TestLogicalOperationOnARegisterAbsoluteYWithPageCrossing(
	LogicalOperations logicalOperation
) {
	using namespace mos6502;

	// given
	cpu.A = 0xCC;
	cpu.Y = 0x0F;
	switch (logicalOperation) {
		case LogicalOperations::And:
			memory[0xFFFC] = CPU::AND_ABS_Y;
			break;
		case LogicalOperations::Or:
			memory[0xFFFC] = CPU::ORA_ABS_Y;
			break;
		case LogicalOperations::Eor:
			memory[0xFFFC] = CPU::EOR_ABS_Y;
			break;
	}
	memory[0xFFFD] = 0xFF;
	memory[0xFFFE] = 0x56;
	memory[0x570E] = 0x67;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 5;

	// when
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, AffectLogicalOperation(0xCC, 0x67, logicalOperation));
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	switch (logicalOperation) {
		case LogicalOperations::And:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_FALSE(cpu.Flag.N);
			break;
		case LogicalOperations::Or:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
		case LogicalOperations::Eor:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
	}
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
};

void MOS6502LogicalOperationsTests::TestLogicalOperationOnARegisterIndirectX(
	LogicalOperations logicalOperation
) {
	using namespace mos6502;

	// given
	cpu.A = 0xCC;
	cpu.X = 0x0F;
	switch (logicalOperation) {
		case LogicalOperations::And:
			memory[0xFFFC] = CPU::AND_IND_X;
			break;
		case LogicalOperations::Or:
			memory[0xFFFC] = CPU::ORA_IND_X;
			break;
		case LogicalOperations::Eor:
			memory[0xFFFC] = CPU::EOR_IND_X;
			break;
	}
	memory[0xFFFD] = 0x90;
	memory[0x9F] = 0x32;
	memory[0xA0] = 0x45;
	memory[0x4532] = 0x67;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 6;

	// when
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, AffectLogicalOperation(0xCC, 0x67, logicalOperation));
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	switch (logicalOperation) {
		case LogicalOperations::And:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_FALSE(cpu.Flag.N);
			break;
		case LogicalOperations::Or:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
		case LogicalOperations::Eor:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
	}
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
};

void MOS6502LogicalOperationsTests::TestLogicalOperationOnARegisterIndirectY(
	LogicalOperations logicalOperation
) {
	using namespace mos6502;

	// given
	cpu.A = 0xCC;
	cpu.Y = 0x0F;
	switch (logicalOperation) {
		case LogicalOperations::And:
			memory[0xFFFC] = CPU::AND_IND_Y;
			break;
		case LogicalOperations::Or:
			memory[0xFFFC] = CPU::ORA_IND_Y;
			break;
		case LogicalOperations::Eor:
			memory[0xFFFC] = CPU::EOR_IND_Y;
			break;
	}
	memory[0xFFFD] = 0x90;
	memory[0x90] = 0x32;
	memory[0x91] = 0x45;
	memory[0x4541] = 0x67;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 5;

	// when
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, AffectLogicalOperation(0xCC, 0x67, logicalOperation));
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	switch (logicalOperation) {
		case LogicalOperations::And:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_FALSE(cpu.Flag.N);
			break;
		case LogicalOperations::Or:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
		case LogicalOperations::Eor:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
	}
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
};

void MOS6502LogicalOperationsTests::TestLogicalOperationOnARegisterIndirectYWithPageCrossing(
	LogicalOperations logicalOperation
) {
	using namespace mos6502;

	// given
	cpu.A = 0xCC;
	cpu.Y = 0x0F;
	switch (logicalOperation) {
		case LogicalOperations::And:
			memory[0xFFFC] = CPU::AND_IND_Y;
			break;
		case LogicalOperations::Or:
			memory[0xFFFC] = CPU::ORA_IND_Y;
			break;
		case LogicalOperations::Eor:
			memory[0xFFFC] = CPU::EOR_IND_Y;
			break;
	}
	memory[0xFFFD] = 0x90;
	memory[0x90] = 0xFF;
	memory[0x91] = 0x45;
	memory[0x460E] = 0x67;
	CPU cpu_copy = cpu;
	constexpr s32 EXPECTED_CYCLES = 6;

	// when
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(cpu.A, AffectLogicalOperation(0xCC, 0x67, logicalOperation));
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	switch (logicalOperation) {
		case LogicalOperations::And:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_FALSE(cpu.Flag.N);
			break;
		case LogicalOperations::Or:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
		case LogicalOperations::Eor:
			EXPECT_FALSE(cpu.Flag.Z);
			EXPECT_TRUE(cpu.Flag.N);
			break;
	}
	VerifyUnmodifiedFlagFromLoadRegister(cpu, cpu_copy);
};

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationANDOnARegisterImmediate) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterImmediate(LogicalOperations::And);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationORAOnARegisterImmediate) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterImmediate(LogicalOperations::Or);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationEOROnARegisterImmediate) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterImmediate(LogicalOperations::Eor);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationANDOnARegisterZeroPage) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterZeroPage(LogicalOperations::And);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationORAOnARegisterZeroPage) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterZeroPage(LogicalOperations::Or);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationEOROnARegisterZeroPage) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterZeroPage(LogicalOperations::Eor);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationANDOnARegisterZeroPageX) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterZeroPageX(LogicalOperations::And);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationORAOnARegisterZeroPageX) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterZeroPageX(LogicalOperations::Or);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationEOROnARegisterZeroPageX) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterZeroPageX(LogicalOperations::Eor);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationANDOnARegisterAbsolute) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterAbsolute(LogicalOperations::And);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationORAOnARegisterAbsolute) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterAbsolute(LogicalOperations::Or);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationEOROnARegisterAbsolute) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterAbsolute(LogicalOperations::Eor);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationANDOnARegisterAbsoluteX) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterAbsoluteX(LogicalOperations::And);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationORAOnARegisterAbsoluteX) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterAbsoluteX(LogicalOperations::Or);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationEOROnARegisterAbsoluteX) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterAbsoluteX(LogicalOperations::Eor);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationANDOnARegisterAbsoluteXWithPageCrossing) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterAbsoluteXWithPageCrossing(LogicalOperations::And);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationORAOnARegisterAbsoluteXWithPageCrossing) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterAbsoluteXWithPageCrossing(LogicalOperations::Or);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationEOROnARegisterAbsoluteXWithPageCrossing) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterAbsoluteXWithPageCrossing(LogicalOperations::Eor);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationANDOnARegisterAbsoluteY) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterAbsoluteY(LogicalOperations::And);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationORAOnARegisterAbsoluteY) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterAbsoluteY(LogicalOperations::Or);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationEOROnARegisterAbsoluteY) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterAbsoluteY(LogicalOperations::Eor);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationANDOnARegisterAbsoluteYWithPageCrossing) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterAbsoluteYWithPageCrossing(LogicalOperations::And);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationORAOnARegisterAbsoluteYWithPageCrossing) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterAbsoluteYWithPageCrossing(LogicalOperations::Or);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationEOROnARegisterAbsoluteYWithPageCrossing) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterAbsoluteYWithPageCrossing(LogicalOperations::Eor);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationANDOnARegisterIndirectX) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterIndirectX(LogicalOperations::And);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationORAOnARegisterIndirectX) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterIndirectX(LogicalOperations::Or);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationEOROnARegisterIndirectX) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterIndirectX(LogicalOperations::Eor);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationANDOnARegisterIndirectY) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterIndirectY(LogicalOperations::And);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationORAOnARegisterIndirectY) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterIndirectY(LogicalOperations::Or);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationEOROnARegisterIndirectY) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterIndirectY(LogicalOperations::Eor);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationANDOnARegisterIndirectYWithPageCrossing) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterIndirectYWithPageCrossing(LogicalOperations::And);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationORAOnARegisterIndirectYWithPageCrossing) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterIndirectYWithPageCrossing(LogicalOperations::Or);
}

TEST_F(MOS6502LogicalOperationsTests, TestLogicalOperationEOROnARegisterIndirectYWithPageCrossing) {
	using namespace mos6502;
	TestLogicalOperationOnARegisterIndirectYWithPageCrossing(LogicalOperations::Eor);
}

TEST_F(MOS6502LogicalOperationsTests, TestBitZeroPage) {
	using namespace mos6502;

	// given
	cpu.A = 0xCC;
	cpu.Flag.V = cpu.Flag.N = false;
	memory[0xFFFC] = CPU::BIT_ZP;
	memory[0xFFFD] = 0x47;
	memory[0x0047] = 0xCC;
	constexpr s32 EXPECTED_CYCLES = 3;

	// when
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0xCC);
	
	
	EXPECT_FALSE(cpu.Flag.Z);
	
	EXPECT_TRUE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.V);
};

TEST_F(MOS6502LogicalOperationsTests, TestBitZeroPageOverflowNonNegative) {
	using namespace mos6502;

	// given
	cpu.A = 0xCC;
	cpu.Flag.V = false;
	cpu.Flag.N = true;
	memory[0xFFFC] = CPU::BIT_ZP;
	memory[0xFFFD] = 0x42;
	memory[0x0042] = 0x40;
	constexpr s32 EXPECTED_CYCLES = 3;

	// when
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0xCC);
	
	
	EXPECT_FALSE(cpu.Flag.Z);
	
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.V);
};

TEST_F(MOS6502LogicalOperationsTests, TestBitZeroPageZero) {
	using namespace mos6502;

	// given
	cpu.A = 0xCC;
	cpu.Flag.V = cpu.Flag.N = true;
	memory[0xFFFC] = CPU::BIT_ZP;
	memory[0xFFFD] = 0x47;
	memory[0x0047] = 0x33;
	constexpr s32 EXPECTED_CYCLES = 3;

	// when
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0xCC);
	
	
	EXPECT_TRUE(cpu.Flag.Z);
	
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.V);
};

TEST_F(MOS6502LogicalOperationsTests, TestBitAbsolute) {
	using namespace mos6502;

	// given
	cpu.A = 0xCC;
	cpu.Flag.N = cpu.Flag.V = false;
	memory[0xFFFC] = CPU::BIT_ABS;
	memory[0xFFFD] = 0x9B;
	memory[0xFFFE] = 0x47;
	memory[0x479B] = 0xCC;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0xCC);
	
	
	EXPECT_FALSE(cpu.Flag.Z);
	
	EXPECT_TRUE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.V);
};

TEST_F(MOS6502LogicalOperationsTests, TestBitAbsoluteZero) {
	using namespace mos6502;

	// given
	cpu.A = 0xCC;
	cpu.Flag.N = cpu.Flag.V = true;
	memory[0xFFFC] = CPU::BIT_ABS;
	memory[0xFFFD] = 0x9B;
	memory[0xFFFE] = 0x47;
	memory[0x479B] = 0x33;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0xCC);
	
	
	EXPECT_TRUE(cpu.Flag.Z);
	
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_FALSE(cpu.Flag.V);
};

TEST_F(MOS6502LogicalOperationsTests, TestBitAbsoluteZeroOverflowNonNegative) {
	using namespace mos6502;

	// given
	cpu.A = 0xCC;
	cpu.Flag.N = true;
	cpu.Flag.V = false;
	memory[0xFFFC] = CPU::BIT_ABS;
	memory[0xFFFD] = 0x9B;
	memory[0xFFFE] = 0x47;
	memory[0x479B] = 0x40;
	constexpr s32 EXPECTED_CYCLES = 4;

	// when
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, memory);

	// then
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0xCC);
	
	
	EXPECT_FALSE(cpu.Flag.Z);
	
	EXPECT_FALSE(cpu.Flag.N);
	EXPECT_TRUE(cpu.Flag.V);
};
