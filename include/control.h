
#pragma once
#include <inttypes.h>
#include "alu.h"
#include "wires.h"
#include "forward.h"
#include "debug.h"
#include "bitwise.h"
#include "cp0.h"
#include "mul.h"
#include "div.h"
#include "cpu.h"
#include "dma.h"
#include <stdlib.h>
#include "cp0.h"
namespace PipelineStall{
    using namespace pipeline_registers;
    bool Stall = false;
    // bool calcuStall(bool use_rs_field,bool use_rt_field,uint32_t rs,uint32_t rt){
    //     bool stall = false;
    //     // bool i_rs = use_rs_field;// i_rs and i_rt indicate that an instruction
    //     // bool i_rt = use_rt_field;// uses the contents of the rs register and the rt register,
    //     // stall = (  ID_EX.eRegWrite
    //     //         and (ID_EX.eMemtoReg)
    //     //         and (ID_EX.eRegisterRn!=0)
    //     //         and (      ( (i_rs) and (ID_EX.eRegisterRn==rs) )
    //     //                or  ( (i_rt) and (ID_EX.eRegisterRn==rt) )
    //     //              )
    //     //         );
    //     return stall;
    // }

}
namespace CONTROL{
    struct {
        uint32_t i_mrn;
        bool i_mm2reg;
        bool i_mwreg;
        uint32_t i_ern;
        bool i_em2reg;
        bool i_ewreg;
        bool o_wreg;
        bool o_m2reg;
        bool o_wmem;
        bool o_jal;
        bool o_link;
        ALUOP_t  o_aluc;
        bool o_aluimm;
        bool o_shift;
        bool o_regrt;
        bool i_rsrtequ;
        bool i_rsGEZ;// if rs ≥ 0 then branch
        bool i_rsLTZ;// if rs < 0 then procedure_call
        bool i_rsLEZ;// if rs ≤ 0 then branch
        bool i_rsGTZ;// if rs > 0 then branch
        bool o_sext;
        uint32_t o_fwda;
        uint32_t o_fwdb;
        uint32_t op;
        uint32_t funct;
        uint32_t rs;
        uint32_t rt;
        uint32_t o_pcsrc;
        uint32_t o_selpc;
        // uint32_t o_wpcir;
        bool o_stall;
        uint32_t o_sl_width_sel;//0b00 32;0b01 16; 0b10 8
        bool o_lbu;/*lbu取出来的8bit数要zero-extend(而lb是8bit signextend到32bit)*/
        bool o_lhu;
        bool o_mfHI;
        bool o_mfLO;
        bool o_mtHI;
        bool o_mtLO;
        bool o_lwl;
        bool o_lwr;
        bool o_swl;
        bool o_swr;
    }CTRL_UNIT;

