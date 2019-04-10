#pragma once
#include "debug.h"
#include "inttypes.h"
enum ALUOP_t
{
    ALU_ADD=1,
    ALU_ADDU=2,
    ALU_SUB=3,
    ALU_SUBU=4,
    ALU_AND=5,
    ALU_OR=6,
    ALU_NOR=7,
    ALU_XOR=8,
    ALU_LU=9,
    ALU_SLL=10,
    ALU_SRL=11,
    ALU_SRA=12,
    ALU_SLT,
    ALU_SLTU,
    ALU_MULT,
    ALU_MULTU,
    ALU_DIV,
    ALU_DIVU
};
// C语言中，对于移位操作执行的是逻辑左移和算术右移，不过对于无符号类型，所有的移位操作都是逻辑的
namespace ALU{
    uint32_t sra(uint32_t src2,uint32_t sa){// rd ← rt >> sa
// csapp exercise 2.63 
    /* 等价于 result = (((int32_t)src2)>>src1); */
        uint32_t w = sizeof(uint32_t)<<3;
        uint32_t xsrl = (uint32_t) src2 >> sa;
        int sign = (( 1 << (w-1-sa) ) & xsrl ) << 1;
        int t = ~ ((1<<(w-sa)) - 1);/* t=[11...1100...00], w-1...w-k 位全 1 */
        int n = t + (sign ^ (1 << (w - sa)));
        uint32_t result = n + xsrl;
        return result;
    }
    uint32_t srl(uint32_t src2,uint32_t sa){// rd ← rt >> sa (logical)
    /* 等价与 result = (((uint32_t)src2)>>src1); */
        uint32_t xsra = (int32_t)src2>>sa;
        uint32_t w = sizeof(uint32_t)<<3;
        uint32_t result = xsra & ( (1<<(w-sa)) -1);
    }
    uint32_t ALUOperation(uint32_t src1,uint32_t src2,ALUOP_t aluop){
        uint32_t aluResult=0;
        switch (aluop)
        {
            case ALU_LU:
                aluResult = ( (src2<<16) & 0xffff0000);
                break;
            case ALU_OR:
                aluResult = (src1 | src2);
                break;
            case ALU_ADD:
                aluResult = (src1 + src2);
                break;
            case ALU_SLL:
                aluResult = src2 << src1;// rd ← rt << sa
                break;
            case ALU_SLT:// the comparison is done using signed arithmetics
                aluResult = 0x00000001 & (((int32_t)src1<(int32_t)src2)?1:0);
                break;
            case ALU_SLTU:
                aluResult = 0x00000001 & (src1<src2?1:0);
                break;
            case ALU_ADDU:
                aluResult = (src1 + src2);// if overflow,exception
                break;
            case ALU_AND:
                aluResult = (src1 & src2);
                break;
            case ALU_SRA:// rd ← rt >> sa
                src1 &= 0x1f; // rd ← rt >> rs (arithmetic)
                aluResult = ((int32_t)src2)>>src1;// sra(src2,src1);
                break;
            case ALU_SRL:// rd ← rt >> sa (logical)
                src1 &= 0x1f;
                aluResult = ((uint32_t)src2)>>src1;// srl(src2,src1)
                break;
            case ALU_SUBU:
                aluResult = src1 - src2;
                break;
            case ALU_SUB:
                aluResult = src1 - src2;// if overflow,exception
                break;
            case ALU_NOR:
                aluResult = ~(src1|src2);
                break;
              // branch的条件应该在ID阶段算出，不应留到EX阶段
            // case ALU_DIV:
            //     aluResult;
            default:
                break;
        }
        return aluResult;
    }

    
} 
