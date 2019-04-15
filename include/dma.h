#pragma once
#include <inttypes.h>
#include "debug.h"
int mipsx_cycle =0;
namespace DMA_TOOLS{
    typedef enum {
        DMA_MDEC_IN  = 0,    // Channel 0: Media Decoder input
        DMA_MDEC_OUT = 1,    // Channel 1: Media Decoder output
        DMA_GPU      = 2,    // Channel 2: GPU
        DMA_CDROM    = 3,    // Channel 3: CDROM drive
        DMA_SPU      = 4,    // Channel 4: SPU
        DMA_PIO      = 5,    // Channel 5: extension port
        DMA_OTC      = 6     // Channel 6: only connected to the RAM and is used to clear an “ordering table”
    }DMA_Channels;
    typedef union {// 0x1f8010f0 - DPCR,  DMA Control
        struct {//intel x86 little endian
            uint32_t priorityMDECin:3; // 0-2   DMA0, MDECin  Priority      (0..7; 0=Highest, 7=Lowest)
            uint32_t enableMDECin:1;   // 3     DMA0, MDECin  Master Enable (0=Disable, 1=Enable)
            uint32_t priorityMDECout:3;// 4-6   DMA1, MDECout Priority      (0..7; 0=Highest, 7=Lowest)
            uint32_t enableMDECout:1;  // 7     DMA1, MDECout Master Enable (0=Disable, 1=Enable)
            uint32_t priorityGPU:3;    // 8-10  DMA2, GPU     Priority      (0..7; 0=Highest, 7=Lowest)
            uint32_t enableGPU:1;      // 11    DMA2, GPU     Master Enable (0=Disable, 1=Enable)
            uint32_t priorityCDROM:3;  // 12-14 DMA3, CDROM   Priority      (0..7; 0=Highest, 7=Lowest)
            uint32_t enableCDROM:1;    // 15    DMA3, CDROM   Master Enable (0=Disable, 1=Enable)
            uint32_t prioritySPU:3;    // 16-18 DMA4, SPU     Priority      (0..7; 0=Highest, 7=Lowest)
            uint32_t enableSPU:1;      // 19    DMA4, SPU     Master Enable (0=Disable, 1=Enable)
            uint32_t priorityPIO:3;    // 20-22 DMA5, PIO     Priority      (0..7; 0=Highest, 7=Lowest)
            uint32_t enablePIO:1;      // 23    DMA5, PIO     Master Enable (0=Disable, 1=Enable)
            uint32_t priorityOTC:3;    // 24-26 DMA6, OTC     Priority      (0..7; 0=Highest, 7=Lowest)
            uint32_t enableOTC:1;      // 27    DMA6, OTC     Master Enable (0=Disable, 1=Enable)
            uint32_t padding:4;        // 28-30 Unknown, Priority Offset or so? (R/W) 
        };                             // 31    Unknown, no effect? (R/W)
        uint32_t raw = 0x07654321;
    // The Nocash spec says that the reset value for the control register is 0x07654321 which means that all channels are disabled and the priority increases with the channel number
    }DPCR_t;

    typedef union {// 0x1f8010f4 - DICR, DMA Interrupt 
        struct {//intel x86 little endian
            uint32_t padding:6;         // 0-5   Unknown  (read/write-able)
            uint32_t not_used:9;        // 6-14  Not used (always zero)
            uint32_t force_irq:1;       // 15    Force IRQ (sets bit31)            (0=None, 1=Force Bit31=1)
            uint32_t channel_irq_en:7;  // 16-22 IRQ Enable for DMA0..DMA6         (0=None, 1=Enable)
            uint32_t master_irq_en:1;   // 23    IRQ Master Enable for DMA0..DMA6  (0=None, 1=Enable)
            uint32_t channel_irq_flags:7;// 24-30 IRQ Flags for DMA0..DMA6          (0=None, 1=IRQ)    (Write 1 to reset)
            uint32_t master_irq_flag:1; //  31    IRQ Master Flag                   (0=None, 1=IRQ)    (Read only)
        };
        uint32_t raw = 0x00000000;
    }DICR_t;

    typedef union { //1F801080h+N*10h - D#_MADR - DMA base address (Channel 0..6) (R/W)
        struct {
            uint32_t addr:24;    //   0-23  Memory Address where the DMA will start reading from/writing to
            uint32_t :8;            //  24-31 Not used (always zero)
        };
        uint32_t raw;
    }D_MADR_t;

    typedef union{// 1F801084h+N*10h - D#_BCR - DMA Block Control (Channel 0..6) (R/W)
        struct {                    // For SyncMode=0 (ie. for OTC and CDROM):
            uint32_t wordCount:16;  // 0-15  BC    Number of words (0001h..FFFFh) (or 0=10000h words)
            uint32_t :16;           //   16-31 0     Not used (usually 0 for OTC, or 1 ("one block") for CDROM)
        }syncMode0;
        struct {                    // For SyncMode=1 (ie. for MDEC, SPU, and GPU-vram-data):
            uint32_t blockSize:16;  //   0-15  BS    Blocksize (words) ;for GPU/SPU max 10h, for MDEC max 20h
            uint32_t blockCount:16; //   16-31 BA    Amount of blocks  ;ie. total length = BS*BA words
        }syncMode1;
        uint32_t raw;
    }D_BCR_t;
    
