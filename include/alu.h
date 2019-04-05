#pragma once
#include "debug.h"
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
    ALU_MULT=13,
    ALU_MULTU=14,
    ALU_DIV=15,
    ALU_DIVU=16,
    ALU_MFHI=17,
    ALU_MFLO=18,
    ALU_MTHI=19,
    ALU_MTLO=20,
    ALU_SLT=21,
    ALU_SLTU=22,
};
namespace ALU{
    uint32_t sra(uint32_t src1,uint32_t sa){// csapp exercise 2.63 
    /* 等价于 result =((int32_t)src1)>>src2); */
        uint32_t w = sizeof(uint32_t)<<3;
        uint32_t xsrl = (uint32_t) src1 >> sa;
        int sign = (( 1 << (w-1-sa) ) & xsrl ) << 1;
        int t = ~ ((1<<(w-sa)) - 1);/* t=[11...1100...00], w-1...w-k 位全 1 */
        int n = t + (sign ^ (1 << (w - sa)));
        uint32_t result = n + xsrl;
        return result;
    }
    uint32_t srl(uint32_t src1,uint32_t sa){
        uint32_t xsra = (int32_t)src1>>sa;
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
            case ALU_SRA:
                aluResult = ((int32_t)src1)>>src2;// sra(src1,src2)
                break;
            case ALU_SRL:
                aluResult = ((uint32_t)src1)>>src2;// srl(src1,src2)
                break;
            case ALU_SUBU:
                aluResult = aluResult - aluResult;
                break;
            case ALU_SUB:
                aluResult = aluResult - aluResult;// if overflow,exception
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
