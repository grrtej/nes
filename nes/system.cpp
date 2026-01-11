#include "system.h"

#include "types.h"

#include <fstream>
#include <iterator>
#include <vector>
#include <cstdio>
#include <print>

System::System(const char* filename) : cpu_ram(1024 * 64), ppu_ram(1024 * 14), acc{ 0 }
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

	while (cycle_count < 4)
	{
		// fetch
		u8 op = cpu_ram[pc];

		switch (op)
		{
		case 0x78:
			std::println(stderr, "SEI");
			pc += 1;
			break;
		case 0xd8:
			std::println(stderr, "CLD");
			pc += 1;
			break;
		case 0xa9:
			acc = cpu_ram[pc + 1];
			std::println(stderr, "LDA ${:02X}", acc);
			pc += 2;
			break;
		default:
			std::println(stderr, "NOT IMPLEMENTED ${:02X}", op);
			break;
		}

		cycle_count++;
	}
}