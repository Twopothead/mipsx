#pragma once
#include <inttypes.h>
#include "debug.h"
#include "iotools.h"
// The SPU occupies the area $1f801c00-$1f801dff. All registers are 16 bit wide
// The SPU is the unit responsible for all aural capabilities of the psx. It
// handles 24 voices, has a 512kb sound buffer
struct SPU_Voice_t{
    

}voice[24];
namespace SPU_Control_Registers{// SPU Control Registers
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
            return RW::io_custom_read(vaddr,base_addr,spu_ctrl_regs.raw,width);
        };
        void write(uint32_t vaddr,uint32_t data,int width){
            int base_addr = BASE_SPU_Control_Registers;
            RW::io_custom_write(vaddr,base_addr,spu_ctrl_regs.raw,data,width);
        }   
}

namespace SPU_Voice_Registers{// SPU Voice 0..23 Registers

// 0x1f801c00 0x1f802000
// 0x1f801d7f 
// switch (address) {
//     case 0x1f801c00 ... 0x1f801d7f:
//         voice = &spu.voice[(address >> 4) & 0x1f];
//         offset = address & 0xf;
//         return spu_voice_read16(voice, offset);
//     case 0x1f801d88:
//         return spu.key_on;
//     case 0x1f801d8a:
//         return spu.key_on >> 16;
//     case 0x1f801d8c:
//         return spu.key_off;
//     case 0x1f801d8e:
//         return spu.key_off >> 16;
//     case 0x1f801daa:
//         return spu.control;
//     case 0x1f801dac:
//         return spu.data_transfer.control;
//     case 0x1f801dae:
//         return spu.status;
//     }
}

namespace SPU{
        uint16_t spumem[1024 * 256];/* 512KB All registers are 16 bit wide */
        const uint32_t SPU_BASE = 0x1f801c00;
        uint32_t read(uint32_t vaddr,int width){
            uint32_t data = 0;
            switch (vaddr)
            {
                case 0x1f801d80 ... 0x1f801dbc:
                    data = SPU_Control_Registers::read(vaddr,width);
                    break;
                case 0x1f801c00 ... 0x1f801d7f:
                    break;
//         voice = &spu.voice[(address >> 4) & 0x1f];
//         offset = address & 0xf;
//         return spu_voice_read16(voice, offset);            
                default:
                    data = RW::io_custom_read(vaddr,SPU_BASE,(uint32_t*)spumem,width);
                    break;
            }
            return data;
        };
        void write(uint32_t vaddr,uint32_t data,int width){
            switch (vaddr)
            {
                case 0x1f801d80 ... 0x1f801dbc:
                    SPU_Control_Registers::write(vaddr,data,width);
                    break;
            
                default:
                    RW::io_custom_write(vaddr,SPU_BASE,(uint32_t *)spumem,data,width);
                    break;
            }
            return;
        }   
} 
