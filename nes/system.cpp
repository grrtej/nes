#include "system.h"

#include "types.h"
#include "instruction_metadata.h"

#include <fstream>
#include <iterator>
#include <vector>
#include <cstdio>
#include <print>
#include <format>
#include <stdexcept>

System::System(const char* filename) : cpu_ram(1024 * 64), ppu_ram(1024 * 14), acc{ 0 }, ps{ 0b00000100 }
{
	std::ifstream file(filename, std::ios::binary);
	file.unsetf(std::ios::skipws);

	std::streampos fileSize;
	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<u8> rom;
	rom.reserve(fileSize);
	rom.insert(rom.begin(), std::istream_iterator<u8>(file), std::istream_iterator<u8>());

	// mapper 0
	for (int i = 0; i < 1024 * 16; i++)
	{
		cpu_ram[0x8000 + i] = rom[16 + i];
	}
	for (int i = 0; i < 1024 * 16; i++)
	{
		cpu_ram[0xC000 + i] = rom[16 + i];
	}
	for (int i = 0; i < 1024 * 8; i++)
	{
		ppu_ram[i] = rom[16 + 1024 * 16 + i];
	}

	// set pc from reset vector
	pc = cpu_ram[0xfffd] << 8 | cpu_ram[0xfffc];
}

void System::cycle()
{
	// remove this soon, just a way to throttle loop
	static int cycle_count = 0;

	if (cycle_count < 4)
	{
		// fetch
		u8 op = cpu_ram[pc];
		// grab metadata
		const char* mnemonic = INSTRUCTION_METADATA_TABLE[op].name;
		int instruction_size = INSTRUCTION_METADATA_TABLE[op].size;

		if (instruction_size == -1) {
			// encountered an instruction outside the standard 6502 ISA
			// should be a good candidate for throwing an exception, right?
			// like what can i do? i cant just guess the behavior and keep chugging.
			// this should be handled by informing the user/dev.
			// however some ROMs might rely on UB, should implement the illegal op at some point
			throw std::runtime_error(std::format("illegal opcode 0x{:02X} at {:04X}", op, pc));
		}

		std::print(stderr, "{} ", mnemonic);

		switch (op)
		{
		case 0x78:
			ps |= 0b00000100; // ERROR: THIS IS WRONG TIMING! SHOULD BE DELAYED 1 INSTRUCTION!
			std::println(stderr, "%{:08b}", ps);
			break;
		case 0xa9:
			acc = cpu_ram[pc + 1];
			std::println(stderr, "${:02X}", acc);
			break;
		default:
			std::println(stderr, "<< NOT IMPLEMENTED >> ${:02X}", op);
			break;
		}

		pc += instruction_size;

		cycle_count++;
	}
}