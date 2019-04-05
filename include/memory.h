#pragma once
#include "memlayout.h"
#include <inttypes.h>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include "iomap.h"
#include "debug.h"
#include "cp0.h"
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


int fffffuck=4;
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
                return io_read( virtual_addr,sizeof(T)<<3 );//return IO_Map::io_read(normalizedAddress - 0x1f800000,32);   // pointer = (T*)&ioports[normalizedAddress - 0x1f800000];
                // do not use pointer here
        case 0x1ffe0130:
            pointer = (T *)&CacheCtrl;
            break;
        case 0x1f000000 ... 0x1f7fffff:// expansion region 1
            x__log("Read expansion region 1");
            return (T)0xffffffff;// it seems to be full-ones

        case 0x1fa00000 ... 0x1fbfffff:// expansion region 3
            x__log("Read expansion region 3");
            return (T)0xcafebabe;
        default:
            x__err("read error: normalizedAddress:%x",normalizedAddress);
            break;
        }
        return *pointer;
    }
    uint32_t read_wrapper(uint32_t virtual_addr,uint32_t width){
        switch (width)
        {
            case 32:// word
                return read<uint32_t>(virtual_addr);
                break;
            case 16:// half word
                return (uint32_t)read<uint16_t>(virtual_addr);
                break;
            case 8:// byte
                return (uint32_t)read<uint8_t>(virtual_addr);
                break;        
            default://default word 32
                return read<uint32_t>(virtual_addr);
                break;
        }

    }

// IsC
// Isolate Cache. If this bit is set, the data cache is “isolated” from main memory; that is, store operations
// modify the data cache but do not cause a main memory write to occur, and load operations return the data value
// from the cache whether or not a cache hit occurred. This bit is also useful in various operations such as flushing.
    template <typename T>
    void write(uint32_t virtual_addr, T data)
    {
        uint32_t normalizedAddress = virtual_addr & 0x1fffffff;
        T *pointer = nullptr;

        if(R3000_CP0::cp0_regs.SR.IsC==1){
            // x__err("cp0 Isolate Cache, ignore write to main memory");
            return;
        }
            
        // if(normalizedAddress==0x000000b0)
        //     if(!fffffuck--){
        //         x__err("virtual_addr%x data:%x",virtual_addr,data);
        //         while(1){
        //             ;
        //         }
                
        //     }
                // printf("ddddddddddddd:%x\n",*(uint32_t*)&real_main_Ram[0xb0]);

            
        switch (normalizedAddress)
        {
        case 0x00000000 ... 0x007ffffc:
            pointer = (T *)&real_main_Ram[normalizedAddress & ~0x00600000];
            break;
        case 0x1fc00000 ... 0x1fc7fffc:
            break;// ignore write to BIOS
        case 0x1f800000 ... 0x1f80fffc:
            io_write(virtual_addr,data,sizeof(T)<<3);// normalizedAddress - 0x1f800000
            //pointer = (T*)&ioports[normalizedAddress - 0x1f800000];
                return;// do not use pointer here
        case 0x1ffe0130:
            pointer = (T *)&CacheCtrl;
            break;
        default:
            x__err("write error: virtualaddr:%x,normalizedAddress:%x,data:%x ",virtual_addr,normalizedAddress,data);
            break;
        }

        *pointer = data;
        // printf("%x %x eeeeeeeeeeee:%x\n",normalizedAddress & ~0x00600000,data,*(uint32_t*)&real_main_Ram[0xb0]);
        
        return;
    }
    void write_wrapper(uint32_t virtual_addr,uint32_t data,uint32_t width){
        uint16_t half_word_data = 0x0000ffff & data;
        uint8_t byte_data = 0xff & data;
        switch (width)
        {
            case 32:// word
                write<uint32_t>(virtual_addr,data);
                break;
            case 16:// half word
                write<uint16_t>(virtual_addr,half_word_data);
                break;
            case 8:// byte
                write<uint8_t>(virtual_addr,byte_data);
                break;        
            default://default word 32
                write<uint32_t>(virtual_addr,data);
                break;
        }

    }

    friend class MIPSX_SYSTEM;
};

