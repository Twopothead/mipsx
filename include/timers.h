#pragma once
#include <inttypes.h>
namespace PSX_Timer{
        const uint32_t BASE_Timer0 = 0x1f801100;
        const uint32_t BASE_Timer1 = 0x1f801110;
        const uint32_t BASE_Timer2 = 0x1f801120;
        struct Timer_t{
            uint16_t counter;
            uint16_t control;
            uint16_t compare;
        };
        Timer_t timers[3];
        uint32_t read(uint32_t vaddr,int width){
            int n = (vaddr>>4) & 0b11;
            switch ( (vaddr &0xf) )
            {
                case 0:// 0x1f801100 for timer0
                    return timers[n].counter;
                case 4:// 0x1f801104 for timer1
                    return timers[n].control;
                case 8:// 0x1f801108 for timer2
                    return timers[n].compare;
                default:
                    break;
            }
            return 0;
        };
        void write(uint32_t vaddr,uint32_t data,int width){
            int n = (vaddr>>4) & 0b11;
            switch ( (vaddr &0xf) )
            {
                case 0:
                    timers[n].counter = (uint16_t) data; 
                    break;
                case 4:
                    timers[n].control = (uint16_t) (data | 0x400);
                    timers[n].counter = 0;
                    break;
                case 8:
                    timers[n].compare = (uint16_t) data;
                    break;            
                default:
                    break;
            }
        }
}