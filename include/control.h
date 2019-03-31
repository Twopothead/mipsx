
#pragma once
#include <inttypes.h>
#include "alu.h"
#include "wires.h"
#include "forward.h"
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
        ALUOP_t  o_aluc;
        bool o_aluimm;
        bool o_shift;
        bool o_regrt;
        bool i_rsrtequ;
        bool o_sext;
        uint32_t o_fwda;
        uint32_t o_fwdb;
        uint32_t op;
        uint32_t funct;
        uint32_t rs;
        uint32_t rt;
        uint32_t o_pcsrc;
        uint32_t o_wpcir;
    }CTRL_UNIT;
    void clear_ctrl(){
        CTRL_UNIT.o_wreg=false;
        CTRL_UNIT.o_m2reg=false;
        CTRL_UNIT.o_wmem=false;
        CTRL_UNIT.o_jal=false;
        CTRL_UNIT.o_aluimm=false;
        CTRL_UNIT.o_shift=false;
        CTRL_UNIT.o_regrt=false;
        CTRL_UNIT.o_sext=false;
        CTRL_UNIT.o_pcsrc=0b00;
        CTRL_UNIT.o_wpcir=0b00;
        CTRL_UNIT.o_pcsrc=0b00;
        CTRL_UNIT.o_wpcir=0b00;
    }
    void Control(){
        clear_ctrl();
        CTRL_UNIT.i_mrn = CrossPipelineWires::MEM_mrn;
        CTRL_UNIT.i_mm2reg = CrossPipelineWires::MEM_mm2reg;
        CTRL_UNIT.i_mwreg = CrossPipelineWires::MEM_mwreg;
        CTRL_UNIT.i_ern = CrossPipelineWires::EX_ern;
        CTRL_UNIT.i_em2reg = CrossPipelineWires::EX_em2reg;
        CTRL_UNIT.i_ewreg = CrossPipelineWires::EX_ewreg;
        ALUOP_t ALUOP;
        switch (CTRL_UNIT.op)
        {
            case 0b000000:/* R-Type */
                CTRL_UNIT.o_wreg = true;
                CTRL_UNIT.o_m2reg = false;
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
                        CTRL_UNIT.o_shift = true;
                        ALUOP = ALU_SLL;
                        break;
                    case 0b000100:/* sllv */
                        ALUOP = ALU_SLL;
                        break;
                    case 0b000011:/* sra */
                        CTRL_UNIT.o_shift = true;
                        ALUOP = ALU_SRA;
                        break;
                    case 0b000111:/* srav */
                        ALUOP = ALU_SRA;
                        break;
                    case 0b000010:/* srl */
                        CTRL_UNIT.o_shift = true; 
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
                        ALUOP = ALU_MFHI;
                        break;
                    case 0b010010:/* mflo */
                        ALUOP = ALU_MFLO;
                        break;
                    case 0b010001:/* mthi */
                        ALUOP = ALU_MTHI;
                        break;
                    case 0b010011:/* mtlo */
                        ALUOP = ALU_MTLO;
                        break;
                    case 0b011000:/* mult */
                        ALUOP = ALU_MULT;
                        break;
                    case 0b011001:/* multu */
                        ALUOP = ALU_MULTU;
                        break;

                    case 0b001000:/* jr */
                        CTRL_UNIT.o_pcsrc = 0b10;
                        break;
                    case 0b001001:/* jalr */
                        CTRL_UNIT.o_jal = true;
                        CTRL_UNIT.o_wreg = true;
                        CTRL_UNIT.o_pcsrc = 0b11;
                        break;                
                        default:
                            break;
                }//funct switch end 
                break;

            case 0b000010:/* j */
                CTRL_UNIT.o_pcsrc = 0b11;
                
                break;
            case 0b000011:/* jal */
                CTRL_UNIT.o_wreg = true;
                break;
            case 0b100000:/* lb */
                ALUOP = ALU_ADD;
                CTRL_UNIT.o_regrt = true;
                CTRL_UNIT.o_m2reg = true;
                CTRL_UNIT.o_aluimm = true;
                CTRL_UNIT.o_wreg = true;
                CTRL_UNIT.o_sext = true;
                break;
            case 0b100100:/* lbu */
                ALUOP = ALU_ADD;
                CTRL_UNIT.o_regrt = true;
                CTRL_UNIT.o_m2reg = true;
                CTRL_UNIT.o_aluimm = true;
                CTRL_UNIT.o_wreg = true;
                CTRL_UNIT.o_sext = true;
                break;
            case 0b100001:/* lh */
                ALUOP = ALU_ADD;
                CTRL_UNIT.o_regrt = true;
                CTRL_UNIT.o_m2reg = true;
                CTRL_UNIT.o_aluimm = true;
                CTRL_UNIT.o_wreg = true;
                CTRL_UNIT.o_sext = true;
                break;
            case 0b100101:/* lhu */
                ALUOP = ALU_ADD;
                CTRL_UNIT.o_regrt = true;
                CTRL_UNIT.o_m2reg = true;
                CTRL_UNIT.o_aluimm = true;
                CTRL_UNIT.o_wreg = true;
                CTRL_UNIT.o_sext = true;
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
                ALUOP = ALU_ADD;
                break;
            case 0b101001:/* sh */
                CTRL_UNIT.o_aluimm = true;
                CTRL_UNIT.o_wmem = true;
                CTRL_UNIT.o_sext = true;
                ALUOP = ALU_ADD;
                break;
            case 0b101011:/* sw */
                CTRL_UNIT.o_aluimm = true;
                CTRL_UNIT.o_wmem = true;
                CTRL_UNIT.o_sext = true;
                ALUOP = ALU_ADD;
                break;
            case 0b010000:// cop0 operations
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
                switch (CTRL_UNIT.rt)
                {
                    case 0b00000:/* bltz */
                        ALUOP = ALU_LTZ;
                        break;
                    case 0b10000:/* bltzal */
                        ALUOP = ALU_LTZ;
                        break;
                    case 0b00001:/* bgez */
                        ALUOP = ALU_GEZ;
                        break;
                    case 0b10001:/* bgezal */
                        ALUOP = ALU_GEZ;
                        break;
                    default:
                        break;
                }
                break;
            case 0b000111:/* bgtz */
                ALUOP = ALU_GTZ;
                break;
            case 0b000110:/* blez */
                ALUOP = ALU_LEZ;
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
                        CTRL_UNIT.o_regrt = true;
                        CTRL_UNIT.o_aluimm = true;
                        CTRL_UNIT.o_wreg = true;
                        ALUOP = ALU_SLT;
                        break;
                    case 0b001011:/* sltiu*/
                        CTRL_UNIT.o_regrt = true;
                        CTRL_UNIT.o_aluimm = true;
                        CTRL_UNIT.o_wreg = true;
                        ALUOP = ALU_SLT;
                        break;
                    case 0b001110:/* xori */
                        CTRL_UNIT.o_regrt = true;
                        CTRL_UNIT.o_aluimm = true;
                        CTRL_UNIT.o_wreg = true;
                        ALUOP = ALU_XOR;
                        break;
                    default:
                        break;
                }
                break;
        }// op switch end
        CTRL_UNIT.o_aluc = ALUOP;
        using namespace ForwardingUnit;
        CTRL_UNIT.o_fwda = calcuforwardA(CTRL_UNIT.rs);
        CTRL_UNIT.o_fwdb = calcuforwardB(CTRL_UNIT.rt);
    }
}