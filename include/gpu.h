#pragma once
#include <inttypes.h>
#include "debug.h"
#include "iotools.h"
#include <vector>   
#include "bitwise.h"
#include "gp0.h"
#include "gp1.h"

namespace GPU{
        using namespace Bitwise;
        const uint32_t BASE_GPU_Registers = 0x1f801810;
        
        // std::vector<uint16_t> transfer;       
        
        uint32_t gpu_stat(){
        //  19    Vertical Resolution         (0=240, 1=480, when Bit22=1)  ;GP1(08h).2
        //  26    Ready to receive Cmd Word   (0=No, 1=Ready)  ;GP0(...) ;via GP0
        //  27    Ready to send VRAM to CPU   (0=No, 1=Ready)  ;GP0(C0h) ;via GPUREAD
        //  28    Ready to receive DMA Block  (0=No, 1=Ready)  ;GP0(...) ;via GP0
            GP1._Reverseflag = 0;//bit 14: the Nocash spec says that when this bit is set on the real hardware just messes up the display in a weird way.
            GP1.ready_for_cmd = 1;
            GP1.ready_for_vram = 1;
            GP1.ready_for_dma = 1;
            return (GP1.raw & ~0x00080000) | 0x1c002000; 
        }
        uint32_t gpu_data(){
            uint32_t lower;
            uint32_t upper;

            return 0;
        }
        // typedef union GPU_Registers{
        //     struct{
        //         uint32_t GP0;
        //         uint32_t GP1;
        //     };
        //     struct{
        //         uint32_t GPUREAD;
        //         uint32_t GPUSTAT;
        //     };
        //     uint32_t raw[2];
        // }GPU_Registers_t;
        // GPU_Registers_t gpu_regs;
        
        uint32_t read(uint32_t vaddr,int width){
            switch (vaddr)
            {
                case 0x1f801810:
                    return gpu_data();
                case 0x1f801814:
                    return gpu_stat();
                default:
                    return 0;
            }
        };

