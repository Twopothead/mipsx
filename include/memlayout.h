
#pragma once
#include <inttypes.h>
namespace PlayStationMemLayout
{
    /* main Ram is mirrored 3 times */
    const uint32_t mem_RAM_size = 0x200000; /* 2048KB */
    const uint32_t start_Ram = 0x00000000;
    const uint32_t end_Ram = 0x001fffff; /* 2048KB */
    const uint32_t start_RAMmirror1 = 0x80000000;
    const uint32_t end_RAMmirror1 = 0x801fffff; /* 2048KB */
    const uint32_t start_RAMmirror2 = 0xa0000000;
    const uint32_t end_RAMmirror2 = 0xa01fffff; /* 2048KB */
                                                /* BIOS rom */
    const uint32_t mem_ROM_size = 0x80000;      /* 512KB  */
    const uint32_t start_Rom = 0xbfc00000;      /* BIOS Region (default 512 Kbytes, max 4 MBytes) */
    const uint32_t end_Rom = 0xbfc7ffff;        /* 512KB  */
                                                /* SPU */
    const uint32_t mem_spu_size = 0x280;        /* 640B   */
    const uint32_t start_spu = 0x1f801c00;
    const uint32_t end_spu = 0x1f801e80;
    /* expansion 1 */
    const uint32_t mem_expansion1_size = 0x800000; /*8*1024KB*/
    const uint32_t start_expansion1 = 0x1f000000;  /* Expansion Region (default 512 Kbytes, max 8 MBytes) */
    const uint32_t end_expansion1 = 0x1f7fffff;    /*8*1024KB*/
                                                /* expansion 2 */
    const uint32_t mem_expansion2_size = 0x80;     /* 128B */
    const uint32_t start_expansion2 = 0x1f802000;  /* Expansion Region 2 (default 128 bytes, max 8 KBytes)*/
    const uint32_t end_expansion2 = 0x1f80207f;    /* 128B */
                                                /* interrupt control*/
    const uint32_t irq_ctrl_I_STAT = 0x1f801070;   /* Interrupt status register */
    const uint32_t irq_ctrl_I_MASK = 0x1f801074;   /* Interrupt mask register */
                                                /* DMA Registers */
    const uint32_t DMA_regs_addr = 0x1f801080;
    /* GPU Registers */
    const uint32_t GPU_regs_addr = 0x1f801810;
    /* Timers (aka Root counters) */
    const uint32_t Timers_addr = 0x1f801100;
    /* Cache Control */
    const uint32_t CacheCtrl_addr = 0xfffe0130; /* size:4 */
                                                /* Ram Size */
    const uint32_t Ram_Size_addr = 0x1f801060;  /* (usually 00000B88h; 2MB RAM mirrored in first 8MB) */
} // namespace PlayStationMemLayout
