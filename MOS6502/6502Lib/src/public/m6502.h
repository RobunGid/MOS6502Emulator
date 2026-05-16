#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <cstdint>

namespace mos6502 {
	using Byte = std::uint8_t;
	using Word = std::uint16_t;
	using u32 = std::uint32_t;
	using s32 = std::int32_t;

	class Memory;
	class CPU;
}

class mos6502::Memory {
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

class mos6502::CPU {
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
		// LDA
	    LDA_IM = 0xA9,
		LDA_ZP = 0xA5,
		LDA_ZP_X = 0xB5,
		LDA_ABS = 0xAD,
		LDA_ABS_X = 0xBD,
		LDA_ABS_Y = 0xB9,
		LDA_IND_X = 0xA1,
		LDA_IND_Y = 0xB1,

		// LDX
		LDX_IM = 0xA2,
		LDX_ZP = 0xA6,
		LDX_ZP_Y = 0xB6,
		LDX_ABS = 0xAE,
		LDX_ABS_Y = 0xBE,

		// LDY
		LDY_IM = 0xA0,
		LDY_ZP = 0xA4,
		LDY_ZP_X = 0xB4,
		LDY_ABS = 0xAC,
		LDY_ABS_X = 0xBC,

		// STA
		STA_ZP = 0x85,
		STA_ZP_X = 0x95,
		STA_ABS = 0x8D,
		STA_ABS_X = 0x9D,
		STA_ABS_Y = 0x99,
		STA_IND_X = 0x81,
		STA_IND_Y = 0x91,

		// STX
		STX_ZP = 0x86,
		STX_ZP_Y = 0x96,
		STX_ABS = 0x8E,

		// STY
		STY_ZP = 0x84,
		STY_ZP_X = 0x94,
		STY_ABS = 0x8c,

		JSR = 0x20;

	void Reset(Memory& memory) {
		PC = 0xFFFC;
		SP = 0xFF;
		C = Z = I = D = B = V = N = 0;
		A = X = Y = 0;
		memory.Init();
	}
    // Read byte from memory at Program Counter address and increment PC
	Byte FetchByte(s32& Cycles, const Memory& memory) {
		Byte data = memory[PC];
		PC++;
		Cycles--;
		return data;
	}

	// Read byte at given any Address
	Byte ReadByte(s32& Cycles, Word Address, Memory& memory) {
		Byte data = memory[Address];
		Cycles--;
		return data;
	}

	// Read byte at given any Address
	Word ReadWord(s32& Cycles, Word Address, Memory& memory) {
		Word data = memory[Address];
		data |= (memory[Address+1] << 8);
		
		Cycles -= 2;

		// if (PLATFORM_BIG_ENDIAN) {
	    // swapBytesInWord(data);
		// }

		return data;
	}

	// Read 2 bytes from memory at Program Counter address and increment pC
	Word FetchWord(s32& Cycles, const Memory& memory) {
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

	// Return actual number of cycles that were used
	s32 Execute(s32 Cycles, Memory& mem);

	// Address Mode: Zero Page
	Word GetAddressZeroPage(s32& Cycles, const Memory& memory);
	// Address Mode: Zero Page X
	Word GetAddressZeroPageX(s32& Cycles, const Memory& memory);
	// Address Mode: Zero Page Y
	Word GetAddressZeroPageY(s32& Cycles, const Memory& memory);
	// Address Mode: Absolute
	Word GetAddressAbsolute(s32& Cycles, const Memory& memory);
	// Address Mode: Absolute X
	Word GetAddressAbsoluteX(s32& Cycles, const Memory& memory);
	// Address Mode: Absolute Y
	Word GetAddressAbsoluteY(s32& Cycles, const Memory& memory);
};

