#pragma once
#include <inttypes.h>
#include <cstring>
#include "alu.h"
namespace pipeline_registers
{
    struct Pre_IF_t{
        uint32_t PC;
    }Pre_IF;
    struct IF_ID_t
    {
        uint32_t IR;
        uint32_t dpc4;
        bool dwpcir;
        uint32_t PCd;
    } IF_ID;
    struct ID_EX_t
    {
        uint32_t IR;
        bool ewreg;
        bool em2reg;
        bool ewmem;
        bool ejal;
        ALUOP_t ealuc;
        bool ealuimm;
        bool eshift;
        uint32_t epc4;
        uint32_t ea;
        uint32_t eb;
        uint32_t eimm;
        uint32_t ern0;
        uint32_t PCe;
    } ID_EX;
    struct EX_MEM_t
    {
        uint32_t IR;
        bool mwreg;
        bool mm2reg;
        bool mwmem;
        uint32_t malu;
        uint32_t mb;
        uint32_t mrn;
        uint32_t PCm;
    } EX_MEM;
    struct MEM_WB_t
    {
        uint32_t IR;
        bool wwreg;
        bool wm2reg;
        uint32_t wmo;
        uint32_t walu;
        uint32_t wrn;
        uint32_t debug_wbPC;
    } MEM_WB;
    void clear_pipeline_registers()
    {
        memset(&IF_ID, 0, sizeof(IF_ID));
        memset(&ID_EX, 0, sizeof(ID_EX));
        memset(&EX_MEM, 0, sizeof(EX_MEM));
        memset(&MEM_WB, 0, sizeof(MEM_WB));
        Pre_IF.PC = 0x00000000;
    }
}