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

// utility functions
namespace {
	void set_n_and_z(u8& ps, u8 v) {
		ps &= ~0b10000010; // clear N and Z
		ps |= v & 0b10000000; // copy v7 to N
		ps |= static_cast<u8>(v == 0) << 1; // copy v==0 to Z
	}
}

// system init will need rework at some point
System::System(const char* filename)
	: cpu_ram(1024 * 64), ppu_ram(1024 * 14),
	acc{ 0 }, ps{ 0b00000100 }, ix{ 0 }, sp{ 0xFD }
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

	if (cycle_count < 9)
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
		case 0x78: // SEI
			ps |= 0b00000100; // ERROR: THIS IS WRONG TIMING! SHOULD BE DELAYED 1 INSTRUCTION!
			std::println(stderr, "[PS: %{:08b}]", ps);
			break;
		case 0xd8: // CLD
			ps &= 0b11110111;
			std::println(stderr, "[PS: %{:08b}]", ps);
			break;
		case 0xa9: // LDA #
		{
			u8 v = cpu_ram[pc + 1];
			acc = v;

			set_n_and_z(ps, acc);

			std::println(stderr, "${:02X} [PS: %{:08b}]", acc, ps);
			break;
		}
		case 0x8d: // STA abs
		{
			u16 addr = cpu_ram[pc + 2] << 8 | cpu_ram[pc + 1]; // le
			cpu_ram[addr] = acc;
			std::println(stderr, "${:04X} [CPU_RAM[{:04X}]: ${:02X}]", addr, addr, cpu_ram[addr]);
			break;
		}
		case 0xa2: // LDX #
		{
			u8 v = cpu_ram[pc + 1];
			ix = v;

			set_n_and_z(ps, ix);

			std::println(stderr, "${:02X} [PS: %{:08b}]", ix, ps);
			break;
		}
		case 0x9a: // TXS
			sp = ix;
			std::println(stderr, "[SP: ${:02X}]", sp);
			break;
		case 0xad: // LDA abs
		{
			u16 addr = cpu_ram[pc + 2] << 8 | cpu_ram[pc + 1];
			u8 v = cpu_ram[addr];
			acc = v;

			set_n_and_z(ps, acc);

			std::println(stderr, "{:04X} [${:02X}]", addr, acc);
			break;
		}
		case 0x29: // AND #
		{
			u8 v = cpu_ram[pc + 1];
			acc &= v;

			set_n_and_z(ps, acc);

			std::println(stderr, "${:02X}", v);
			break;
		}
		default:
			std::println(stderr, "<< NOT IMPLEMENTED >> ${:02X}", op);
			break;
		}

		pc += instruction_size;

		cycle_count++;
	}
}