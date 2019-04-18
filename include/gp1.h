#pragma once
#include "bitwise.h"
#include "gp0.h"
#include "gpu.h"
#include "inttypes.h"
#include "draw.h"
namespace GP1_CMDS{
    using namespace Bitwise;
    using namespace GPU;
    #define get_bit(nr,value) ( ( (value>>nr) & 0x1) ? 0b1:0b0 )
    void GP1_00h_resetGPU(uint32_t data){
// GP1(00h) - Reset GPU
        GP1.raw = 0x14802000;// GPUSTAT becomes 14802000h.
        textured_rectangle_x_flip = 0;
        textured_rectangle_y_flip = 0;
    }

    void GP1_01h_reset_cmd_buffer(uint32_t data){
// GP1(01h) - Reset Command Buffer
        GP0 = 0;// 0-23  Not used (zero)
    }

    void GP1_02h_ackGPUirq(uint32_t data){
// GP1(02h) - Acknowledge GPU Interrupt (IRQ1)
        GP1.irq = 0;// Resets the IRQ flag in GPUSTAT.24.
    }

    void GP1_03h_display_enable(uint32_t data){
// GP1(03h) - Display Enable
        GP1.display_enable = data & 0b1;        //0     Display On/Off   (0=On, 1=Off)                         ;GPUSTAT.2
        //  1-23  Not used (zero)
    }
    
    void GP1_04h_dma_dir_req(uint32_t data){
// GP1(04h) - DMA Direction / Data Request
        GP1.dma_direction = data & 0b11;        //0-1  DMA Direction (0=Off, 1=FIFO, 2=CPUtoGP0, 3=GPUREADtoCPU) ;GPUSTAT.29-30
    }
    

    void GP1_05h_set_display_offset(uint32_t data){
// GP1(05h) - Start of Display area (in VRAM)
        display_area_x = extract(0,9,data);     //   0-9   X (0-1023)    (halfword address in VRAM)  (relative to begin of VRAM)
        display_area_y = extract(10,18,data);   //   10-18 Y (0-511)     (scanline number in VRAM)   (relative to begin of VRAM)
        //   19-23 Not used (zero)
        display_area_x &= 0x3fe;// aligned to a 16bit pixel.
        //Note that the LSB of the horizontal coordinate is ignored. It means that
        //we’re always aligned to a 16bit pixel.
    }

    void GP1_06h_set_horizontal_display_range(uint32_t data){
//  GP1(06h) - Horizontal Display range (on Screen)
        display_area_x1 = extract(0,11,data);   //   0-11   X1 (260h+0)       ;12bit       ;\counted in 53.222400MHz units,
        display_area_x2 = extract(12,23,data);  //   12-23  X2 (260h+320*8)   ;12bit       ;/relative to HSYNC
    }

    void GP1_07h_set_vertical_display_range(uint32_t data){
// GP1(07h) - Vertical Display range (on Screen)
        display_area_y1 = extract(0,9,data);    //   0-9   Y1 (NTSC=88h-(224/2), (PAL=A3h-(264/2))  ;\scanline numbers on screen,
        display_area_y2 = extract(10,19,data);  //   10-19 Y2 (NTSC=88h+(224/2), (PAL=A3h+(264/2))  ;/relative to VSYNC
        //   20-23 Not used (zero)
    }

    void GP1_08h_set_display_mode(uint32_t data){
// GP1(08h) - Display mode
        GP1.width0 = data & 0b11;           //   0-1   Horizontal Resolution 1     (0=256, 1=320, 2=512, 3=640) ;GPUSTAT.17-18
        GP1.height = get_bit(2,data);       //   2     Vertical Resolution         (0=240, 1=480, when Bit5=1)  ;GPUSTAT.19
        GP1.video_mode = get_bit(3,data);   //   3     Video Mode                  (0=NTSC/60Hz, 1=PAL/50Hz)    ;GPUSTAT.20
        GP1.use_24bpp = get_bit(4,data);    //   4     Display Area Color Depth    (0=15bit, 1=24bit)           ;GPUSTAT.21
        GP1.interlacing = get_bit(5,data);  //   5     Vertical Interlace          (0=Off, 1=On)                ;GPUSTAT.22
        GP1.width1 = get_bit(6,data);       //   6     Horizontal Resolution 2     (0=256/320/512/640, 1=368)   ;GPUSTAT.16
        GP1._Reverseflag = get_bit(7,data); //   7     "Reverseflag"               (0=Normal, 1=Distorted)      ;GPUSTAT.14
        //   8-23  Not used (zero)
    }



}