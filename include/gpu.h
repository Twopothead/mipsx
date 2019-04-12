#pragma once
#include <inttypes.h>
#include "debug.h"
#include "iotools.h"
namespace GPU_Registers{
        const uint32_t BASE_GPU_Registers = 0x1f801810; 
        typedef union GPU_Registers{
            struct{
                uint32_t GP0;
                uint32_t GP1;
            };
            struct{
                uint32_t GPUREAD;
                uint32_t GPUSTAT;
            };
            uint32_t raw[2];
        }GPU_Registers_t;
        GPU_Registers_t gpu_regs;
        uint32_t read(uint32_t vaddr,int width){
            int base_addr = BASE_GPU_Registers;
            return RW::io_custom_read(vaddr,base_addr,gpu_regs.raw,width);
        };
        void write(uint32_t vaddr,uint32_t data,int width){
            int base_addr = BASE_GPU_Registers;
            RW::io_custom_write(vaddr,base_addr,gpu_regs.raw,data,width);
        }
}

namespace GPU
{
    
} // GPU
