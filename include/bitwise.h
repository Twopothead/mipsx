#pragma once
#include <inttypes.h>
#include "debug.h"
namespace Bitwise
{

#define set_bit(nr, addr) ({\
    register int res ; \
    __asm__ __volatile__("btsl %2,%3\n\tsetb %%al": \
    "=a" (res):"0" (0),"r" (nr),"m" (*(addr))); \
    res; })

#define clear_bit(nr, addr) ({\
    register int res ; \
    __asm__ __volatile__("btrl %2,%3\n\tsetnb %%al": \
    "=a" (res):"0" (0),"r" (nr),"m" (*(addr))); \
    res; })

uint32_t extract_hi(int high_n, uint32_t tvalue)
{
    uint32_t *tp = (uint32_t *)&tvalue;
    int *ip = (int *)tp, value = *ip;
    uint32_t himask, safemask;
    int low_m = 8 * sizeof(uint32_t) - high_n;
    himask = ((uint32_t)(-1)) << low_m;
    safemask = ~(((uint32_t)(-1)) << high_n);
    return (((value & himask) >> low_m) & safemask);
}

uint32_t extract_lo(int low_n, uint32_t tvalue)
{
    uint32_t *tp = (uint32_t *)&tvalue;
    int *ip = (int *)tp, value = *ip;
    uint32_t mask;
    mask = ~(((uint32_t)(-1)) << low_n);
    return (value & mask);
}

uint32_t extract(int i, int j, uint32_t tvalue)
{
    int totalWidth = 8 * sizeof(uint32_t);
    int width = j - i + 1;
    uint32_t *tp = (uint32_t *)&tvalue;
    if (8 * sizeof(uint32_t) == 32)
    {
        int *ip = (int *)tp, value = *ip;
        uint32_t leftZero = (j == totalWidth - 1) ? ((uint32_t)(-1)) : ~((uint32_t)(-1) << (j + 1));
        uint32_t mask = ((uint32_t)(-1) << (totalWidth - width)) >> (totalWidth - j - 1);
        mask &= leftZero;
        return (value & mask) >> i;
    }
}
} // namespace Bitwise

namespace DECODE
{
    uint32_t get_op(uint32_t instruction);
    uint32_t get_rs(uint32_t instruction);
    uint32_t get_rt(uint32_t instruction);
    uint32_t get_immediate(uint32_t instruction);
    uint32_t get_target(uint32_t instruction);
    uint32_t get_rd(uint32_t instruction);
    uint32_t get_shamt(uint32_t instruction);
    uint32_t get_funct(uint32_t instruction);
    static inline uint32_t _16to32(uint16_t in_16) { return (0x0 | in_16); }
    uint32_t sign_extend(uint16_t value_16b)
    {
        uint16_t mask = 1 << 15;
        bool sign = (value_16b & mask) ? true : false;
        uint32_t tmp = _16to32(value_16b);
        uint32_t extendedValue = (sign) ? (0xffff0000 | tmp) : tmp;
        return extendedValue;
    }
    uint32_t zero_extend(uint16_t value_16b)
    {
        uint32_t extendedValue = (0x0 | value_16b);
        return extendedValue;
    }
    uint32_t shift_left_2(uint32_t in_26b){
        return (in_26b << 2);
    };
    uint32_t calcu_bpc(uint32_t _dpc4,uint32_t _sextended_imm_shift2){// beq or bne
        uint32_t branch_target_address;
        branch_target_address = _dpc4 + _sextended_imm_shift2;
        return branch_target_address;        
    }
    uint32_t calcu_jpc(uint32_t _dpc4,uint32_t target26b){// j or jal
        uint32_t jump_target_address;
        uint32_t jump_address_low_28b = shift_left_2(target26b); 
        uint32_t jump_address_high_4b = _dpc4 & 0xf0000000;
        jump_target_address = jump_address_high_4b | jump_address_low_28b;
        return jump_target_address;
    }
}

uint32_t DECODE::get_op(uint32_t instruction)
{
    return Bitwise::extract_hi(6, instruction);
}
uint32_t DECODE::get_rs(uint32_t instruction)
{
    return Bitwise::extract(21, 25, instruction);
}
uint32_t DECODE::get_rt(uint32_t instruction)
{
    return Bitwise::extract(16, 20, instruction);
}
uint32_t DECODE::get_immediate(uint32_t instruction)
{
    return Bitwise::extract_lo(16, instruction);
}
uint32_t DECODE::get_target(uint32_t instruction)
{
    return Bitwise::extract_lo(26, instruction);
}
uint32_t DECODE::get_rd(uint32_t instruction)
{
    return Bitwise::extract(11, 15, instruction);
}
uint32_t DECODE::get_funct(uint32_t instruction)
{
    return Bitwise::extract_lo(6, instruction);
}
uint32_t DECODE::get_shamt(uint32_t instruction)
{
    return Bitwise::extract(6, 10, instruction);
}