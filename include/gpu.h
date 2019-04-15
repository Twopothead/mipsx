#pragma once
#include <inttypes.h>
#include "debug.h"
#include "iotools.h"
#include <vector>   



namespace GPU{

typedef union 
{// GPU Status Register
    struct  // 1F801814h - GPUSTAT - GPU Status Register (R)
    {
        uint32_t tpagex:4;              //  0-3   Texture page X Base   (N*64)                              ;GP0(E1h).0-3
        uint32_t tpagey:4;              //   4     Texture page Y Base   (N*256) (ie. 0 or 256)              ;GP0(E1h).4
        uint32_t transparency:1;        //   5-6   Semi Transparency     (0=B/2+F/2, 1=B+F, 2=B-F, 3=B+F/4)  ;GP0(E1h).5-6
        uint32_t tpagecolor:2;          //   7-8   Texture page colors   (0=4bit, 1=8bit, 2=15bit, 3=Reserved)GP0(E1h).7-8
        uint32_t dither:1;              //   9     Dither 24bit to 15bit (0=Off/strip LSBs, 1=Dither Enabled);GP0(E1h).9
        uint32_t draw_to_display:1;     //   10    Drawing to display area (0=Prohibited, 1=Allowed)         ;GP0(E1h).10
        uint32_t apply_mask:1;          //   11    Set Mask-bit when drawing pixels (0=No, 1=Yes/Mask)       ;GP0(E6h).0
        uint32_t no_draw_over_mask:1;   //   12    Draw Pixels           (0=Always, 1=Not to Masked areas)   ;GP0(E6h).1
        uint32_t _InterlaceField:1;
        uint32_t _Reverseflag:1;
        uint32_t _TextureDisable:1;
//   13    Interlace Field       (or, always 1 when GP1(08h).5=0)
//   14    "Reverseflag"         (0=Normal, 1=Distorted)             ;GP1(08h).7
//   15    Texture Disable       (0=Normal, 1=Disable Textures)      ;GP0(E1h).11
        uint32_t width1:1;              //   16    Horizontal Resolution 2     (0=256/320/512/640, 1=368)    ;GP1(08h).6
        uint32_t width0:2;              //   17-18 Horizontal Resolution 1     (0=256, 1=320, 2=512, 3=640)  ;GP1(08h).0-1
        uint32_t height:1;              //   19    Vertical Resolution         (0=240, 1=480, when Bit22=1)  ;GP1(08h).2
        uint32_t video_mode:1;          //   20    Video Mode                  (0=NTSC/60Hz, 1=PAL/50Hz)     ;GP1(08h).3
        uint32_t use_24bpp;             //   21    Display Area Color Depth    (0=15bit, 1=24bit)            ;GP1(08h).4
        uint32_t interlacing:1;         //   22    Vertical Interlace          (0=Off, 1=On)                 ;GP1(08h).5
        uint32_t display_enable:1;      //   23    Display Enable              (0=Enabled, 1=Disabled)       ;GP1(03h).0
        uint32_t irq:1;                 //   24    Interrupt Request (IRQ1)    (0=Off, 1=IRQ)       ;GP0(1Fh)/GP1(02h)
        uint32_t dma:1;
//   25    DMA / Data Request, meaning depends on GP1(04h) DMA Direction:
//           When GP1(04h)=0 ---> Always zero (0)
//           When GP1(04h)=1 ---> FIFO State  (0=Full, 1=Not Full)
//           When GP1(04h)=2 ---> Same as GPUSTAT.28
//           When GP1(04h)=3 ---> Same as GPUSTAT.27
        uint32_t ready_for_cmd:1;       //   26    Ready to receive Cmd Word   (0=No, 1=Ready)  ;GP0(...) ;via GP0
        uint32_t ready_for_vram:1;      //   27    Ready to send VRAM to CPU   (0=No, 1=Ready)  ;GP0(C0h) ;via GPUREAD
        uint32_t ready_for_dma:1;       //   28    Ready to receive DMA Block  (0=No, 1=Ready)  ;GP0(...) ;via GP0
        uint32_t dma_direction:2;       //   29-30 DMA Direction (0=Off, 1=?, 2=CPUtoGP0, 3=GPUREADtoCPU)    ;GP1(04h).0-1
        uint32_t draw_even_lines:1;     //   31    Drawing even/odd lines in interlace mode (0=Even or Vblank, 1=Odd)
    };
    uint32_t raw = 0x14802000; // GP1(00h) - Reset GPU GPUSTAT becomes 14802000h. ;
}GPUstat_t;