    struct {
        uint32_t cop0_ins;
        uint32_t rt;
        uint32_t rd;
        bool i_ecancel;
        bool i_irq;
        bool o_cancel;
        bool o_isbr;
        bool o_ove;
        bool o_wsta;/* cp0 reg 12 */
        bool o_wcau;/* cp0 reg 13 */
        bool o_wepc;/* cp0 reg 14 */
        uint32_t o_wcp0_regs_sel;/*选择　写信号*/
        bool o_mtc0;
        bool o_rfe;
        uint32_t o_mfc0;
        uint32_t o_cause;
        uint32_t o_sepc;
        bool o_exc;
    }CTRL_CP0_UNIT;
    void cp0_notsoimportantCases(){
        const uint32_t ins_TLBR  = 0b01000010000000000000000000000001;
        const uint32_t ins_TLBP  = 0b01000010000000000000000000001000;
        const uint32_t ins_RFE   = 0b01000010000000000000000000010000;
        const uint32_t ins_TLBWI = 0b01000010000000000000000000000010;
        const uint32_t ins_TLBWR = 0b01000010000000000000000000000110;
        switch (CTRL_CP0_UNIT.cop0_ins)
        {
            case ins_TLBR:// TLBR
                x__err("unhandled cp0 TLBR");
                break;
            case ins_TLBP:// TLBP
                x__err("unhandled cp0 TLBP");
                break;
            case ins_RFE:// RFE
// RFE Restore previous interrupt mask and mode bits of status register into current status
// bits. Restore old status bits into previous status bits.
                CTRL_CP0_UNIT.o_rfe = true;// 其实这里并不需要改PC,Rfe不修改pc,它之前有jmp帮他返回
                // x__err("unhandled cp0 RFE");
                break;
            case ins_TLBWI:// TLBWI
                x__err("unhandled cp0 TLBWI");
                break;
            case ins_TLBWR:// TLBWR
                x__err("unhandled cp0 TLBWR");
                break;
            default:
                break;
        }
    }
    void cp0_mtc0_ctrl(){
        // CPR[0, rd, sel] ← rt
        switch (CTRL_CP0_UNIT.rd)
        {
                case 12:// cp0 r12 status_reg
                    CTRL_CP0_UNIT.o_wsta = true;
                    break;
                case 13:// cp0 r13 cause_reg
                    CTRL_CP0_UNIT.o_wcau = true;
                    break;
                case 14:// cp0 r14 Exception Program Counter
                    CTRL_CP0_UNIT.o_wepc = true;
                case 15 ... 31:
                    CTRL_CP0_UNIT.o_wcp0_regs_sel = CTRL_CP0_UNIT.rd | 0xffff0000;
                    /* 为防止出现默认的0,这里我把高16位置1,最后选寄存器时再&回来，我们只用到它的最低5位来选cp0 regs */
                    break;
                case 0 ... 11:
                    CTRL_CP0_UNIT.o_wcp0_regs_sel = CTRL_CP0_UNIT.rd | 0xffff0000;
                    /* 为防止出现默认的0,这里我把高16位置1,最后选寄存器时再&回来，我们只用到它的最低5位来选cp0 regs */
                    break;
                default:
                    break;
        }
        
    }
    void cp0_mfc0_ctrl(){
        // rt ← CPR[0,rd,sel]
        switch (CTRL_CP0_UNIT.rd)
        {
                case 12:// cp0 r12 status_reg
                    CTRL_CP0_UNIT.o_mfc0 = 12;
                    break;
                case 13:// cp0 r13 cause_reg
                    CTRL_CP0_UNIT.o_mfc0 = 13;
                    break;
                case 14:// cp0 r14 Exception Program Counter
                    CTRL_CP0_UNIT.o_mfc0 = 14;
                case 15 ... 31:
                    CTRL_CP0_UNIT.o_mfc0 = CTRL_CP0_UNIT.rd | 0xffff0000;
                    /* 为防止出现默认的0,这里我把高16位置1,最后选寄存器时再&回来，我们只用到它的最低5位来选cp0 regs */
                    break;
                case 0 ... 11:
                    CTRL_CP0_UNIT.o_mfc0 = CTRL_CP0_UNIT.rd | 0xffff0000;
                    /* 为防止出现默认的0,这里我把高16位置1,最后选寄存器时再&回来，我们只用到它的最低5位来选cp0 regs */
                    break;
                default:
                    break;
        }
        
    }
    void cp0_ctrl(){
        using namespace DECODE;
        CTRL_CP0_UNIT.rd = get_rd(CTRL_CP0_UNIT.cop0_ins);
        CTRL_CP0_UNIT.rt = get_rt(CTRL_CP0_UNIT.cop0_ins);
        uint32_t MTMF_rs = get_rs(CTRL_CP0_UNIT.cop0_ins);
        uint32_t funct = get_funct(CTRL_CP0_UNIT.cop0_ins);
        cp0_notsoimportantCases();
        switch (MTMF_rs)
        {
            case 0b00000:/* MF mfc0 */
                CTRL_UNIT.o_regrt = true;
                CTRL_UNIT.o_wreg = true;
                cp0_mfc0_ctrl();
                break;
            case 0b00100:/* MT mtc0 */
                CTRL_CP0_UNIT.o_mtc0 = true;
                cp0_mtc0_ctrl();
                break;
            default:
                break;
        }
        // x__err("%x %x",CTRL_CP0_UNIT.rt,CTRL_CP0_UNIT.rd);
        
    }

