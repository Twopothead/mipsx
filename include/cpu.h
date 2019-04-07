#pragma once
#include <inttypes.h>
typedef union {
    struct
    {
        const uint32_t R00_ZR = 0;
        uint32_t R01_AT;
        uint32_t R02_V0, R03_V1;
        uint32_t R04_A0, R05_A1, R06_A2, R07_A3;
        uint32_t R08_T0, R09_T1, R10_T2, R11_T3, R12_T4, R13_T5, R14_T6, R15_T7;
        uint32_t R16_S0, R17_S1, R18_S2, R19_S3, R20_S4, R21_S5, R22_S6, R23_S7;
        uint32_t R24_T8, R25_T9;
        uint32_t R26_K0, R27_K1;
        uint32_t R28_GP;
        uint32_t R29_SP;
        uint32_t R30_FP;
        uint32_t R31_RA;
    };
    uint32_t register_file[32];
    public:
    void set_reg(int index, uint32_t value)
    {
        if (index)
            register_file[index] = value;
    };
    friend class MIPSX_SYSTEM;
} R3000A_General_Purpose_Registers;

namespace HiLORegs{
    uint32_t HI = 0xcafebabe;
    uint32_t LO = 0xdeadbeef;
}

class R3000A_CPU
{
    R3000A_General_Purpose_Registers gp;
    public:
        R3000A_CPU(){
            memset(&gp.register_file,0,sizeof(uint32_t[32]));
        };
        friend class MIPSX_SYSTEM;
        void dump_regs(){
            printf("PSX HI=%0#10x LO=%0#10x\n",HiLORegs::HI,HiLORegs::LO);
            printf("GPR00: r0 %08x at %08x v0 %08x v1 %08x\n",\
                gp.R00_ZR,gp.R01_AT,gp.R02_V0,gp.R03_V1);
            printf("GPR04: a0 %08x a1 %08x a2 %08x a3 %08x\n",\
                gp.R04_A0,gp.R05_A1,gp.R06_A2,gp.R07_A3);
            printf("GPR08: t0 %08x t1 %08x t2 %08x t3 %08x\n",\
                gp.R08_T0,gp.R09_T1,gp.R10_T2,gp.R11_T3);
            printf("GPR12: t4 %08x t5 %08x t6 %08x t7 %08x\n",\
                gp.R12_T4,gp.R13_T5,gp.R14_T6,gp.R15_T7);
            printf("GPR16: s0 %08x s1 %08x s2 %08x s3 %08x\n",\
                gp.R16_S0,gp.R17_S1,gp.R18_S2,gp.R19_S3);
            printf("GPR20: s4 %08x s5 %08x s6 %08x s7 %08x\n",\
                gp.R20_S4,gp.R21_S5,gp.R22_S6,gp.R23_S7);
            printf("GPR24: t8 %08x t9 %08x k0 %08x k1 %08x\n",\
                gp.R24_T8,gp.R25_T9,gp.R26_K0,gp.R27_K1);
            printf("GPR28: gp %08x sp %08x s8 %08x ra %08x\n",\
                gp.R28_GP,gp.R29_SP,gp.R30_FP,gp.R31_RA);
        }
};