        const uint32_t BASE_GPU_Registers = 0x1f801810;
        GPUstat_t GP1;
        uint32_t data_latch = 0;
        std::vector<unsigned int> cmd;// It (=GP0 only?) has a 64-byte (16-word) command FIFO buffer.
        // std::vector<uint16_t> transfer;
        typedef union {
            struct 
            {
                uint16_t r : 5;
                uint16_t g : 5;
                uint16_t b : 5;
                uint16_t msb :1;
            };
            struct 
            {
                uint16_t left  : 8;
                uint16_t right : 8;
            };
            struct 
            {
                uint16_t ll : 4;
                uint16_t ml : 4;
                uint16_t mr : 4;
                uint16_t rr : 4;
            };
            uint16_t raw;// Each "pixel" has the size of one word (16 bit).
        }pixel_t;

        pixel_t frame_buffer[1024][512];// The frame buffer has a size of 1 MB and is treated as a space of 1024pixels wide and 512 pixels high.
        

        uint32_t texture_window_mask_x;
        uint32_t texture_window_mask_y;
        uint32_t texture_window_offset_x;
        uint32_t texture_window_offset_y;
        uint32_t drawing_area_x1;
        uint32_t drawing_area_y1;
        uint32_t drawing_area_x2;
        uint32_t drawing_area_y2;
        uint32_t x_offset;
        uint32_t y_offset;
        uint32_t display_area_x;
        uint32_t display_area_y;
        uint32_t display_area_x1;
        uint32_t display_area_y1;
        uint32_t display_area_x2;
        uint32_t display_area_y2;
//They’re used to mirror a textured rectangle horizontally or vertically:
        bool textured_rectangle_x_flip;// mirror textured rectangles along the x axis
        bool textured_rectangle_y_flip;// mirror textured rectangles along the y axis


        // struct {    
        //     uint32_t buffer[16];
        //     int wr;
        //     int rd;
        // }fifo;
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
            // uint32_t 
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
        void write(uint32_t vaddr,uint32_t data,int width){
            switch (vaddr)
            {
                case 0x1f801810:
                    ;//return GP0(data);
                case 0x1f801814:
                    ;//return GP1(data);
                default:
                    return;
            }
        }
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
                    break;


                // Draw mode/environment setting packets
                case 0xe1: // 0xe1 draw mode setting
                    break;
                case 0xe2: // 0xe2 texture window setting
                case 0xe3: // 0xe3 set drawing area top left
                case 0xe4: // 0xe4 set drawing area bottom right
                case 0xe5: // 0xe5 drawing offset
                case 0xe6: // 0xe6 mask setting
                    cmd.erase(cmd.begin());
                    break;

                default:
                    cmd.erase(cmd.begin());
                    break;
            }

        }
        void WriteGP1(uint32_t data){
            cmd.push_back(data);
            switch (cmd[0]>>24 & 0xff)
            {
                case 0x00:// GP1(00h) - Reset GPU
                    GP1.raw = 0x14802000;// GPUSTAT becomes 14802000h.
                    break;
                case 0x01:// GP1(01h) - Reset Command Buffer
                    break;
                case 0x02:// GP1(02h) - Acknowledge GPU Interrupt (IRQ1)
                    GP1.irq = 0;// Resets the IRQ flag in GPUSTAT.24.
                    break;
                case 0x03:// GP1(03h) - Display Enable
                    break;
                case 0x04:// GP1(04h) - DMA Direction / Data Request
                    break;
                case 0x05:// GP1(05h) - Start of Display area (in VRAM)
                    break;
                case 0x06:// GP1(06h) - Horizontal Display range (on Screen)
                    break;
                case 0x07:// GP1(07h) - Vertical Display range (on Screen)
                    break;
                case 0x08:// GP1(08h) - Display mode
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

}


