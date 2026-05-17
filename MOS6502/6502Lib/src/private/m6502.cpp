#include "m6502.h"

mos6502::Word mos6502::CPU::GetAddressZeroPage(mos6502::s32& Cycles, const mos6502::Memory& memory) {
	Word address = FetchByte(Cycles, memory);
	return address;
}

mos6502::Word mos6502::CPU::GetAddressZeroPageX(mos6502::s32& Cycles, const mos6502::Memory& memory) {
	Word address = FetchByte(Cycles, memory);
	address += X;
	Cycles--;
	return address;
}

mos6502::Word mos6502::CPU::GetAddressZeroPageY(mos6502::s32& Cycles, const mos6502::Memory& memory) {
	Word address = FetchByte(Cycles, memory);
	address += Y;
	Cycles--;
	return address;
}

mos6502::Word mos6502::CPU::GetAddressAbsolute(mos6502::s32& Cycles, const mos6502::Memory& memory) {
	Word address = FetchWord(Cycles, memory);
	return address;
}

mos6502::Word mos6502::CPU::GetAddressAbsoluteX(mos6502::s32& Cycles, const mos6502::Memory& memory) {
	Word address = FetchWord(Cycles, memory);
	address += X;
	if (((address & 0xFF) + X) > 0xFF) {
		Cycles--;
	};
	return address;
}

mos6502::Word mos6502::CPU::GetAddressAbsoluteX_5(mos6502::s32& Cycles, const mos6502::Memory& memory) {
	Word address = FetchWord(Cycles, memory);
	address += X;
	Cycles--;
	return address;
}

mos6502::Word mos6502::CPU::GetAddressAbsoluteY(mos6502::s32& Cycles, const mos6502::Memory& memory) {
	Word address = FetchWord(Cycles, memory);
	address += Y;
	if (((address & 0xFF) + Y) > 0xFF) {
		Cycles--;
	};
	return address;
}

mos6502::Word mos6502::CPU::GetAddressAbsoluteY_5(mos6502::s32& Cycles, const mos6502::Memory& memory) {
	Word address = FetchWord(Cycles, memory);
	address += Y;
	Cycles--;
	return address;
}

mos6502::Word mos6502::CPU::GetAddressIndirectX(mos6502::s32& Cycles, const mos6502::Memory& memory) {
	Byte zero_page_address = FetchByte(Cycles, memory);
	zero_page_address += X;
	Cycles--;
	Word address = ReadWord(Cycles, zero_page_address, memory);
	return address;
}

mos6502::Word mos6502::CPU::GetAddressIndirectY(mos6502::s32& Cycles, const mos6502::Memory& memory) {
	Byte zero_page_address = FetchByte(Cycles, memory);
	Word address = ReadWord(Cycles, zero_page_address, memory);
	address += Y;
	if (((address & 0xFF) + Y) > 0xFF) {
		Cycles--;
	};
	return address;
}

mos6502::Word mos6502::CPU::GetAddressIndirectY_6(mos6502::s32& Cycles, const mos6502::Memory& memory) {
	Byte zero_page_address = FetchByte(Cycles, memory);
	Word address = ReadWord(Cycles, zero_page_address, memory);
	address += Y;
	Cycles--;
	return address;
}

