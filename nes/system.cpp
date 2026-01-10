#include "system.h"

#include <fstream>
#include <vector>

void System::loadRom(const char *filename)
{
    std::ifstream file(filename, std::ios::binary);
    file.unsetf(std::ios::skipws);
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    std::vector<uint8_t> rom;
    rom.reserve(fileSize);

    // read the data:
    rom.insert(rom.begin(),
               std::istream_iterator<uint8_t>(file),
               std::istream_iterator<uint8_t>());

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
        ppu_ram[i]=rom[16+1024*16+i];
    }
}
