#pragma once

#include <cstdint>

class System
{
public:
    uint8_t cpu_ram[1024 * 64];
    uint8_t ppu_ram[1024 * 14];

    void loadRom(const char *filename);
};