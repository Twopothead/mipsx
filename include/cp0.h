#pragma once
#include <inttypes.h>
#include <cstring>
#include "debug.h"
//intel x86是小端模式，位域bit field需要逆过来放
typedef union{
    struct{
        uint32_t padding2:8;
        uint32_t Index:6;
        uint32_t padding1:17;
        uint32_t P:1;
    };
    uint32_t raw;
}index_reg;

typedef union {
    struct {
        uint32_t padding2:8;
        uint32_t Random:6;
        uint32_t padding1:18;
    };
    uint32_t raw;    
}random_reg;

typedef union {
    struct {
        uint32_t padding:6;
        uint32_t PID:6;
        uint32_t VPN:20;
    };
    uint32_t raw;    
}tlb_high_reg;

typedef union {
    struct {       
        uint32_t padding:8;
        uint32_t G:1;
        uint32_t V:1;
        uint32_t D:1;
        uint32_t N:1;
        uint32_t FPN:20;          
    };
    uint32_t raw;  
}tlb_low_reg;

typedef union {
    struct{
        uint32_t padding4:2;
        uint32_t EXECODE:5;
        uint32_t padding3:1;
        uint32_t SW:2;
        uint32_t IP:6;
        uint32_t padding2:12;
        uint32_t CE:2;
        uint32_t padding1:1;
        uint32_t BD:1;
    };
    uint32_t raw;
}cause_reg;// 13

namespace CP0_CauseReg_EXECODE_Field{
    enum EXECODE_t{
        INT = 0,
        MOD = 1,
        TLBL = 2,
        TLBS = 3,
        ADEL = 4,
        ADES = 5,
        IBE = 6,
        DBE = 7,
        SYS = 8,
        BP = 9,
        RI = 10,
        CpU = 11,// Co-Processor Unusable Exception
        OVF = 12,
        Tr = 13,
        EXECODE_Padding0 = 14,
        FPE = 15,
        ED_Padding1 = 16,ED_Padding2 = 17,ED_Padding3 = 18,ED_Padding4 =19,
        ED_Padding5 = 20,ED_Padding6 = 21,ED_Padding7 = 22,
        WATCH = 23,
        MCheck = 24,
        ED_Padding8 = 25,ED_Padding9 = 26,ED_Padding10 = 27,
        ED_Padding11 = 28,ED_Padding12 = 29,
        CacheErr = 30,
        ED_Padding13 = 31 
    };
}


typedef union {
    struct {
        uint32_t padding:2;
        uint32_t BADV:19;
        uint32_t PTE_BASE:11;
    };
    uint32_t raw;
}context_reg;


typedef union {
    struct {
        uint32_t IEc:1;
        uint32_t KUc:1;
        uint32_t IEp:1;
        uint32_t KUp:1;
        uint32_t IEo:1;
        uint32_t KUo:1;
        uint32_t padding3:2;
        uint32_t IntMask:8;
        uint32_t IsC:1;
        uint32_t SwC:1;
        uint32_t PZ:1;
        uint32_t CM:1;
        uint32_t PE:1;
        uint32_t TS:1;
        uint32_t BEV:1;
        uint32_t padding2:2;
        uint32_t RE:1;
        uint32_t padding1:2;
        uint32_t CU:4;
    };
    uint32_t raw;
}status_reg;// 12

typedef union {
    struct {
        uint32_t Rev:8;
        uint32_t Imp:8;
        uint32_t padding:16;
    };
    uint32_t raw;
}prid_reg;

namespace R3000_CP0{
    typedef union{
        struct{
        index_reg  INDX;   // cp0 r0  Index
        random_reg RAND;   // cp0 r1  Random
        tlb_low_reg TLBL;  // cp0 r2  TBL low
        uint32_t   BPC;    // cp0 r3  Breakpoint PC
        context_reg CTXT;  // cp0 r4  Context
        uint32_t   BDA;    // cp0 r5  Breakpoint data
        uint32_t   PIDMASK;// cp0 r6  PID Mask
        uint32_t   DCIC;   // cp0 r7  Data/Counter interrupt control
        uint32_t   BADV;   // cp0 r8  Bad Virtual Address
        uint32_t   BDAM;   // cp0 r9  Break data mask
        uint32_t   TLBH;   // cp0 r10 TBL high
        uint32_t   BPCM;   // cp0 r11 Break point counter mask

        status_reg SR;     // cp0 r12 System status register
        cause_reg  CAUSE;  // cp0 r13 Cause
        uint32_t   EPC;    // cp0 r14 Exception Program Counter
        
        prid_reg   PRID;   // cp0 r15 Processor ID
        uint32_t   ERREG;  // cp0 r16  ( 0-16=>17) 
        uint32_t   padding[15];// (0-15=>15)
        };
        uint32_t val[32];
    }R3000_CP0_Registers;// type
    R3000_CP0_Registers cp0_regs;// instance
    void dump_cp0_regs(){
        // 这个地方不要忘了.raw
        // x__log("--------------CP0 registers--------------");
        printf("CP0 STATUS=%0#10x CAUSE=%0#10x EPC=%0#10x\n",\
            cp0_regs.SR.raw,cp0_regs.CAUSE.raw,cp0_regs.EPC);

        printf("CPR00: r00 %08x r01 %08x r02 %08x r03 %08x\n",\
                cp0_regs.INDX.raw,cp0_regs.RAND.raw,cp0_regs.TLBL.raw,cp0_regs.BPC);
        printf("CPR04: r04 %08x r05 %08x r06 %08x r07 %08x\n",\
                cp0_regs.CTXT.raw,cp0_regs.BDA,cp0_regs.PIDMASK,cp0_regs.DCIC);
        printf("CPR08: r08 %08x r09 %08x r10 %08x r11 %08x\n",\
                cp0_regs.BADV,cp0_regs.BDAM,cp0_regs.TLBH,cp0_regs.BPCM);
        printf("CPR12: r12 %08x r13 %08x r14 %08x r15 %08x\n",\
                cp0_regs.SR.raw,cp0_regs.CAUSE.raw,cp0_regs.EPC,cp0_regs.PRID);
        // printf("CPR16: r16 %08x r17 %08x r18 %08x r19 %08x\n",\
        //         cp0_regs.ERREG,cp0_regs.padding[0],cp0_regs.padding[1],cp0_regs.padding[2]);
        // printf("CPR20: r20 %08x r21 %08x r22 %08x r23 %08x\n",\
        //         cp0_regs.padding[3],cp0_regs.padding[4],cp0_regs.padding[5],cp0_regs.padding[6]);
        // printf("CPR24: r24 %08x r25 %08x r26 %08x r27 %08x\n",\
        //         cp0_regs.padding[7],cp0_regs.padding[8],cp0_regs.padding[9],cp0_regs.padding[10]);
        // printf("CPR28: r28 %08x r29 %08x r30 %08x r31 %08x\n",\
        //         cp0_regs.padding[11],cp0_regs.padding[12],cp0_regs.padding[13],cp0_regs.padding[14]);
        printf("CPR16 to CPR31 are reserved and not shown.\n");
    }
}




// class R3000_CP0{
//     R3000_CP0_Registers regs;
//     public:
//     R3000_CP0(){
//     };
// };
