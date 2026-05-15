#include "m6502.h"

mos6502::s32 mos6502::CPU::Execute(s32 Cycles, Memory& mem) {
	const s32 CyclesRequested = Cycles;
	while (Cycles > 0) {
		Byte instruction = FetchByte(Cycles, mem);
		switch (instruction) {

			case LDA_IM: {
				Byte value = FetchByte(Cycles, mem);
				A = value;
				Z = A == 0;
				N = (A & 0b10000000) > 0;
			} break;

			case LDA_ZP: {
				Byte zero_page_address = FetchByte(Cycles, mem);
				A = ReadByte(Cycles, zero_page_address, mem);
				Z = A == 0;
				N = (A & 0b10000000) > 0;
			} break;

			case LDA_ZP_X: {
				Byte zero_page_address = FetchByte(Cycles, mem);
				zero_page_address += X;
				Cycles--;
				A = ReadByte(Cycles, zero_page_address, mem);
				Z = A == 0;
				N = (A & 0b10000000) > 0;
			} break;

			case LDA_ABS: {
				Word absolute_address = FetchWord(Cycles, mem);
				A = ReadByte(Cycles, absolute_address, mem);
				Z = A == 0;
				N = (A & 0b10000000) > 0;
			} break;

			case LDA_ABS_X: {
				Word absolute_address = FetchWord(Cycles, mem);
				Word absolute_address_x = absolute_address + X;
				if (((absolute_address & 0xFF) + X) > 0xFF) {
					Cycles--;
				};
				A = ReadByte(Cycles, absolute_address_x, mem);
				Z = A == 0;
				N = (A & 0b10000000) > 0;
			} break;

			case LDA_ABS_Y: {
				Word absolute_address = FetchWord(Cycles, mem);
				Word absolute_address_y = absolute_address + Y;
				if (((absolute_address & 0xFF) + Y) > 0xFF) {
					Cycles--;
				};
				A = ReadByte(Cycles, absolute_address_y, mem);
				Z = A == 0;
				N = (A & 0b10000000) > 0;
			} break;

			case LDA_IND_X: {
				Byte zero_page_address = FetchByte(Cycles, mem);
				Byte zero_page_address_x = zero_page_address + X;
				Cycles--;
				Word address = ReadWord(Cycles, zero_page_address_x, mem);
				A = ReadByte(Cycles, address, mem);
				Z = A == 0;
				N = (A & 0b10000000) > 0;
			} break;

			case LDA_IND_Y: {
				Byte zero_page_address = FetchByte(Cycles, mem);
				Word address = ReadWord(Cycles, zero_page_address, mem);
				Word address_y = address + Y;
				if (((address_y & 0xFF) + Y) > 0xFF) {
					Cycles--;
				};
				A = ReadByte(Cycles, address_y, mem);
				Z = A == 0;
				N = (A & 0b10000000) > 0;
			} break;

			case JSR: {
				Word subAddress = FetchWord(Cycles, mem);
				mem.writeWord(PC - 1, SP, Cycles);
				SP += 2;
				PC = subAddress;
				Cycles--;
			} break;

			default: {
				throw -1;
			} break;
		}
	}
	const s32 NumCyclesUsed = CyclesRequested - Cycles;
	return NumCyclesUsed;
}