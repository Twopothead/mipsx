#pragma once
#include <inttypes.h>
typedef enum{
    DMA_MDEC_IN,
    DMA_MDEC_OUT,
    DMA_GPU,
    DMA_CDROM,
    DMA_SPU,
    DMA_PIO,
    DMA_OTC     // 
}DMA_Channels;
typedef union {
    struct {//intel x86 little endian
        uint32_t padding:6;// 0-5   Unknown  (read/write-able)
        uint32_t not_used:9;// 6-14  Not used (always zero)
        uint32_t force_irq:1;// 15    Force IRQ (sets bit31)            (0=None, 1=Force Bit31=1)
        uint32_t channel_irq_en:7;// 16-22 IRQ Enable for DMA0..DMA6         (0=None, 1=Enable)
        uint32_t irq_en:1;// 23    IRQ Master Enable for DMA0..DMA6  (0=None, 1=Enable)
        uint32_t channel_irq_flags:7;// 24-30 IRQ Flags for DMA0..DMA6          (0=None, 1=IRQ)    (Write 1 to reset)
        uint32_t irq_flag:1;//  31    IRQ Master Flag                   (0=None, 1=IRQ)    (Read only)
    };
    uint32_t raw;
}DMA_Channel_Control_register;
typedef union {
    struct {

    };
    
    uint32_t raw;
}DMA_;


namespace DMA{
// The Nocash spec says that the reset value for the control register is 0x07654321 which means that all channels are disabled and the priority increases with the channel number
    uint32_t DPCR = 0x07654321;// 1f8010f0h DPCR - DMA Control register
    uint32_t DPIR = 0x00000000; // 1f8010f4h DICR - DMA Interrupt register
    struct Channel_t{
        uint32_t address;
        uint32_t counter;
        uint32_t control;
    };
    Channel_t channels[3];
    uint32_t get_DMA_regindex(uint32_t vaddr){
        return (vaddr>>2) & 0b11;
    }
    uint32_t get_DMA_channelindex(uint32_t vaddr){
        return (vaddr>>4) & 0b111;
    }
    uint32_t read(uint32_t vaddr,int width){
        uint32_t data = 0;
        uint32_t channel_index = get_DMA_channelindex(vaddr);
        if(channel_index == 7)
        {
            switch (get_DMA_regindex(vaddr))
            {
                case 0:// 1f8010f0h
                    data = DPCR;
                    break;
                case 1:// 1f8010f4h
                    data = DPIR;
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
        }else
        {
            switch (get_DMA_regindex(vaddr))
            {
                case 0:// 0x1f801080 0x1f801090 0x1f8010a0 0x1f8010b0 0x1f8010c0 0x1f8010d0 0x1f8010e0
                    data = channels[channel_index].address; 
                    break;
                case 1:// 0x1f801084 0x1f801094 0x1f8010a4 0x1f8010b4 0x1f8010c4 0x1f8010d4 0x1f8010e4
                    data = channels[channel_index].counter;
                    break;
                case 2:// 0x1f801088 0x1f801098 0x1f8010a0 0x1f8010b8 0x1f8010c8 0x1f8010d8 0x1f8010e8
                    data = channels[channel_index].control;            
                default:
                    break;
            }
        }
        return data;
     }
    void write(uint32_t vaddr,uint32_t data,int width){
        uint32_t channel_index = get_DMA_channelindex(vaddr);
        if( channel_index == 7 )
        {
            switch (get_DMA_regindex(vaddr))
            {
                case 0:
                    DPCR = data;
                    break;
                case 1:
                    while(1){
                        ;/* code */
                    }
                    
                    // DPIR = data;
                    break;
                case 2:
                    break;
                case 3:
                    break;                
                default:
                    break;
            }
        }else
        {
            switch (get_DMA_regindex(vaddr))
            {
                case 0:// 0x1f801080 0x1f801090 0x1f8010a0 0x1f8010b0 0x1f8010c0 0x1f8010d0 0x1f8010e0
                    channels[channel_index].address = data & 0x00ffffff; 
                    break;
                case 1:// 0x1f801084 0x1f801094 0x1f8010a4 0x1f8010b4 0x1f8010c4 0x1f8010d4 0x1f8010e4
                    channels[channel_index].counter = data & 0xffffffff;
                    break;
                case 2:// 0x1f801088 0x1f801098 0x1f8010a0 0x1f8010b8 0x1f8010c8 0x1f8010d8 0x1f8010e8
                    channels[channel_index].control = data;            
                default:
                    break;
            }
        }
    }
}