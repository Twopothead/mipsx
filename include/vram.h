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
            return frame_buffer[y][x].raw;
        }

        void vwrite(int x,int y,uint16_t data){
            if(x<0 || x>0x400) return;
            if(y<0 || y>0x200) return;
            frame_buffer[y][x].raw = data;// y*1024+x
        }

}