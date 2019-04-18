#pragma once
#include <stdio.h>
#include <inttypes.h>
#include "cpu.h"
// https://image.slidesharecdn.com/05-multiplydivide-140517040534-phpapp01/95/05-multiply-divide-19-1024.jpg?cb=1400299578
// https://www.slideshare.net/prochwani95/05-multiply-divide
namespace DivideUnit{
    typedef unsigned __int128 uint128_t;
    uint128_t Remainder;// 65 bit
    uint32_t Divisor;
    #define Get_bit(nr,value) ( ( (value>>nr) & 0x1) ? true:false )
    uint128_t set_high32(uint128_t r,uint32_t high32){
        uint32_t origin_lo = r & 0xffffffff;
        r = high32;
        r <<= 32;
        r |= origin_lo; 
        return r;
    }
    uint128_t set_low32(uint128_t r,uint32_t low32){
        uint32_t origin_high = r >>32;
        r = origin_high;
        r <<= 32;
        r |= low32;
        return r;
    }
    uint32_t get_hi(){// Remainder
        return (Remainder>>32) & 0xffffffff;
    }
    uint32_t get_lo(){// Quotient
        return (Remainder & 0xffffffff);
    }
    void Divu(uint32_t dividend,uint32_t divisor){
    // unsigned division
        Remainder = dividend;
        Divisor = divisor;
        uint64_t difference;
        for(int i=0;i<32;i++){
            Remainder <<= 1;
            difference = (Remainder>>32) - Divisor;
            if(Get_bit(32,difference)==0){
                Remainder = set_high32(Remainder,difference);
                Remainder |= 0b1;
            }else{
                ;
            }    
        }
        // printf("%d / %d = %d ... %d\n",dividend,divisor,get_lo(),get_hi());
    }
    uint32_t negate(uint32_t num){// 求补
        num = ~num + 1;
        return num;
    }
    void Div(uint32_t dividend,uint32_t divisor){
    // signed division
        bool s_dividend = Get_bit(31,dividend);
        bool s_divisor = Get_bit(31,divisor);
        bool s_remainder = (s_dividend) ^ (s_divisor); 
        Remainder = (s_dividend)?(-dividend):dividend;
        Divisor = (s_divisor)?(-divisor):divisor;
        uint64_t difference;
        for(int i=0;i<32;i++){
            Remainder <<= 1;
            difference = (Remainder>>32) - Divisor;
            if(Get_bit(32,difference)==0){// carry out of the adder
                Remainder = set_high32(Remainder,difference);
                Remainder |= 0b1;
            }else{
                ;
            }    
        }
// Thus the correctly signed division algorithm 
// negates the quotient if the signs of the operands are opposite 
// and makes the sign of the nonzero remainder match the dividend.
        if(s_remainder)
            Remainder = set_low32(Remainder, negate(Remainder & 0xffffffff));
        if(get_hi()!=0)
            Remainder = set_high32(Remainder, (s_dividend ? negate(get_hi()):get_hi()) );

        // printf("%d / %d = %d ... %d\n",dividend,divisor,get_lo(),get_hi());
    }
}