    void cp0_operations(uint32_t writeData){//ID stage
        // CPR[0, rd, sel] ← rt
        if(CTRL_CP0_UNIT.o_mtc0){
            if(CTRL_CP0_UNIT.o_wsta){
                R3000_CP0::cp0_regs.SR.raw = writeData; 
                return;
            }
            if(CTRL_CP0_UNIT.o_wcau){
                R3000_CP0::cp0_regs.CAUSE.raw = writeData; 
                return;
            }
            if(CTRL_CP0_UNIT.o_wepc){
                R3000_CP0::cp0_regs.EPC = writeData;
                return;
            }
            uint32_t rd_sel = CTRL_CP0_UNIT.o_wcp0_regs_sel & 0x0000ffff;//呼应上文
            rd_sel &= 0b11111;//32 cp0 regs
            R3000_CP0::cp0_regs.val[rd_sel] = writeData;// CPR[0, rd, sel] ← rt
        }
        using namespace R3000_CP0;
        if(CTRL_CP0_UNIT.o_rfe){
            uint32_t mode = cp0_regs.SR.raw & 0x3f;
            cp0_regs.SR.raw &= ~0x3f;
            cp0_regs.SR.raw |= mode >> 2; 
        }

    }
    // rt ← CPR[0,rd,sel]


    void hilo_operations_ID(uint32_t rs_writeData,uint32_t rt_data){//ID stage
        if(CTRL_UNIT.o_mtHI){// HI ← rs
            mirror_hilo::mirror_hi = rs_writeData;
        }
        if(CTRL_UNIT.o_mtLO){// LO ← rs
            mirror_hilo::mirror_lo = rs_writeData;
        }
        uint32_t src1 = rs_writeData;
        uint32_t src2 = rt_data;
        uint64_t product64;
        switch (CTRL_UNIT.o_aluc)
        {
            case ALU_MULT:
                product64 = MultiplyUnit::Mult(src1,src2);
                mirror_hilo::mirror_hi = product64 >> 32;
                mirror_hilo::mirror_lo = product64 & 0xffffffff;
                break;
            case ALU_MULTU:
                product64 = MultiplyUnit::Multu(src1,src2);
                mirror_hilo::mirror_hi = product64 >> 32;
                mirror_hilo::mirror_lo = product64 & 0xffffffff;
                break;
            case ALU_DIV:
                DivideUnit::Div(src1,src2);
                mirror_hilo::mirror_hi = DivideUnit::get_hi();
                mirror_hilo::mirror_lo = DivideUnit::get_lo();
                break;
            case ALU_DIVU:
                DivideUnit::Divu(src1,src2);
                mirror_hilo::mirror_hi = DivideUnit::get_hi();
                mirror_hilo::mirror_lo = DivideUnit::get_lo();
                break;
            default:
                break;
        }
    }

