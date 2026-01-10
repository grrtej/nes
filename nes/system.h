#pragma once

#include <cstdint>
#include <vector>

class System
{
public:
	System(const char* filename);
	std::vector<uint8_t> cpu_ram;
	std::vector<uint8_t> ppu_ram;
};