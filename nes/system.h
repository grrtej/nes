#pragma once

#include "types.h"

#include <vector>

class System
{
public:
	System(const char* filename);
	void cycle();

	std::vector<u8> cpu_ram;
	std::vector<u8> ppu_ram;
	u16 pc;
	u8 acc;
	u8 ps;
};