    void clear_ctrl(){
        CTRL_UNIT.o_wreg=false;
        CTRL_UNIT.o_m2reg=false;
        CTRL_UNIT.o_wmem=false;
        CTRL_UNIT.o_jal=false;
        CTRL_UNIT.o_link=false;
        CTRL_UNIT.o_aluimm=false;
        CTRL_UNIT.o_shift=false;
        CTRL_UNIT.o_regrt=false;
        CTRL_UNIT.o_sext=false;
        CTRL_UNIT.o_pcsrc=0b00;
        // CTRL_UNIT.o_selpc=0b00;// default npc
        // CTRL_UNIT.o_wpcir=0b00;
        CTRL_UNIT.o_stall=false;
        CTRL_UNIT.o_sl_width_sel = 0b00;
        CTRL_UNIT.o_lbu = false;
        CTRL_UNIT.o_lhu = false;
        CTRL_UNIT.o_mfHI = false;
        CTRL_UNIT.o_mfLO = false;
        CTRL_UNIT.o_mtHI = false;
        CTRL_UNIT.o_mtLO = false;
        CTRL_UNIT.o_lwl = false;
        CTRL_UNIT.o_lwr = false;
        CTRL_UNIT.o_swl = false;
        CTRL_UNIT.o_swr = false;
        CTRL_CP0_UNIT.o_cancel = false;
        CTRL_CP0_UNIT.o_isbr = false;
        CTRL_CP0_UNIT.o_ove = false;
        CTRL_CP0_UNIT.o_wsta = false;
        CTRL_CP0_UNIT.o_wcau = false;
        CTRL_CP0_UNIT.o_wepc = false;
        CTRL_CP0_UNIT.o_wcp0_regs_sel = 0x0;
        CTRL_CP0_UNIT.o_mtc0 = false;
        CTRL_CP0_UNIT.o_mfc0 = 0x0;
        CTRL_CP0_UNIT.o_cause = 0x0;
        CTRL_CP0_UNIT.o_sepc = 0;
        CTRL_CP0_UNIT.o_exc = false;
        CTRL_CP0_UNIT.o_rfe = false;
    }