    typedef union{// 1F801088h+N*10h - D#_CHCR - DMA Channel Control (Channel 0..6) (R/W)
        struct{//intel x86 little endian
            uint32_t direction:1; // 0   transfer direction: RAM-to-device(0) or device-to-RAM(1)
            uint32_t step:1;      // 1   Address increment(0) or decrement(1) mode
            uint32_t chopmode:1;  // 2   Chopping mode
            uint32_t padding1:6;
            uint32_t sync:2;      // [10 : 9] Synchronization type: Manual(0), Request(1) or Linked List(2)
            uint32_t padding2:5;
            uint32_t chopDMAwin:3;// [18 : 16] Chopping DMA window
            uint32_t padding3:1;
            uint32_t chopCPUwin:3;// [22 : 20] Chopping CPU window
            uint32_t padding4:1;
            uint32_t enable:1;    // 24 Enable
            uint32_t padding5:3;
            uint32_t trigger:1;   // 28 Manual trigger
            uint32_t padding6:3;
        };
        uint32_t raw;
    }D_CHCR_t;

    
    
}

namespace DMA{
    using namespace DMA_TOOLS;
// The Nocash spec says that the reset value for the control register is 0x07654321 which means that all channels are disabled and the priority increases with the channel number
    DPCR_t dpcr;// 1f8010f0h DPCR - DMA Control register
    DICR_t dicr;// 1f8010f4h DICR - DMA Interrupt register
    struct Channel_t{
        D_MADR_t address;
        D_BCR_t counter;
        D_CHCR_t control;
    };
    Channel_t channels[7];

    uint32_t get_DMA_channelindex(uint32_t vaddr){
        return (vaddr>>4) & 0b111;// 0...6 DMA ports
    }
    uint32_t get_DMA_regindex(uint32_t vaddr){
        return (vaddr>>2) & 0b11;
    }
    uint32_t read(uint32_t vaddr,int width){
        uint32_t data = 0;
        uint32_t channel_index = get_DMA_channelindex(vaddr);
        if(channel_index < 7)
        {// Per-channel registers 0...6
            switch (get_DMA_regindex(vaddr))
            {
                case 0:// 0x1f801080 0x1f801090 0x1f8010a0 0x1f8010b0 0x1f8010c0 0x1f8010d0 0x1f8010e0
                    data = channels[channel_index].address.raw; 
                    break;
                case 1:// 0x1f801084 0x1f801094 0x1f8010a4 0x1f8010b4 0x1f8010c4 0x1f8010d4 0x1f8010e4
                    data = channels[channel_index].counter.raw;
                    break;
                case 2:// 0x1f801088 0x1f801098 0x1f8010a0 0x1f8010b8 0x1f8010c8 0x1f8010d8 0x1f8010e8
                    data = channels[channel_index].control.raw;
                    break;            
                default:
                    break;
            }
            return data;
        }
        // Control 
        if(channel_index == 7)
        {// Common DMA registers
            switch (get_DMA_regindex(vaddr))
            {
                case 0:// 1f8010f0h
                    data = dpcr.raw;
                    break;
                case 1:// 1f8010f4h
                    data = dicr.raw;
                    break;
                case 2:// 1F8010F8h (usually 7FFAC68Bh? or 0BFAC688h)
                    data = 0x7ffac68b;
                    break;
                case 3:// 1f8010fch (usually 00FFFFF7h) (...maybe OTC fill-value)
                    data = 0x00fffff7;
                    break;
                default:
                    break;
            }
        }
        return data;
     }
    void write(uint32_t vaddr,uint32_t data,int width){
        // if(vaddr==0x1f8010f4){
        //     x__err("%d",mipsx_cycle);
        //     while(1){
        //         ;/* code */
        //     }
            
        // }
   
            
        uint32_t channel_index = get_DMA_channelindex(vaddr);
        if( channel_index == 7 )
        {// control
            switch (get_DMA_regindex(vaddr))
            {
                case 0:
                    dpcr.raw = data;
                    break;
                case 1:
                    dicr.raw = data;
                    break;
                case 2:
                    break;
                case 3:
                    break;                
                default:
                    break;
            }
        }else
        {// Per-channel registers 0...6
            switch (get_DMA_regindex(vaddr))
            {
                case 0:// 0x1f801080 0x1f801090 0x1f8010a0 0x1f8010b0 0x1f8010c0 0x1f8010d0 0x1f8010e0
                    channels[channel_index].address.raw = data & 0x00ffffff; 
                    break;
                case 1:// 0x1f801084 0x1f801094 0x1f8010a4 0x1f8010b4 0x1f8010c4 0x1f8010d4 0x1f8010e4
                    channels[channel_index].counter.raw = data & 0xffffffff;
                    break;
                case 2:// 0x1f801088 0x1f801098 0x1f8010a0 0x1f8010b8 0x1f8010c8 0x1f8010d8 0x1f8010e8
                    channels[channel_index].control.raw = data & 0x71770703;
                    break;            
                default:
                    x__err("io DMA write error: addr:%x data:%x",vaddr,data);
                    break;
            }
        }
        return;
    }

    void dma_main(){
        if(dpcr.enableOTC){
            if(channels[6].control.raw=0x11000002){
                // do
                channels[6].control.raw &= ~0x01000000;
            }
        }
        if(dpcr.enablePIO){

        }
        if(dpcr.enableSPU){

        }
        if(dpcr.enableGPU){

        }
        if(dpcr.enableMDECout){

        }
        if(dpcr.enableMDECin){
            
        }
    }
}