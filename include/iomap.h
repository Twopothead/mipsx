#pragma once
#include <inttypes.h>
#include "debug.h"
#include "timers.h"
#include "dma.h"
#include "iotools.h"
#include "spu.h"
#include "gpu.h"
namespace Memory_Control_1{
        const uint32_t BASE_Memory_Control_1 = 0x1f801000;
        typedef union Memory_Control_1{
            struct {
                uint32_t Expansion_1_Base_Address;
                uint32_t Expansion_2_Base_Address;
                uint32_t Expansion_1_Delay;
                uint32_t Expansion_3_Delay;
                uint32_t BIOS_ROM_Delay;
                uint32_t SPU_DELAY;
                uint32_t CDROM_DELAY;
                uint32_t Expansion_2_Delay;
                uint32_t COM_DELAY;
            };
            uint32_t raw[9];
        }Memory_Control_1_t;
        Memory_Control_1_t memctrl1;
        uint32_t read(uint32_t vaddr,int width){
            int base_addr = BASE_Memory_Control_1;
            return RW::io_custom_read(vaddr,base_addr,memctrl1.raw,width);
        }
        void write(uint32_t vaddr,uint32_t data,int width){
            int base_addr = BASE_Memory_Control_1;
            RW::io_custom_write(vaddr,base_addr,memctrl1.raw,data,width);
        }
}

namespace Memory_Control_2{
        uint32_t RAM_SIZE;
        const uint32_t BASE_RAM_SIZE = 0x1f801060;
        uint32_t read(uint32_t vaddr,int width){
            int base_addr = BASE_RAM_SIZE;
            return RW::io_custom_read(vaddr,base_addr,&RAM_SIZE,width);
        };
        void write(uint32_t vaddr,uint32_t data,int width){
            int base_addr = BASE_RAM_SIZE;
            RW::io_custom_write(vaddr,base_addr,&RAM_SIZE,data,width);
        }
}



namespace Interrupt_Control{
        const uint32_t BASE_Interrupt_Control = 0x1f801070;
        typedef union Interrupt_Control{
            struct {
                uint32_t I_STAT;
                uint32_t I_MASK;
            };
            uint32_t raw[2];   
        }Interrupt_Control_t;
        Interrupt_Control_t interrupt_ctrl;
        uint32_t read(uint32_t vaddr,int width){
            int base_addr = BASE_Interrupt_Control;
            return RW::io_custom_read(vaddr,base_addr,interrupt_ctrl.raw,width);
        };
        void write(uint32_t vaddr,uint32_t data,int width){
            int base_addr = BASE_Interrupt_Control;
            RW::io_custom_write(vaddr,base_addr,interrupt_ctrl.raw,data,width);
        }
}

// namespace MDEC_Registers{
//         typedef union MDEC_Registers{
//             struct {
//                 /* data */
//             };
            
//         };
        
// }

uint32_t io_read(uint32_t vaddr,int width){
        uint32_t data = 0;
        switch (vaddr)
        {
            case 0x1f801000 ... 0x1f801020:
                data = Memory_Control_1::read(vaddr,width);
                break;
            case 0x1f801080 ... 0x1f8010ff:
                data = DMA::read(vaddr,width);
                break;
            case 0x1f801060:
                data = Memory_Control_2::read(vaddr,width);
                break;
            case 0x1f801070 ... 0x1f801074:
                data = Interrupt_Control::read(vaddr,width);
                break;
            case 0x1f801100 ... 0x1f80110f:// Timer 0
            case 0x1f801110 ... 0x1f80111f:// Timer 1
            case 0x1f801120 ... 0x1f80112f:// Timer 2
                data = PSX_Timer::read(vaddr,width);
                break;

            case 0x1f801810 ... 0x1f801814:
                data = GPU::read(vaddr,width);
                break;
            // case 0x1f802041 ... 0x1f802042:
            //     ;//ignore Expansion Region 2 - Int/Dip/Post
            //     break;
            case 0x1f802000 ... 0x1f802fff:// expansion region 2
                x__log("Read expansion region 2");
                return 0xcafebabe;
            case 0x1f801c00 ... 0x1f801dff:
                data = SPU::read(vaddr,width);
                break;
                // printf("spu \t");
            // case 0x1f801d80 ... 0x1f801dbc:
            //     data = SPU_Control_Registers::read(vaddr,width);
            //     break;
            default:
                x__err("io read error:%x ",vaddr);
                break;
        }
        return data;
}

void io_write(uint32_t vaddr,uint32_t data,int width){
        //         if(vaddr==0x1f801114){
        //     x__err("%d %x",mipsx_cycle,data);
        //     // while(1){
        //     //     ;/* code */
        //     // }
        // }
        // if(vaddr==0x1f8010f4)
        //     x__err("%x",Cycle::cycle);
        switch (vaddr)
        {
            case 0x1f801000 ... 0x1f801020:
                Memory_Control_1::write(vaddr,data,width);
                break;
            case 0x1f801060:
                Memory_Control_2::write(vaddr,data,width);
                break;
            case 0x1f801070 ... 0x1f801074:
                Interrupt_Control::write(vaddr,data,width);
                break;
            case 0x1f801080 ... 0x1f8010ff:
                DMA::write(vaddr,data,width);
                break;
            case 0x1f801100 ... 0x1f80110f:// Timer 0
            case 0x1f801110 ... 0x1f80111f:// Timer 1
            case 0x1f801120 ... 0x1f80112f:// Timer 2
                PSX_Timer::write(vaddr,data,width);
                break;
            case 0x1f801810 ... 0x1f801814:
                GPU::write(vaddr,data,width);
                break;
            case 0x1f801c00 ... 0x1f801dff:
                data = SPU::read(vaddr,width);
                break;
            // case 0x1f801d80 ... 0x1f801dbc:
            //     SPU_Control_Registers::write(vaddr,data,width);
            //     break;
            case 0x1f802041 ... 0x1f802042:
                ;//ignore Expansion Region 2 - Int/Dip/Post
                break;
            default:
                x__err("io write error: addr:%x data:%x",vaddr,data);
                break;
        }   
}