mos6502::s32 mos6502::CPU::Execute(s32 Cycles, Memory& memory) {
	const s32 CyclesRequested = Cycles;

	// Load given register with value from given address
	auto loadRegister = [&Cycles, &memory, this] (Word address, Byte& Register) {
		Register = ReadByte(Cycles, address, memory);
		Z = Register == 0;
		N = (Register & 0b10000000) > 0;
	};

	while (Cycles > 0) {
		Byte instruction = FetchByte(Cycles, memory);
		switch (instruction) {

			case LDA_IM: {
				A = FetchByte(Cycles, memory);
				Z = A == 0;
				N = (A & 0b10000000) > 0;
			} break;

			case LDX_IM: {
				X = FetchByte(Cycles, memory);
				Z = X == 0;
				N = (X & 0b10000000) > 0;
			} break;

			case LDY_IM: {
				Y = FetchByte(Cycles, memory);
				Z = Y == 0;
				N = (Y & 0b10000000) > 0;
			} break;

			case LDA_ZP: {
				Word address = GetAddressZeroPage(Cycles, memory);
				loadRegister(address, A);
			} break;

			case LDX_ZP: {
				Word address = GetAddressZeroPage(Cycles, memory);
				loadRegister(address, X);
			} break;

			case LDY_ZP: {
				Byte address = GetAddressZeroPage(Cycles, memory);
				loadRegister(address, Y);
			} break;

			case LDA_ZP_X: {
				Byte address = GetAddressZeroPageX(Cycles, memory);
				loadRegister(address, A);
			} break;

			case LDX_ZP_Y: {
				Byte address = GetAddressZeroPageY(Cycles, memory);
				loadRegister(address, X);
			} break;

			case LDY_ZP_X: {
				Word address = GetAddressZeroPageX(Cycles, memory);
				loadRegister(address, Y);
			} break;

			case LDA_ABS: {
				Word address = GetAddressAbsolute(Cycles, memory);
				loadRegister(address, A);
			} break;

			case LDX_ABS: {
				Word address = GetAddressAbsolute(Cycles, memory);
				loadRegister(address, X);
			} break;

			case LDY_ABS: {
				Word address = GetAddressAbsolute(Cycles, memory);
				loadRegister(address, Y);
			} break;

			case LDA_ABS_X: {
				Word address = GetAddressAbsoluteX(Cycles, memory);
				loadRegister(address, A);
			} break;

			case LDY_ABS_X: {
				Word address = GetAddressAbsoluteX(Cycles, memory);
				loadRegister(address, Y);
			} break;

			case LDA_ABS_Y: {
				Word address = GetAddressAbsoluteY(Cycles, memory);
				loadRegister(address, A);
			} break;

			case LDX_ABS_Y: {
				Word address = GetAddressAbsoluteY(Cycles, memory);
				loadRegister(address, X);
			} break;

			case LDA_IND_X: {
				Word address = GetAddressIndirectX(Cycles, memory);
				loadRegister(address, A);
			} break;

			case LDA_IND_Y: {
				Word address = GetAddressIndirectY(Cycles, memory);
				loadRegister(address, A);
			} break;

			case STA_ZP: {
				Word address = GetAddressZeroPage(Cycles, memory);
				WriteByte(A, Cycles, address, memory);
			} break;

			case STA_ZP_X: {
				Word address = GetAddressZeroPageX(Cycles, memory);
				WriteByte(A, Cycles, address, memory);
			} break;

			case STX_ZP: {
				Word address = GetAddressZeroPage(Cycles, memory);
				WriteByte(X, Cycles, address, memory);
			} break;

			case STX_ZP_Y: {
				Word address = GetAddressZeroPageY(Cycles, memory);
				WriteByte(X, Cycles, address, memory);
			} break;

			case STY_ZP: {
				Word address = GetAddressZeroPage(Cycles, memory);
				WriteByte(Y, Cycles, address, memory);
			} break;

			case STY_ZP_X: {
				Word address = GetAddressZeroPageX(Cycles, memory);
				WriteByte(Y, Cycles, address, memory);
			} break;

			case STA_ABS: {
				Word address = GetAddressAbsolute(Cycles, memory);
				WriteByte(A, Cycles, address, memory);
			} break;

			case STA_ABS_X: {
				Word address = GetAddressAbsoluteX_5(Cycles, memory);
				WriteByte(A, Cycles, address, memory);
			} break;

			case STA_ABS_Y: {
				Word address = GetAddressAbsoluteY_5(Cycles, memory);
				WriteByte(A, Cycles, address, memory);
			} break;

			case STA_IND_X: {
				Word address = GetAddressIndirectX(Cycles, memory);
				WriteByte(A, Cycles, address, memory);
			} break;

			case STA_IND_Y: {
				Word address = GetAddressIndirectY_6(Cycles, memory);
				WriteByte(A, Cycles, address, memory);
			} break;

			case STX_ABS: {
				Word address = GetAddressAbsolute(Cycles, memory);
				WriteByte(X, Cycles, address, memory);
			} break;

			case STY_ABS: {
				Word address = GetAddressAbsolute(Cycles, memory);
				WriteByte(Y, Cycles, address, memory);
			} break;

			case JSR: {
				Word subAddress = FetchWord(Cycles, memory);
				PushPCToStack(Cycles, memory);
				PC = subAddress;
				Cycles--;
			} break;

			case RTS: {
				Word ReturnAddress = PopWordFromStack(Cycles, memory);
				PC = ReturnAddress + 1;
				Cycles -= 2;
			} break;

			default: {
				throw -1;
			} break;
		}
	}
	const s32 NumCyclesUsed = CyclesRequested - Cycles;
	return NumCyclesUsed;
}