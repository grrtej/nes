#include "system.h"

#include "types.h"

#include <fstream>
#include <vector>

System::System(const char* filename) : cpu_ram(1024 * 64), ppu_ram(1024 * 14)
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

}