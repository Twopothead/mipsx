#pragma once
#include <inttypes.h>
namespace VRAM{
        typedef union{
            struct 
            {
                uint16_t r : 5;
                uint16_t g : 5;
                uint16_t b : 5;
                uint16_t alpha :1;
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
        
        uint16_t vread(int x,int y){
            return frame_buffer[x][y].raw;
        }

        void vwrite(int x,int y,uint16_t data){
            // if(x<0 || x>0x400) return;
            // if(y<0 || y>0x200) return;
            frame_buffer[x][y].raw = data;
        }

        union tclut
        {
            uint16_t full;
            struct
            {
                unsigned x : 6 ;
                unsigned y : 9 ;
                unsigned zr : 1;
            };
        };

        union tpage
        {
            uint16_t full;
            struct
            {
                unsigned xb  : 4;  // 0-3   Texture page X Base   (N*64) (ie. in 64-halfword steps)    ;GPUSTAT.0-3
                unsigned yb  : 1;  // 4    Texture page Y Base   (N*256) (ie. 0 or 256)               ;GPUSTAT.4
                unsigned st  : 2;  // 5-6   Semi Transparency     (0=B/2+F/2, 1=B+F, 2=B-F, 3=B+F/4)   ;GPUSTAT.5-6
                unsigned col : 2;  // 7-8   Texture page colors   (0=4bit, 1=8bit, 2=15bit, 3=Reserved);GPUSTAT.7-8
                unsigned : 7 ;
            };
        };

}