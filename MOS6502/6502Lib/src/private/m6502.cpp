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
	Word address_With_offset = address + X;
	if (((address & 0xFF) + X) > 0xFF) {
		Cycles--;
	};
	return address_With_offset;
}

mos6502::Word mos6502::CPU::GetAddressAbsoluteX_5(mos6502::s32& Cycles, const mos6502::Memory& memory) {
	Word address = FetchWord(Cycles, memory);
	address += X;
	Cycles--;
	return address;
}

mos6502::Word mos6502::CPU::GetAddressAbsoluteY(mos6502::s32& Cycles, const mos6502::Memory& memory) {
	Word address = FetchWord(Cycles, memory);
	Word address_With_offset = address + Y;
	if (((address & 0xFF) + Y) > 0xFF) {
		Cycles--;
	};
	return address_With_offset;
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
	Word address_With_offset = address + Y;
	if (((address & 0xFF) + Y) > 0xFF) {
		Cycles--;
	};
	return address_With_offset;
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
	// and set appropriate flags
	auto loadRegister = [&Cycles, &memory, this] (Word address, Byte& Register) {
		Register = ReadByte(Cycles, address, memory);
		Flag.Z = Register == 0;
		Flag.N = (Register & 0b10000000) > 0;
	};

	// Apply logical AND to A register with value from given 
	// address and set appropriate flags
	auto applyANDToARegister = [&Cycles, &memory, this] (Word address) {
		A &= ReadByte(Cycles, address, memory);
		Flag.Z = A == 0;
		Flag.N = (A & 0b10000000) > 0;
	};

	// Apply logical AND to A register with value from given 
	// address and set appropriate flags
	auto applyORToARegister = [&Cycles, &memory, this] (Word address) {
		A |= ReadByte(Cycles, address, memory);
		Flag.Z = A == 0;
		Flag.N = (A & 0b10000000) > 0;
	};

	// Apply logical AND to A register with value from given 
	// address and set appropriate flags
	auto applyEORToARegister = [&Cycles, &memory, this] (Word address) {
		A ^= ReadByte(Cycles, address, memory);
		Flag.Z = A == 0;
		Flag.N = (A & 0b10000000) > 0;
	};

	while (Cycles > 0) {
		Byte instruction = FetchByte(Cycles, memory);
		switch (instruction) {

			case LDA_IM: {
				A = FetchByte(Cycles, memory);
				Flag.Z = A == 0;
				Flag.N = (A & 0b10000000) > 0;
			} break;

			case LDX_IM: {
				X = FetchByte(Cycles, memory);
				Flag.Z = X == 0;
				Flag.N = (X & 0b10000000) > 0;
			} break;

			case LDY_IM: {
				Y = FetchByte(Cycles, memory);
				Flag.Z = Y == 0;
				Flag.N = (Y & 0b10000000) > 0;
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

			case TSX: {
				X = SP;
				Cycles--;
				Flag.Z = X == 0;
				Flag.N = (X & 0b10000000) > 0;
			} break;

			case TXS: {
				SP = X;
				Cycles--;
			} break;

			case PHA: {
				PushByteOntoStack(Cycles, A, memory);
			} break;

			case PHP: {
				PushByteOntoStack(Cycles, PS, memory);
			} break;

			case PLA: {
				A = PopByteFromStack(Cycles, memory);
				Flag.Z = A == 0;
				Flag.N = (A & 0b10000000) > 0;
			} break;

			case PLP: {
				PS = PopByteFromStack(Cycles, memory);
			} break;

			case RTS: {
				Word returnAddress = PopWordFromStack(Cycles, memory);
				PC = returnAddress + 1;
				Cycles -= 2;
			} break;

			case AND_IM: {
				A &= FetchByte(Cycles, memory);
				Flag.Z = A == 0;
				Flag.N = (A & 0b10000000) > 0;
			} break;

			case ORA_IM: {
				A |= FetchByte(Cycles, memory);
				Flag.Z = A == 0;
				Flag.N = (A & 0b10000000) > 0;
			} break;

			case EOR_IM: {
				A ^= FetchByte(Cycles, memory);
				Flag.Z = A == 0;
				Flag.N = (A & 0b10000000) > 0;
			} break;

			case AND_ZP: {
				Byte address = GetAddressZeroPage(Cycles, memory); 
				applyANDToARegister(address);
			} break;

			case ORA_ZP: {
				Byte address = GetAddressZeroPage(Cycles, memory); 
				applyORToARegister(address);
			} break;

			case EOR_ZP: {
				Byte address = GetAddressZeroPage(Cycles, memory); 
				applyEORToARegister(address);
			} break;

			case AND_ZP_X: {
				Byte address = GetAddressZeroPageX(Cycles, memory); 
				applyANDToARegister(address);
			} break;

			case ORA_ZP_X: {
				Byte address = GetAddressZeroPageX(Cycles, memory); 
				applyORToARegister(address);
			} break;

			case EOR_ZP_X: {
				Byte address = GetAddressZeroPageX(Cycles, memory); 
				applyEORToARegister(address);
			} break;

			case AND_ABS: {
				Word address = GetAddressAbsolute(Cycles, memory); 
				applyANDToARegister(address);
			} break;

			case ORA_ABS: {
				Word address = GetAddressAbsolute(Cycles, memory); 
				applyORToARegister(address);
			} break;

			case EOR_ABS: {
				Word address = GetAddressAbsolute(Cycles, memory); 
				applyEORToARegister(address);
			} break;

			case AND_ABS_X: {
				Word address = GetAddressAbsoluteX(Cycles, memory); 
				applyANDToARegister(address);
			} break;

			case ORA_ABS_X: {
				Word address = GetAddressAbsoluteX(Cycles, memory); 
				applyORToARegister(address);
			} break;

			case EOR_ABS_X: {
				Word address = GetAddressAbsoluteX(Cycles, memory); 
				applyEORToARegister(address);
			} break;

			case AND_ABS_Y: {
				Word address = GetAddressAbsoluteY(Cycles, memory); 
				applyANDToARegister(address);
			} break;

			case ORA_ABS_Y: {
				Word address = GetAddressAbsoluteY(Cycles, memory); 
				applyORToARegister(address);
			} break;

			case EOR_ABS_Y: {
				Word address = GetAddressAbsoluteY(Cycles, memory); 
				applyEORToARegister(address);
			} break;

			case AND_IND_X: {
				Word address = GetAddressIndirectX(Cycles, memory); 
				applyANDToARegister(address);
			} break;

			case ORA_IND_X: {
				Word address = GetAddressIndirectX(Cycles, memory); 
				applyORToARegister(address);
			} break;

			case EOR_IND_X: {
				Word address = GetAddressIndirectX(Cycles, memory); 
				applyEORToARegister(address);
			} break;

			case AND_IND_Y: {
				Word address = GetAddressIndirectY(Cycles, memory); 
				applyANDToARegister(address);
			} break;

			case ORA_IND_Y: {
				Word address = GetAddressIndirectY(Cycles, memory); 
				applyORToARegister(address);
			} break;

			case EOR_IND_Y: {
				Word address = GetAddressIndirectY(Cycles, memory); 
				applyEORToARegister(address);
			} break;

			case BIT_ZP: {
				Word address = GetAddressZeroPage(Cycles, memory);
				Word value = ReadByte(Cycles, address, memory);
				Flag.Z = (A & value) == 0;
				PS = (value & 0b11000000) | (PS & 0b00111111);
			} break;

			case BIT_ABS: {
				Word address = GetAddressAbsolute(Cycles, memory);
				Word value = ReadByte(Cycles, address, memory);
				Flag.Z = (A & value) == 0;
				PS = (value & 0b11000000) | (PS & 0b00111111);
			} break;

			case TAX: {
				X = A;
				Flag.Z = X == 0;
				Flag.N = (X & 0b10000000) > 0;
				Cycles--;
			} break;

			case TAY: {
				Y = A;
				Flag.Z = Y == 0;
				Flag.N = (Y & 0b10000000) > 0;
				Cycles--;
			} break;

			case TXA: {
				A = X;
				Flag.Z = A == 0;
				Flag.N = (A & 0b10000000) > 0;
				Cycles--;
			} break;

			case TYA: {
				A = Y;
				Flag.Z = A == 0;
				Flag.N = (A & 0b10000000) > 0;
				Cycles--;
			} break;

			case INX: {
				X++;
				Flag.Z = X == 0;
				Flag.N = (X & 0b10000000) > 0;
				Cycles--;
			} break;

			case INY: {
				Y++;
				Flag.Z = Y == 0;
				Flag.N = (Y & 0b10000000) > 0;
				Cycles--;
			} break;

			case DEX: {
				X--;
				Flag.Z = X == 0;
				Flag.N = (X & 0b10000000) > 0;
				Cycles--;
			} break;

			case DEY: {
				Y--;
				Flag.Z = Y == 0;
				Flag.N = (Y & 0b10000000) > 0;
				Cycles--;
			} break;

			case INC_ZP: {
				Word address = GetAddressZeroPage(Cycles, memory);
				Byte value = ReadByte(Cycles, address, memory);
				value++;
				Cycles--;
				WriteByte(value, Cycles, address, memory);
				Flag.Z = value == 0;
				Flag.N = (value & 0b10000000) > 0;
			} break;

			case DEC_ZP: {
				Word address = GetAddressZeroPage(Cycles, memory);
				Byte value = ReadByte(Cycles, address, memory);
				value--;
				Cycles--;
				WriteByte(value, Cycles, address, memory);
				Flag.Z = value == 0;
				Flag.N = (value & 0b10000000) > 0;
			} break;

			case INC_ZP_X: {
				Word address = GetAddressZeroPageX(Cycles, memory);
				Byte value = ReadByte(Cycles, address, memory);
				value++;
				Cycles--;
				WriteByte(value, Cycles, address, memory);
				Flag.Z = value == 0;
				Flag.N = (value & 0b10000000) > 0;
			} break;

			case DEC_ZP_X: {
				Word address = GetAddressZeroPageX(Cycles, memory);
				Byte value = ReadByte(Cycles, address, memory);
				value--;
				Cycles--;
				WriteByte(value, Cycles, address, memory);
				Flag.Z = value == 0;
				Flag.N = (value & 0b10000000) > 0;
			} break;

			case INC_ABS: {
				Word address = GetAddressAbsolute(Cycles, memory);
				Byte value = ReadByte(Cycles, address, memory);
				value++;
				Cycles--;
				WriteByte(value, Cycles, address, memory);
				Flag.Z = value == 0;
				Flag.N = (value & 0b10000000) > 0;
			} break;

			case DEC_ABS: {
				Word address = GetAddressAbsolute(Cycles, memory);
				Byte value = ReadByte(Cycles, address, memory);
				value--;
				Cycles--;
				WriteByte(value, Cycles, address, memory);
				Flag.Z = value == 0;
				Flag.N = (value & 0b10000000) > 0;
			} break;

			case INC_ABS_X: {
				Word address = GetAddressAbsoluteX_5(Cycles, memory);
				Byte value = ReadByte(Cycles, address, memory);
				value++;
				Cycles--;
				WriteByte(value, Cycles, address, memory);
				Flag.Z = value == 0;
				Flag.N = (value & 0b10000000) > 0;
			} break;

			case DEC_ABS_X: {
				Word address = GetAddressAbsoluteX_5(Cycles, memory);
				Byte value = ReadByte(Cycles, address, memory);
				value--;
				Cycles--;
				WriteByte(value, Cycles, address, memory);
				Flag.Z = value == 0;
				Flag.N = (value & 0b10000000) > 0;
			} break;

			case BEQ: {
				SByte offset = static_cast<SByte>(FetchByte(Cycles, memory));
				if (Flag.Z) {
					if ((PC & 0xFF00) != ((PC + offset) & 0xFF00)) {
						Cycles -= 2;
					};

					PC += offset;
					Cycles--;
				}
			} break;

			/*
			An original 6502 has does not correctly fetch 
			the target address if the indirect vector falls 
			on a page boundary (e.g. $xxFF where xx is any 
			value from $00 to $FF). In this case fetches 
			the LSB from $xxFF as expected but takes the MSB 
			from $xx00. This is fixed in some later chips 
			like the 65SC02 so for compatibility always ensure 
			the indirect vector is not at the end of the page.
			https://web.archive.org/web/20200716035408/http://www.obelisk.me.uk/6502/reference.html#JMP
			*/
			case JMP_ABS: {
				Word address = GetAddressAbsolute(Cycles, memory);
				PC = address;
			} break;

			case JMP_IND: {
				Word address = GetAddressAbsolute(Cycles, memory);
				Word actual_address = ReadWord(Cycles, address, memory);
				PC = actual_address;
			} break;

			default: {
				throw -1;
			} break;
		}
	}
	const s32 NumCyclesUsed = CyclesRequested - Cycles;
	return NumCyclesUsed;
}

mos6502::Word mos6502::CPU::LoadProgram(const Byte* program, u32 numberOfBytes, Memory& memory) { 
	Word loadAddress = 0x00;
	if (program && numberOfBytes > 2) {
		u32 at = 0;
		loadAddress = program[at] | (program[at+1] << 8);
		at += 2;
		for (Word i = loadAddress; i < loadAddress + numberOfBytes - 2; i++) {
			memory[i] = program[at++];
		};
	}
	return loadAddress;
}

void mos6502::CPU::PrintStatus() const {
	printf("A = 0x%X, X = 0x%X, Y = 0x%X\n", A, X, Y); 
	printf("PC = 0x%X, SP = 0x%X\n", PC, SP); 
	printf("PS = %d\n", PS);;
}