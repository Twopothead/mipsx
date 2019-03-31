#pragma once
#include "debug.h"
enum ALUOP_t
{
    ALU_ADD,
    ALU_ADDU,
    ALU_SUB,
    ALU_SUBU,
    ALU_AND,
    ALU_OR,
    ALU_NOR,
    ALU_XOR,
    ALU_LU,
    ALU_SLL,
    ALU_SRL,
    ALU_SRA,
    ALU_MULT,
    ALU_MULTU,
    ALU_DIV,
    ALU_DIVU,
    ALU_MFHI,
    ALU_MFLO,
    ALU_MTHI,
    ALU_MTLO,
    ALU_SLT,
    ALU_SLTU,
    ALU_EQ,
    ALU_NE,
    ALU_LEZ,
    ALU_LTZ,
    ALU_GTZ,
    ALU_GEZ
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
            
                
            default:
                break;
        }
        return aluResult;
    }

    
} 
