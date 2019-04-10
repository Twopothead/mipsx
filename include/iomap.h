#pragma once
#include <inttypes.h>
#include "debug.h"
#include "timers.h"
#include "dma.h"
namespace RW{
    void io_custom_write(uint32_t addr,uint32_t base_addr,uint32_t *pbase,uint32_t data,int width){
            uint32_t offset = addr - base_addr;//addr - 0x1f801d80;
            uint32_t *pword = pbase;//spu_ctrl_regs.raw;
            uint16_t *phalf_word = (uint16_t*)pbase;//spu_ctrl_regs.raw;
            uint8_t *pbyte = (uint8_t*)pbase;//spu_ctrl_regs.raw;
            switch (width)
            {
                case 32:
                    pword += offset>>2;//offset/4;   (32/8=4)
                    *pword = (uint32_t)data;
                    break;
                case 16:
                    phalf_word += offset>>1;//offset/2; (16/8=2)
                    *phalf_word = (uint16_t)(data & 0x0000ffff); 
                    break;
                case 8:
                    pbyte += offset;//offset/1; (8/8=1)
                    *pbyte = (uint8_t)(data & 0xff);
                    break;            
                default:
                    pword += offset>>2;
                    *pword = (uint32_t)data;
                    break;
            }
            return;
    }

    uint32_t io_custom_read(uint32_t addr,uint32_t base_addr,uint32_t *pbase,int width){
            uint32_t offset = addr - base_addr;//addr - 0x1f801d80;
            uint32_t *pword = pbase;//spu_ctrl_regs.raw;
            uint16_t *phalf_word = (uint16_t*)pbase;//spu_ctrl_regs.raw;
            uint8_t *pbyte = (uint8_t*)pbase;//spu_ctrl_regs.raw;
            uint32_t data = 0;
            switch (width)
            {
                case 32:
                    pword += offset>>2;//offset/4;   (32/8=4)
                    data = *pword;
                    break;
                case 16:
                    phalf_word += offset>>1;//offset/2; (16/8=2)
                    data = *phalf_word; 
                    break;
                case 8:
                    pbyte += offset;//offset/1; (8/8=1)
                    data = *pbyte;
                    break;            
                default:
                    pword += offset>>2;
                    data =*pword;
                    break;
            }
            return data;
    }
}


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
            RW::io_custom_read(vaddr,base_addr,memctrl1.raw,width);
        };
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
            RW::io_custom_read(vaddr,base_addr,&RAM_SIZE,width);
        };
        void write(uint32_t vaddr,uint32_t data,int width){
            int base_addr = BASE_RAM_SIZE;
            RW::io_custom_write(vaddr,base_addr,&RAM_SIZE,data,width);
        }
}

namespace SPU_Control_Registers{
        const uint32_t BASE_SPU_Control_Registers = 0x1f801d80;
        typedef union SPU_Control_Registers{
            struct{
                uint32_t  Main_Volume_Left_Right;
                uint32_t  Reverb_Output_Volume_Left_Right;
                uint32_t  Voice_0to23_Key_ON_Start_Attack_Decay_Sustain_W;
                uint32_t  Voice_0to23_Key_OFF_Start_Release_W;
                uint32_t  Voice_0to23_Channel_FM_pitch_lfo_mode_R_W;
                uint32_t  Voice_0to23_Channel_Noise_mode_R_W;
                uint32_t  Voice_0to23_Channel_Reverb_mode_R_W;
                uint32_t  Voice_0to23_Channel_ON_OFF_status_R;
                uint16_t  Unknown_R_or_W;
                uint16_t  Sound_RAM_Reverb_Work_Area_Start_Address;
                uint16_t  Sound_RAM_IRQ_Address;
                uint16_t  Sound_RAM_Data_Transfer_Address;
                uint16_t  Sound_RAM_Data_Transfer_Fifo;
                uint16_t  SPU_Control_Register_SPUCNT;
                uint16_t  Sound_RAM_Data_Transfer_Control;
                uint16_t  SPU_Status_Register_SPUSTAT_R;
                uint32_t  CD_Volume_Left_Right;
                uint32_t  Extern_Volume_Left_Right;
                uint32_t  Current_Main_Volume_Left_Right;
                uint32_t  Unknown_R_W;
            };
            uint32_t raw[16];
        }SPU_Control_Registers_t;
        SPU_Control_Registers_t spu_ctrl_regs;
        uint32_t read(uint32_t vaddr,int width){
            int base_addr = BASE_SPU_Control_Registers;
            RW::io_custom_read(vaddr,base_addr,spu_ctrl_regs.raw,width);
        };
        void write(uint32_t vaddr,uint32_t data,int width){
            int base_addr = BASE_SPU_Control_Registers;
            RW::io_custom_write(vaddr,base_addr,spu_ctrl_regs.raw,data,width);
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
            RW::io_custom_read(vaddr,base_addr,interrupt_ctrl.raw,width);
        };
        void write(uint32_t vaddr,uint32_t data,int width){
            int base_addr = BASE_Interrupt_Control;
            RW::io_custom_write(vaddr,base_addr,interrupt_ctrl.raw,data,width);
        }
}
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
            RW::io_custom_read(vaddr,base_addr,gpu_regs.raw,width);
        };
        void write(uint32_t vaddr,uint32_t data,int width){
            int base_addr = BASE_GPU_Registers;
            RW::io_custom_write(vaddr,base_addr,gpu_regs.raw,data,width);
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
        width = 32;
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

            case 0x1f801810 ... 0x1f801810:
                data = GPU_Registers::read(vaddr,width);
                break;
            // case 0x1f802041 ... 0x1f802042:
            //     ;//ignore Expansion Region 2 - Int/Dip/Post
            //     break;
            case 0x1f802000 ... 0x1f802fff:// expansion region 2
                x__log("Read expansion region 2");
                return 0xcafebabe;
            case 0x1f801d80 ... 0x1f801dbc:
                data = SPU_Control_Registers::read(vaddr,width);
                break;
            default:
                x__err("io read error:%x ",vaddr);
                break;
        }
        return data;
}

void io_write(uint32_t vaddr,uint32_t data,int width){
        width = 32;
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
            case 0x1f801810 ... 0x1f801810:
                GPU_Registers::write(vaddr,data,width);
                break;
            case 0x1f801d80 ... 0x1f801dbc:
                SPU_Control_Registers::write(vaddr,data,width);
                break;
            case 0x1f802041 ... 0x1f802042:
                ;//ignore Expansion Region 2 - Int/Dip/Post
                break;
            default:
                x__err("io write error: addr:%x data:%x",vaddr,data);
                break;
        }   
}