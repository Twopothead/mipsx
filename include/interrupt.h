#pragma once
#include <inttypes.h>
#include "iotools.h"
#include "cp0.h"
#include <stdlib.h>
#include "wires.h"
#include "pipeline_regs.h"
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
        void check_interrupt(){
            using namespace R3000_CP0;
            uint32_t iec = cp0_regs.SR.IEc;
            uint32_t irq = (cp0_regs.SR.raw & cp0_regs.CAUSE.raw & 0xff00) != 0;
            if(irq&&iec){
                x__log("fuck your mother %x",cp0_regs.SR.BEV);
                using namespace pipeline_registers;
                IF_ID.intr = true;
                // x__err("%x %x %x %x",IF_pc,ID_pcd,EX_pce,MEM_pcm);
                //选PCE
                // EX_pce

            }
            // printf("%x %x",cp0_regs.SR.raw& 0xff00 , cp0_regs.CAUSE.raw);
            
            
                
        }
        void update_irq(uint32_t stat,uint32_t mask){
            interrupt_ctrl.I_STAT = stat;
            interrupt_ctrl.I_MASK = mask;
            using namespace R3000_CP0;
            if(interrupt_ctrl.I_STAT & interrupt_ctrl.I_MASK){
                cp0_regs.CAUSE.raw |= (1<<10);// cp0_regs.CAUSE.IP |= 1;
            }else
            {
                cp0_regs.CAUSE.raw &= ~(1<<10);// cp0_regs.CAUSE.IP &= ~1;
            }
        }

        void set_imask(uint32_t value){
            update_irq(interrupt_ctrl.I_STAT,value);            
        }

        void set_istat(uint32_t value){
            update_irq(value,interrupt_ctrl.I_MASK);
        }

        void bus_irq(uint32_t interrupt){
            uint32_t flag = 1 << interrupt;
            set_istat(interrupt_ctrl.I_STAT | flag);
        }
        void write(uint32_t vaddr,uint32_t data,int width){
            switch (vaddr)
            {
                case 0x1f801070:
                    set_istat( data );
                    break;
                case 0x1f801074:
                    set_imask( data & 0x7ff);
                    break;
                default:
                    x__err("Interrupt_Control write error:vaddr%x,data%x",vaddr,data);
                    break;
            }
        }
        // 1F801074h I_MASK - Interrupt mask register (R/W)
        // Status: Read I_STAT (0=No IRQ, 1=IRQ)
        // Acknowledge: Write I_STAT (0=Clear Bit, 1=No change)
        // Mask: Read/Write I_MASK (0=Disabled, 1=Enabled)
        enum IRQ_MASK{
            IRQ0_VBLANK     = 0,//   0     IRQ0 VBLANK (PAL=50Hz, NTSC=60Hz)
            IRQ1_GPU        = 1,//   1     IRQ1 GPU   Can be requested via GP0(1Fh) command (rarely used)
            IRQ2_CDROM      = 2,//   2     IRQ2 CDROM
            IRQ3_DMA        = 3,//   3     IRQ3 DMA
            IRQ4_TMR0       = 4,//   4     IRQ4 TMR0  Timer 0 aka Root Counter 0 (Sysclk or Dotclk)
            IRQ5_TMR1       = 5,//   5     IRQ5 TMR1  Timer 1 aka Root Counter 1 (Sysclk or H-blank)
            IRQ6_TMR2       = 6,//   6     IRQ6 TMR2  Timer 2 aka Root Counter 2 (Sysclk or Sysclk/8)
            IRQ7_CTRLMEMCARD= 7,//   7     IRQ7 Controller and Memory Card - Byte Received Interrupt
            IRQ8_SIO        = 8,//   8     IRQ8 SIO
            IRQ9_SPU        = 9,//   9     IRQ9 SPU
            IRQ10_PIO       = 10//   10    IRQ10 Controller - Lightpen Interrupt (reportedly also PIO...?)
            //   11-15 Not used (always zero)
            //   16-31 Garbage
        };


}