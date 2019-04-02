#pragma once
#include "memlayout.h"
#include <inttypes.h>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include "iomap.h"
namespace BIOS
{
    void load_ROM(uint8_t *rom_p, const char *psx_bios_path)
    {
        FILE *SCPH1001_fn;
        SCPH1001_fn = fopen(psx_bios_path, "rb");
        if (SCPH1001_fn)
            fread(rom_p, 1, PlayStationMemLayout::mem_ROM_size, SCPH1001_fn);
        fclose(SCPH1001_fn);
    }
} 


class PlayStationMemory
{

    uint8_t *real_main_Ram;
    uint8_t *bios_Rom;
    uint8_t *expansion1;
    uint32_t CacheCtrl;

  public:
    PlayStationMemory()
    {
        using namespace PlayStationMemLayout;
        real_main_Ram = new uint8_t[mem_RAM_size];
        bios_Rom = new uint8_t[mem_ROM_size];
        expansion1 = new uint8_t[mem_expansion1_size];
        memset(real_main_Ram, 0, mem_RAM_size);
        memset(bios_Rom, 0, mem_ROM_size);
        memset(expansion1, 0, mem_expansion1_size);
        BIOS::load_ROM(bios_Rom, "../bios/SCPH1001.BIN");
    }
    template <typename T>
    T read(uint32_t virtual_addr)
    {
        uint32_t normalizedAddress = virtual_addr & 0x1fffffff;
        T *pointer = nullptr;
        switch (normalizedAddress)
        {
        case 0x00000000 ... 0x007ffffc:
            pointer = (T *)&real_main_Ram[normalizedAddress & ~0x00600000];
            break;
        case 0x1fc00000 ... 0x1fc7fffc:
            pointer = (T *)&bios_Rom[normalizedAddress - 0x1fc00000];
            break;
        case 0x1f800000 ... 0x1f80fffc:
                return IO_Map::io_read(normalizedAddress - 0x1f800000,32);   // pointer = (T*)&ioports[normalizedAddress - 0x1f800000];
                // do not use pointer here
        case 0x1ffe0130:
            pointer = (T *)&CacheCtrl;
            break;
        default:
            x__err("read error: normalizedAddress:%x",normalizedAddress);
            break;
        }
        return *pointer;
    }
    template <typename T>
    void write(uint32_t virtual_addr, T data)
    {
        uint32_t normalizedAddress = virtual_addr & 0x1fffffff;
        T *pointer = nullptr;
        switch (normalizedAddress)
        {
        case 0x00000000 ... 0x007ffffc:
            pointer = (T *)&real_main_Ram[normalizedAddress & ~0x00600000];
            break;
        case 0x1fc00000 ... 0x1fc7fffc:
            break;
        case 0x1f800000 ... 0x1f80fffc:
            IO_Map::io_write(normalizedAddress - 0x1f800000,data,32);//pointer = (T*)&ioports[normalizedAddress - 0x1f800000];
                return;// do not use pointer here
        case 0x1ffe0130:
            pointer = (T *)&CacheCtrl;
            break;
        default:
            x__err("write error: normalizedAddress:%x,data:%x ",normalizedAddress,data);
            break;
        }

        *pointer = data;
        return;
    }
};

