#pragma once
#include <stdio.h>
#include <stdlib.h>

using Byte = std::uint8_t;
using Word = std::uint16_t;
using u32 = std::uint32_t;
using s32 = std::int32_t;

class Memory {
	static constexpr u32 MAX_MEM = 1024 * 64;
	Byte Data[MAX_MEM];
	
	public: void Init(void) {
		for (u32 i = 0; i < MAX_MEM; ++i) {
			Data[i] = 0;
		}
	}
	// Read 1 byte
	public: Byte operator[](u32 address) const {
		return Data[address];
	}

	// Write 1 ybte
	public: Byte& operator[](u32 address) {
		return Data[address];
	}

	// Write 2 bytes
	public: void writeWord(Word word, u32 address, s32& Cycles) {
		Data[address] = word & 0xFF;
		Data[address + 1] = (word >> 8);
		Cycles -= 2;
	}
};

class CPU {
	public:
	Word PC; // Program Counter
	Byte SP; // Stack Pointer
	Byte A, X, Y; // Accumulator, X register, Y register

	Byte C : 1; // Status flag - Carry Flag
	Byte Z : 1; // Status flag - Zero Flag
	Byte I : 1; // Status flag - Interrupt Disable
	Byte D : 1; // Status flag - Decimal Mode
	Byte B : 1; // Status flag - Break Command
	Byte V : 1; // Status flag - Overflow Flag
	Byte N : 1; // Status flag - Negative Flag
	 
	static const Byte 
	    LDA_IM = 0xA9,
		LDA_ZP = 0xA5,
		LDA_ZP_X = 0xB5,
		JSR = 0x20;

	void Reset(Memory& memory) {
		PC = 0xFFFC;
		SP = 0xFF;
		C = Z = I = D = B = V = N = 0;
		A = X = Y = 0;
		memory.Init();
	}
    // Read byte from memory at Program Counter address and increment PC
	Byte FetchByte(s32& Cycles, Memory& memory) {
		Byte data = memory[PC];
		PC++;
		Cycles--;
		return data;
	}

    // Read byte from memory at given Address
	Byte ReadByte(s32& Cycles, Byte Address, Memory& memory) {
		Byte data = memory[Address];
		Cycles--;
		return data;
	}

	// Read 2 bytes from memory at Program Counter address and increment pC
	Word FetchWord(s32& Cycles, Memory& memory) {
		Word data = memory[PC];
		PC++;
		
		data |= (memory[PC] << 8);
        PC++;
		
		Cycles -= 2;


		// if (PLATFORM_BIG_ENDIAN) {
	    // swapBytesInWord(data);
		// }

		return data;
	}

	s32 Execute(s32 Cycles, Memory& mem) {
		// const s32 CyclesRequested = Cycles;
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

				case JSR: {
					Word subAddress = FetchWord(Cycles, mem);
					mem.writeWord(PC - 1, SP, Cycles);
					SP += 2;
					PC = subAddress;
					Cycles--;
				} break;

				default: {
					printf("Unexpected instruction %d, PC = %x\n", instruction, PC);
				} break;
			}
		}
		const s32 NumCyclesUsed = CyclesRequested - Cycles;
		return NumCyclesUsed;
	}
};

