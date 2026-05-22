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
	struct StatusFlags;
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
};

struct mos6502::StatusFlags {
		Byte C : 1; // Status flag - Carry Flag
		Byte Z : 1; // Status flag - Zero Flag
		Byte I : 1; // Status flag - Interrupt Disable
		Byte D : 1; // Status flag - Decimal Mode
		Byte B : 1; // Status flag - Break Command
		Byte Unused : 1; // Status flag - Break Command
		Byte V : 1; // Status flag - Overflow Flag
		Byte N : 1; // Status flag - Negative Flag
};

class mos6502::CPU {
	public:
	Word PC; // Program Counter
	Byte SP; // Stack Pointer
	Byte A, X, Y; // Accumulator, X register, Y register

	union {
		Byte PS;
		mos6502::StatusFlags Flag;
	};
	 
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

		// JMP
		JMP_ABS = 0x4C,
		JMP_IND = 0x6C,

		// Stack Operations
		TSX = 0xBA,
		TXS = 0x9A,
		PHA = 0x48,
		PHP = 0x08,
		PLA = 0x68,
		PLP = 0x28,

		// AND
		AND_IM = 0x29,
		AND_ZP = 0x25,
		AND_ZP_X = 0x35,
		AND_ABS = 0x2D,
		AND_ABS_X = 0x3D,
		AND_ABS_Y = 0x39,
		AND_IND_X = 0x21,
		AND_IND_Y = 0x31,

		// ORA
		ORA_IM = 0x09,
		ORA_ZP = 005,
		ORA_ZP_X = 0x15,
		ORA_ABS = 0x0D,
		ORA_ABS_X = 0x1D,
		ORA_ABS_Y = 0x19,
		ORA_IND_X = 0x01,
		ORA_IND_Y = 0x11,

		// EOR
		EOR_IM = 0x49,
		EOR_ZP = 0x45,
		EOR_ZP_X = 0x55,
		EOR_ABS = 0x4D,
		EOR_ABS_X = 0x5D,
		EOR_ABS_Y = 0x59,
		EOR_IND_X = 0x41,
		EOR_IND_Y = 0x51,
		BIT_ZP = 0x24,
		BIT_ABS = 0x2C,

		JSR = 0x20,
		RTS = 0x60
		;


	void Reset(Word resetVector, Memory& memory) {
		PC = resetVector;
		SP = 0xFF;
		Flag.C = Flag.Z = Flag.I = Flag.D = Flag.B = Flag.V = Flag.N = 0;
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
	Word ReadWord(s32& Cycles, Word Address, const Memory& memory) {
		Word data = memory[Address];
		data |= (memory[Address+1] << 8);
		
		Cycles -= 2;

		// if (PLATFORM_BIG_ENDIAN) {
	    // swapBytesInWord(data);
		// }

		return data;
	}

	void WriteByte(Byte value, s32& cycles, Word address, Memory& memory) {
		memory[address] = value;
		cycles--;
	}

	void WriteWord(Word value, s32& Cycles, Word address, Memory& memory) {
		memory[address] = value & 0xFF;
		memory[address + 1] = (value >> 8);
		Cycles -= 2;
	}

	// Stack Pointer as a full 16-bit address in the 1st page
	Word SPToAddress () const {
		return 0x100 | SP;
	}

	// Push PC-1 onto the stack
	void PushPCToStack(s32& Cycles, Memory& memory) {
		WriteWord(PC - 1, Cycles, SPToAddress()-1, memory);
		SP -= 2;
	}

	void PushByteOntoStack(s32& Cycles, Byte value, Memory& memory) {
		memory[SPToAddress()] = value;
		SP--;
		Cycles -= 2;
	}

	Byte PopByteFromStack(s32& Cycles, Memory& memory) {
		SP++;
		const Word SPWord = SPToAddress();
		Byte value = memory[SPWord];
		Cycles -= 3;
		return value;
	}

	// Push PC-1 onto the stack
	Word PopWordFromStack(s32& Cycles, Memory& memory) {
		Word valueFromStack = ReadWord(Cycles, SPToAddress()+1, memory);
		SP += 2;
		Cycles--;
		return valueFromStack;
	}

	// Read 2 bytes from memory at Program Counter address and increment pC
	Word FetchWord(s32& Cycles, const Memory& memory) {
		Word data = memory[PC];
		PC++;
		
		data |= (memory[PC] << 8);
        PC++;
		
		Cycles -= 2;
		return data;
	}

	// Return actual number of cycles that were used
	s32 Execute(s32 Cycles, Memory& mem);

	// Load program in memory and return address where program was loaded into, or zero else
	Word LoadProgram(const Byte* program, u32 numberOfBytes, Memory& memory);

	// Print values of registers, program counter, stack pointer, flags
	void PrintStatus() const;

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
	// Address Mode: Absolute X always consuming 5 cycles (STA Absolute, X)
	Word GetAddressAbsoluteX_5(s32& Cycles, const Memory& memory);
	// Address Mode: Absolute Y
	Word GetAddressAbsoluteY(s32& Cycles, const Memory& memory);
	// Address Mode: Absolute Y always consuming 5 cycles (STA Absolute, Y)
	Word GetAddressAbsoluteY_5(s32& Cycles, const Memory& memory);
	// Address Mode: Indirect X | Indexed Indirect
	Word GetAddressIndirectX(s32& Cycles, const Memory& memory);
	// Address Mode: Indirect Y | Indirect Indexed
	Word GetAddressIndirectY(s32& Cycles, const Memory& memory);
	// Address Mode: Indirect Y | Indirect Indexed always consuming 6 cycles (STA Indirect, Y)
	Word GetAddressIndirectY_6(s32& Cycles, const Memory& memory);
};

