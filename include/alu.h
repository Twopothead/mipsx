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
    ALU_EQ=23,
    ALU_NE=24,
    ALU_LEZ=25,
    ALU_LTZ=26,
    ALU_GTZ=27,
    ALU_GEZ=28
};
namespace ALU{
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
            case ALU_SLT:
                aluResult = 0x00000001 & (src1<src2?1:0);
                break;
            case ALU_SLTU:
                aluResult = 0x00000001 & (src1<src2?1:0);
                break;
            case ALU_ADDU:
                aluResult = (src1 + src2);// if overflow,exception
                break;
            case ALU_AND:
                aluResult = (src1 & src2);
                
            default:
                break;
        }
        return aluResult;
    }

    
} 
