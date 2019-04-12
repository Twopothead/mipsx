#pragma once
#include <inttypes.h>
namespace RW{
    void io_custom_write(uint32_t addr,uint32_t base_addr,uint32_t *pbase,uint32_t data,int width){
            uint32_t offset = addr - base_addr;//addr - 0x1f801d80;
            uint32_t *pword = pbase;//spu_ctrl_regs.raw;
            uint16_t *phalf_word = (uint16_t*)pbase;//spu_ctrl_regs.raw;
            uint8_t *pbyte = (uint8_t*)pbase;//spu_ctrl_regs.raw;
            switch (width)
            {
                case 32:
                    pword += offset>>2;//offset/4;   (32/8=4)
                    *pword = (uint32_t)data;
                    break;
                case 16:
                    phalf_word += offset>>1;//offset/2; (16/8=2)
                    *phalf_word = (uint16_t)(data & 0x0000ffff); 
                    break;
                case 8:
                    pbyte += offset;//offset/1; (8/8=1)
                    *pbyte = (uint8_t)(data & 0xff);
                    break;            
                default:
                    pword += offset>>2;
                    *pword = (uint32_t)data;
                    break;
            }
            return;
    }

    uint32_t io_custom_read(uint32_t addr,uint32_t base_addr,uint32_t *pbase,int width){
            uint32_t offset = addr - base_addr;//addr - 0x1f801d80;
            uint32_t *pword = pbase;//spu_ctrl_regs.raw;
            uint16_t *phalf_word = (uint16_t*)pbase;//spu_ctrl_regs.raw;
            uint8_t *pbyte = (uint8_t*)pbase;//spu_ctrl_regs.raw;
            uint32_t data = 0;
            switch (width)
            {
                case 32:
                    pword += offset>>2;//offset/4;   (32/8=4)
                    data = *pword;
                    break;
                case 16:
                    phalf_word += offset>>1;//offset/2; (16/8=2)
                    data = *phalf_word; 
                    break;
                case 8:
                    pbyte += offset;//offset/1; (8/8=1)
                    data = *pbyte;
                    break;            
                default:
                    pword += offset>>2;
                    data =*pword;
                    break;
            }
            return data;
    }
}