        void WriteGP0(uint32_t data){
            cmd.push_back(data);
            switch ( cmd[0]>>24 & 0xff)
            {
//  GP0(00h)                 - Nop?
                case 0x00: // nop
                    GP1.raw = 0x14802000;// Accordingly, GPUSTAT becomes 14802000h.
                    cmd.erase(cmd.begin());
                    break;
                case 0x01: // GP0(01h) - Clear Cache
                    cmd.clear();
                    break;
                case 0x02: // GP0(02h) - Fill Rectangle in VRAM
                    // Rect 
                    cmd.clear();
                    break;
// GP0(20h..3Fh)            - Render Polygons
                case 0x20:// GP0(20h) - Monochrome three-point polygon, opaque
                case 0x22:// GP0(22h) - Monochrome three-point polygon, semi-transparent
                    // POLY_F3
                    cmd.clear();
                    break;

                case 0x28:// GP0(28h) - Monochrome four-point polygon, opaque
                case 0x2a:// GP0(2Ah) - Monochrome four-point polygon, semi-transparent
                    // POLY_F4
                    cmd.clear();
                    break;

                case 0x2c:// GP0(2Ch) - Textured four-point polygon, opaque, texture-blending
                    //
                    cmd.clear();
                    break;
                case 0x2d:// GP0(2Dh) - Textured four-point polygon, opaque, raw-texture
                    //
                    cmd.clear();
                case 0x2e:// GP0(2Eh) - Textured four-point polygon, semi-transparent, texture-blending
                    //
                    cmd.clear();
                case 0x2f:// GP0(2Fh) - Textured four-point polygon, semi-transparent, raw-texture
                    //
                    cmd.clear();

                case 0x40:// GP0(40h) - Monochrome line, opaque

                case 0x42:// GP0(42h) - Monochrome line, semi-transparent
                    // 
                    cmd.clear();
                    break;

                case 0xa0: // GP0(A0h) - Copy Rectangle (CPU to VRAM)
                    // 
                    cmd.clear();
                    break;


                // Draw mode/environment setting packets
                case 0xe1: // 0xe1 draw mode setting
                    GP0_CMDS::E1_draw_mode_setting(data);
                    cmd.clear();
                    break;
                case 0xe2: // 0xe2 texture window setting
                    GP0_CMDS::E2_texture_window_setting(data);
                    cmd.clear();
                    break;
                case 0xe3: // 0xe3 set drawing area top left
                    GP0_CMDS::E3_set_drawing_area_top_left(data);
                    cmd.clear();
                    break;
                case 0xe4: // 0xe4 set drawing area bottom right
                    GP0_CMDS::E4_set_drawing_area_bottom_right(data);
                    cmd.clear();
                    break;
                case 0xe5: // 0xe5 drawing offset
                    GP0_CMDS::E5_drawing_offset(data);
                    cmd.clear();
                    break;
                case 0xe6: // 0xe6 mask setting
                    GP0_CMDS::E6_mask_setting(data);
                    cmd.clear();
                    break;
                default:
                    cmd.erase(cmd.begin());
                    break;
            }

        }
        void WriteGP1(uint32_t data){
            using namespace GP1_CMDS;
            switch ( data>>24 & 0xff )
            {
                case 0x00:// GP1(00h) - Reset GPU
                    GP1_00h_resetGPU(data);
                    break;
                case 0x01:// GP1(01h) - Reset Command Buffer
                    GP1_01h_reset_cmd_buffer(data);
                    break;
                case 0x02:// GP1(02h) - Acknowledge GPU Interrupt (IRQ1)
                    GP1_02h_ackGPUirq(data);
                    break;
                case 0x03:// GP1(03h) - Display Enable
                    GP1_03h_display_enable(data);
                    break;
                case 0x04:// GP1(04h) - DMA Direction / Data Request
                    GP1_04h_dma_dir_req(data);
                    break;
                case 0x05:// GP1(05h) - Start of Display area (in VRAM)
                    GP1_05h_set_display_offset(data);
                    break;
                case 0x06:// GP1(06h) - Horizontal Display range (on Screen)
                    GP1_06h_set_horizontal_display_range(data);
                    break;
                case 0x07:// GP1(07h) - Vertical Display range (on Screen)
                    GP1_07h_set_vertical_display_range(data);
                    break;
                case 0x08:// GP1(08h) - Display mode
                    GP1_08h_set_display_mode(data);
                    break;
                case 0x09:// GP1(09h) - New Texture Disable
                    break;
                case 0x10:// GP1(10h) - Get GPU Info
                    break;
                case 0x11 ... 0x1f: // GP1(11h..1Fh) - Mirrors of GP1(10h), Get GPU Info
                    break;
                case 0x20:// GP1(20h) - Special/Prototype Texture Disable
                    break;
                case 0x0b:// GP1(0Bh) - Unknown/Internal?
                    break;
                case 0x0a:// GP1(0Ah,0Ch..0Fh,21h..3Fh) - N/A Not used?
                    break;

                default:
                    break;
            }
        }
        void WriteList(uint32_t *ptr,uint32_t size){
            while(size--){
                WriteGP0( *ptr++ );
            }
        }
        void resetGPU(){
            // GP1(00h) - Reset GPU
            // 0-23  Not used (zero)
            // Resets the GPU to the following values:
            // GP1(01h)      ;clear fifo
            // GP1(02h)      ;ack irq (0)
            // GP1(03h)      ;display off (1)
            // GP1(04h)      ;dma off (0)
            // GP1(05h)      ;display address (0)
            // GP1(06h)      ;display x1,x2 (x1=200h, x2=200h+256*10)
            // GP1(07h)      ;display y1,y2 (y1=010h, y2=010h+240)
            // GP1(08h)      ;display mode 320x200 NTSC (0)
            // GP0(E1h..E6h) ;rendering attributes (0)
            // Accordingly, GPUSTAT becomes 14802000h. The x1,y1 values are too small, ie. the upper-left edge isn't visible. Note that GP1(09h) is NOT affected by the reset command.
            GP1.raw = 0x14802000;
        }
        void write(uint32_t vaddr,uint32_t data,int width){
            switch (vaddr)
            {
                case 0x1f801810:
                    WriteGP0(data);// We’ll have to decode those command like we decoded the CPU instructions.
                    break;
                case 0x1f801814:
                    WriteGP1(data);// We’ll have to decode those command like we decoded the CPU instructions.
                    break;
                default:
                    return;
            }
        }

}


