#pragma once
#include <inttypes.h>
#include "alu.h"
namespace IF_Signals
{
    uint32_t npc;
    uint32_t pc4;
    uint32_t ins;
    uint32_t exception_handler_address;
    void clear_temp_IF_signals(){
        npc = 0,pc4 = 0,ins = 0;
        exception_handler_address = 0;
    }

} 

namespace ID_Signals
{
    bool wreg;
    bool m2reg;
    bool wmem;
    bool jal;
    bool link;
    bool lbu;
    bool lhu;
    ALUOP_t aluc;
    bool aluimm;
    bool shift;
    bool regrt;
    bool sext;
    uint32_t fwda;
    uint32_t fwdb;
    uint32_t jpc;
    uint32_t dpc4;
    uint32_t bpc;
    uint32_t da;
    uint32_t db;
    uint32_t dimm;
    uint32_t drn;
    uint32_t op;
    uint32_t funct;
    uint32_t rs;
    uint32_t rt;
    uint32_t addr;
    uint32_t imm;
    uint32_t rd;
    uint32_t cp0_epcin;
    bool cp0_exc;
    uint32_t dsl_width_sel;
    void clear_ID_temp_signals(){
        wreg = false,wreg = false,m2reg = false,wmem = false,jal = false,aluimm = false,shift =false,regrt = false,sext = false;
        fwda = 0,fwdb = 0,jpc = 0,dpc4 = 0,bpc = 0,da = 0,db = 0, dimm = 0,drn = 0,op = 0,funct = 0,rs = 0,rt = 0,addr = 0,imm =0,rd = 0;
        cp0_epcin = 0;
        cp0_exc = false;
        dsl_width_sel = 0;
    }
} 

namespace EXE_Signals
{
    uint32_t epc8;
    uint32_t sa;// shamt: shift amount
    uint32_t eALUOp1;
    uint32_t eALUOp2;
    uint32_t eALUresult;
    uint32_t ealu;
    uint32_t ern;
    uint32_t pc8c0r;

    void clear_ex_temp_signals(){
        epc8 = 0,sa = 0,eALUOp1 = 0,eALUOp2 = 0,eALUresult = 0,ealu = 0,ern =0;
        pc8c0r = 0;
    }
} 

namespace MEM_Signals
{
    uint32_t mmo;
    uint32_t storeload_width_sel;
    uint32_t storeload_width;
    void clear_MEM_temp_signals(){
        mmo = 0;
        storeload_width_sel = 0;
        storeload_width = 32;// default 32 
    }
}
namespace WB_Signals
{
    uint32_t wdi;
    void clear_WB_temp_signals(){
        wdi = 0;
    }
}