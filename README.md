# MOS6502 Emulator (C++)

A simple and lightweight MOS 6502 CPU emulator written in modern C++
The project focuses on correctness, readability, and high test coverage using Google Test

## Features

- Emulation of the MOS 6502 CPU
- Supports most official opcodes
- Cycle-accurate behavior 
- High test coverage using Google Test (GTest)
- Cross-platform build system using CMake

## Requirements

- C++17 or newer
- CMake 3.15+
- Google Test

## Build Instructions

### Build

```
mkdir build
cd build
cmake ..
cmake --build .
```

### Run tests

```
mkdir build
cd build
cmake ..
cmake --build .
./build/MOS6502/6502Test/MoS6502Test
```

## Testing

The project uses Google Test for unit testing.

- Nearly full opcode coverage
- Edge cases and flag behavior tested

## Example Usage

```cpp
#include <m6502.h>
int main() {
	mos6502::Memory memory;
	mos6502::CPU cpu;
	cpu.Reset(0xFFFC, memory);

	mos6502::Byte prg = {... ENTER YOUR PROGRAM HERE AS OPCODES ...};

	Word startAddress = cpu.LoadProgram(prg, sizeof(prg) / sizeof(prg[0]), memory);
	cpu.PC = startAddress;

	for (mos6502::s32 clock = 10000; clock > 0;) {
		clock -= cpu.Execute(20, memory);
		cpu.PrintStatus();
	}
	return 0;
}
```


## License

MIT License

## Contributing

Contributions are welcome. Feel free to open issues or submit pull requests.