    void Control(){
        clear_ctrl();
        CTRL_UNIT.i_mrn = CrossPipelineWires::MEM_mrn;
        CTRL_UNIT.i_mm2reg = CrossPipelineWires::MEM_mm2reg;
        CTRL_UNIT.i_mwreg = CrossPipelineWires::MEM_mwreg;
        CTRL_UNIT.i_ern = CrossPipelineWires::EX_ern;
        CTRL_UNIT.i_em2reg = CrossPipelineWires::EX_em2reg;
        CTRL_UNIT.i_ewreg = CrossPipelineWires::EX_ewreg;
        CTRL_UNIT.o_selpc=0b00;// set default value : select npc
        CTRL_UNIT.o_stall=false;// set default value : no stall
        ALUOP_t ALUOP;
        switch (CTRL_UNIT.op)
        {
            case 0b000000:/* R-Type */
                CTRL_UNIT.o_wreg = true;
                CTRL_UNIT.o_m2reg = false;
                CTRL_UNIT.o_regrt = false;
                switch (CTRL_UNIT.funct)
                {
                    case 0b100000:/* add */
                        ALUOP = ALU_ADD;
                        break;
                    case 0b100001:/* addu */
                        ALUOP = ALU_ADDU;
                        break;
                    case 0b100100:/* and */
                        ALUOP = ALU_AND;
                        break;
                    case 0b100111:/* nor */
                        ALUOP = ALU_NOR;
                        break;
                    case 0b100101:/* or */
                        ALUOP = ALU_OR;
                        break;
                    case 0b101010:/* slt */
                        ALUOP = ALU_SLT;
                        break;
                    case 0b101011:/* sltu */
                        ALUOP = ALU_SLTU;
                        break;
                    case 0b100010:/* sub */
                        ALUOP = ALU_SUB;
                        break;
                    case 0b100011:/* subu*/
                        ALUOP = ALU_SUBU;
                        break;
                    case 0b100110:/* xor */
                        ALUOP = ALU_XOR;
                        break;

                    case 0b000000:/* sll */
                        CTRL_UNIT.o_shift = true;// come from immediate
                        ALUOP = ALU_SLL;
                        break;
                    case 0b000100:/* sllv */
                        ALUOP = ALU_SLL;
                        break;
                    case 0b000011:/* sra */
                        CTRL_UNIT.o_shift = true;// come from immediate
                        ALUOP = ALU_SRA;
                        break;
                    case 0b000111:/* srav */
                        ALUOP = ALU_SRA;
                        break;
                    case 0b000010:/* srl */
                        CTRL_UNIT.o_shift = true;// come from immediate 
                        ALUOP = ALU_SRL;
                        break;
                    case 0b000110:/* srlv */
                        ALUOP = ALU_SRL;
                        break;
                    case 0b011010:/* div */
                        ALUOP = ALU_DIV;
                        break; 
                    case 0b011011:/* divu */
                        ALUOP = ALU_DIVU;
                        break;
                    case 0b010000:/* mfhi */
                        CTRL_UNIT.o_mfHI = true;
                        CTRL_UNIT.o_wreg = true;
                        break;
                    case 0b010010:/* mflo */
                        CTRL_UNIT.o_mfLO = true;
                        CTRL_UNIT.o_wreg = true;
                        break;
                    case 0b010001:/* mthi */
                        CTRL_UNIT.o_mtHI = true;
                        //CTRL_UNIT.o_wreg = false; rd域是0,所以即便是写Reg为true也没关系，这里默认true,(考虑到forward问题)
                        break;
                    case 0b010011:/* mtlo */
                        CTRL_UNIT.o_mtLO = true;
                        //CTRL_UNIT.o_wreg = false; rd域是0,所以即便是写Reg为true也没关系，这里默认true,(考虑到forward问题)
                        break;
                    case 0b011000:/* mult */
                        ALUOP = ALU_MULT;
                        break;
                    case 0b011001:/* multu */
                        ALUOP = ALU_MULTU;
                        break;

                    case 0b001000:/* jr */
                        CTRL_UNIT.o_pcsrc = 0b10;
                        /* CTRL_UNIT.o_wreg = false; R type but do not write Reg*/

                        break;
                    case 0b001001:/* jalr */
                        CTRL_UNIT.o_jal = false;/*不变成31.而是直接rd*/
                        CTRL_UNIT.o_link = true;
                        CTRL_UNIT.o_wreg = true;
                        CTRL_UNIT.o_regrt = false;
                        CTRL_UNIT.o_pcsrc = 0b10;
                        break;  

                    case 0b001100:/* syscall */  
                        CTRL_CP0_UNIT.o_exc = true;
                        CTRL_CP0_UNIT.o_cancel = true;
                        CTRL_UNIT.o_selpc = 0b10;// 00: npc; 01: epc; 10: exc_base
                        CTRL_CP0_UNIT.o_cause = CP0_CauseReg_EXECODE_Field::SYS << 2;//syscall
                        CTRL_CP0_UNIT.o_sepc = 0b01;// 0b00: pc; 0b01: pcd; 0b10: pce; 0b11: pcm
                        // The PCD (the address of the syscall instruction) is saved into the EPC.
// the syscall instruction saves its own address into the EPC and has no delay slot: its follow-up instruction is always canceled.
                        CTRL_CP0_UNIT.o_wsta = true;// cp0 r12 status_reg
                        CTRL_CP0_UNIT.o_wcau = true;// cp0 r13 cause_reg
                        CTRL_CP0_UNIT.o_wepc = true;// cp0 r14 Exception Program Counter
// In our implementation, it is not allowed to put the syscall instruction in a delay slot.
                        break;
                    case 0b001101:/* break*/
                        CTRL_CP0_UNIT.o_exc = true;
                        CTRL_CP0_UNIT.o_cancel = true;
                        CTRL_UNIT.o_selpc = 0b10;
                        CTRL_CP0_UNIT.o_cause = CP0_CauseReg_EXECODE_Field::BP << 2;
                        CTRL_CP0_UNIT.o_sepc = 0b01;
                        CTRL_CP0_UNIT.o_wsta = true;
                        CTRL_CP0_UNIT.o_wcau = true;
                        CTRL_CP0_UNIT.o_wepc = true;                        
                        break;
                    default:
                        break;
                }//funct switch end 
                break;

            case 0b000010:/* j */
                CTRL_UNIT.o_pcsrc = 0b11;
                break;
            case 0b000011:/* jal */
                CTRL_UNIT.o_pcsrc = 0b11;
                CTRL_UNIT.o_wreg = true;
                CTRL_UNIT.o_jal = true;
                CTRL_UNIT.o_link = true;
                break;
            case 0b100000:/* lb */
                ALUOP = ALU_ADD;
                CTRL_UNIT.o_regrt = true;
                CTRL_UNIT.o_m2reg = true;
                CTRL_UNIT.o_aluimm = true;
                CTRL_UNIT.o_wreg = true;
                CTRL_UNIT.o_sext = true;
                CTRL_UNIT.o_sl_width_sel = 0b10;// select byte width 8
                break;
            case 0b100010:/* lwl */
                CTRL_UNIT.o_lwl = true;
                ALUOP = ALU_ADD;
                CTRL_UNIT.o_regrt = true;
                CTRL_UNIT.o_m2reg = true;
                CTRL_UNIT.o_aluimm = true;
                CTRL_UNIT.o_wreg = true;
                CTRL_UNIT.o_sext = true;
                x__err("fuck lwl %d",mipsx_cycle);
                // exit(0);
                break;
            case 0b100110:/* lwr */
                CTRL_UNIT.o_lwr = true;
                ALUOP = ALU_ADD;
                CTRL_UNIT.o_regrt = true;
                CTRL_UNIT.o_m2reg = true;
                CTRL_UNIT.o_aluimm = true;
                CTRL_UNIT.o_wreg = true;
                CTRL_UNIT.o_sext = true;
                x__err("fuck lwr %d",mipsx_cycle);
                exit(0);
                break;
            case 0b101010:/* swl */
                CTRL_UNIT.o_swl = true;
                CTRL_UNIT.o_aluimm = true;
                CTRL_UNIT.o_wmem = true;
                CTRL_UNIT.o_sext = true;
                ALUOP = ALU_ADD;
                x__err("fuck swl %d",mipsx_cycle);
                break;
            case 0b101110:/* swr */
                CTRL_UNIT.o_swr = true;
                CTRL_UNIT.o_aluimm = true;
                CTRL_UNIT.o_wmem = true;
                CTRL_UNIT.o_sext = true;
                ALUOP = ALU_ADD;
                x__err("fuck swr %d",mipsx_cycle);
                break;

            
            case 0b100100:/* lbu */
                ALUOP = ALU_ADD;
                CTRL_UNIT.o_regrt = true;
                CTRL_UNIT.o_m2reg = true;
                CTRL_UNIT.o_aluimm = true;
                CTRL_UNIT.o_wreg = true;
                CTRL_UNIT.o_sext = true;/*地址还是要signextend的，但取出来的八bit位是zero-extend*/
                CTRL_UNIT.o_lbu = true;/* 控制the fetched value 是 zero-extend */
                CTRL_UNIT.o_sl_width_sel = 0b10;// select byte width 8
                break;
            case 0b100001:/* lh */
                ALUOP = ALU_ADD;
                CTRL_UNIT.o_regrt = true;
                CTRL_UNIT.o_m2reg = true;
                CTRL_UNIT.o_aluimm = true;
                CTRL_UNIT.o_wreg = true;
                CTRL_UNIT.o_sext = true;
                CTRL_UNIT.o_sl_width_sel = 0b01;// select half word width 16
                break;
            case 0b100101:/* lhu */
                ALUOP = ALU_ADD;
                CTRL_UNIT.o_regrt = true;
                CTRL_UNIT.o_m2reg = true;
                CTRL_UNIT.o_aluimm = true;
                CTRL_UNIT.o_wreg = true;
                CTRL_UNIT.o_sext = true;/* 保留 */
                CTRL_UNIT.o_lhu = true;/* 控制the fetched value 是 zero-extend */
                CTRL_UNIT.o_sl_width_sel = 0b01;// select half word width 16
                break;
            case 0b100011:/* lw */
                ALUOP = ALU_ADD;
                CTRL_UNIT.o_regrt = true;
                CTRL_UNIT.o_m2reg = true;
                CTRL_UNIT.o_aluimm = true;
                CTRL_UNIT.o_wreg = true;
                CTRL_UNIT.o_sext = true;
                break;
            case 0b101000:/* sb */
                CTRL_UNIT.o_aluimm = true;
                CTRL_UNIT.o_wmem = true;
                CTRL_UNIT.o_sext = true;
                CTRL_UNIT.o_sl_width_sel = 0b10;// select byte width 8
                ALUOP = ALU_ADD;
                break;
            case 0b101001:/* sh */
                CTRL_UNIT.o_aluimm = true;
                CTRL_UNIT.o_wmem = true;
                CTRL_UNIT.o_sext = true;
                CTRL_UNIT.o_sl_width_sel = 0b01;// select half word width 16
                ALUOP = ALU_ADD;
                break;
            case 0b101011:/* sw */
                CTRL_UNIT.o_aluimm = true;
                CTRL_UNIT.o_wmem = true;
                CTRL_UNIT.o_sext = true;
                ALUOP = ALU_ADD;
                break;
            case 0b010000:// cop0 operations
                // x__err("unhandled cp0");
                cp0_ctrl();
                break;
            case 0b010001:// cop1 operations
                break;
            case 0b010010:// cop2 operations
                break;
            case 0b000100:/* beq */
                CTRL_UNIT.o_sext = true;
                if(CTRL_UNIT.i_rsrtequ)
                    CTRL_UNIT.o_pcsrc = 0b01;
                break;
            case 0b000101:/* bne */
                CTRL_UNIT.o_sext = true;
                if(!CTRL_UNIT.i_rsrtequ)
                    CTRL_UNIT.o_pcsrc = 0b01;
                break;
            case 0b000001:/* Zero-relative branches */
                switch (CTRL_UNIT.rt)//sign_extend(offset || 0 2 )
                {// the six MSBs are 0b000001 which can encode four different instructions:
                    case 0b00000:/* bltz */
                        CTRL_UNIT.o_sext = true;
                        if(CTRL_UNIT.i_rsLTZ)
                            CTRL_UNIT.o_pcsrc = 0b01;                        
                        break;
                    case 0b10000:/* bltzal */
                        CTRL_UNIT.o_sext = true;
                        CTRL_UNIT.o_wreg = true;/* and link */
                        if(CTRL_UNIT.i_rsLTZ)
                            CTRL_UNIT.o_pcsrc = 0b01;
                        break;
                    case 0b00001:/* bgez */
                        CTRL_UNIT.o_sext = true;
                        if(CTRL_UNIT.i_rsGEZ)
                            CTRL_UNIT.o_pcsrc = 0b01;
                        break;
                    case 0b10001:/* bgezal */
                        CTRL_UNIT.o_sext = true;
                        CTRL_UNIT.o_wreg = true;/* and link */
                        if(CTRL_UNIT.i_rsGEZ)
                            CTRL_UNIT.o_pcsrc = 0b01;
                        break;
                    default:
                        break;
                }
                break;
            case 0b000111:/* bgtz */
                CTRL_UNIT.o_sext = true;
                if(CTRL_UNIT.i_rsGTZ)
                    CTRL_UNIT.o_pcsrc = 0b01;
                break;
            case 0b000110:/* blez */
                CTRL_UNIT.o_sext = true;
                if(CTRL_UNIT.i_rsLEZ)
                    CTRL_UNIT.o_pcsrc = 0b01;
                break;



            default:/* I-Type */
                switch (CTRL_UNIT.op)
                {
                    case 0b001000:/* addi */
                        CTRL_UNIT.o_regrt = true;
                        CTRL_UNIT.o_aluimm = true;
                        CTRL_UNIT.o_sext = true;
                        CTRL_UNIT.o_wreg = true;
                        ALUOP = ALU_ADD;
                        break;
                    case 0b001001:/* addiu */
                        CTRL_UNIT.o_regrt = true;
                        CTRL_UNIT.o_aluimm = true;
                        CTRL_UNIT.o_sext = true;
                        CTRL_UNIT.o_wreg = true;
                        ALUOP = ALU_ADD;
                        break;
                    case 0b001100:/* andi */
                        CTRL_UNIT.o_regrt = true;
                        CTRL_UNIT.o_aluimm = true;
                        CTRL_UNIT.o_wreg = true;
                        ALUOP = ALU_AND;
                        break;
                    case 0b001111:/* lui */
                        CTRL_UNIT.o_regrt = true;
                        CTRL_UNIT.o_aluimm = true;
                        CTRL_UNIT.o_wreg = true;
                        ALUOP = ALU_LU;
                        break;
                    case 0b001101:/* ori */
                        CTRL_UNIT.o_regrt = true;
                        CTRL_UNIT.o_aluimm = true;
                        CTRL_UNIT.o_wreg = true;
                        ALUOP = ALU_OR;
                        break;
                    case 0b001010:/* slti */
                        CTRL_UNIT.o_sext = true;// Compare the contents of GPR rs and the 16-bit signed immediate as signed integers
                        CTRL_UNIT.o_regrt = true;
                        CTRL_UNIT.o_aluimm = true;
                        CTRL_UNIT.o_wreg = true;
                        ALUOP = ALU_SLT;
                        break;
                    case 0b001011:/* sltiu*/
                        CTRL_UNIT.o_regrt = true;
                        CTRL_UNIT.o_aluimm = true;
                        CTRL_UNIT.o_wreg = true;
                        ALUOP = ALU_SLTU;
                        break;
                    case 0b001110:/* xori */
                        CTRL_UNIT.o_regrt = true;
                        CTRL_UNIT.o_aluimm = true;
                        CTRL_UNIT.o_wreg = true;
                        // x__err("xorri %d",mipsx_cycle);
                        ALUOP = ALU_XOR;
                        // GPR[rt] ← GPR[rs] xor zero_extend(immediate)
                        break;
                    default:
                        break;
                }
                break;
        }// op switch end
        CTRL_UNIT.o_aluc = ALUOP;
        using  namespace CrossPipelineWires;
// The signal cancel generated in the ID stage is stored into an ID/EXE pipeline register 
// whose output e_cancel is used for canceling the next instruction to syscall
        CTRL_UNIT.o_pcsrc = (EX_cancel) ? 0b00 : CTRL_UNIT.o_pcsrc;
        CTRL_UNIT.o_wmem = (EX_cancel) ? false : CTRL_UNIT.o_wmem;
        CTRL_UNIT.o_wreg = (EX_cancel) ? false : CTRL_UNIT.o_wreg; 
        using namespace ForwardingUnit;
        CTRL_UNIT.o_fwda = calcuforwardA(CTRL_UNIT.rs);
        CTRL_UNIT.o_fwdb = calcuforwardB(CTRL_UNIT.rt);

        if(CTRL_CP0_UNIT.i_irq){
            CTRL_UNIT.o_wmem = false;
            CTRL_UNIT.o_wreg = false;
            CTRL_UNIT.o_selpc = 0b10;// base
            CTRL_CP0_UNIT.o_sepc = 0b11;//pcm
            CTRL_CP0_UNIT.o_exc = true;
            CTRL_CP0_UNIT.o_cancel = true;/*目的不是改变PC,而是让他不写*/
            ID_EX.ewmem = false;// cancel 0x80050f28 0x3c018008
            ID_EX.ewreg = false;
            EX_MEM.mwmem = false;// cancel 0x80050f24 0x8d08929c
            EX_MEM.mwreg = false;// cancel 0x80050f24 0x8d08929c
            MEM_WB.wwreg = false;// cancel 0x80050f20 0x3c088008
            // x__err("fjiba %x %x %x",CTRL_UNIT.o_selpc,ID_EX.IR,R3000_CP0::cp0_regs.EPC);
        }
    }

}

