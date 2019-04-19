#pragma once
#include <inttypes.h>
#include <cstring>
#include "alu.h"
#include "pipeline_regs.h"
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
        bool intr;
    } IF_ID;
    struct ID_EX_t
    {
        uint32_t IR;
        bool ewreg;
        bool em2reg;
        bool ewmem;
        bool ejal;
        bool elink;
        bool elbu;
        bool elhu;
        ALUOP_t ealuc;
        bool ealuimm;
        bool eshift;
        uint32_t epc4;
        uint32_t ea;
        uint32_t eb;
        uint32_t eimm;
        uint32_t ern0;
        uint32_t esl_width_sel;
        uint32_t emfc0;
        bool emfHI;
        bool emfLO;
        bool ewriteHILO;
        bool ecancel;
        bool elwl;
        bool elwr;
        bool eswl;
        bool eswr;
        uint32_t non_aligned_use_tregvalue;
        uint32_t pipeline_cp0_regs[31];/*注意，这只是pipeling registers里暂存的cp0 regs，不是cp0 regs本身*/
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
        uint32_t msl_width_sel;
        bool mlbu;
        bool mlhu;
        bool mwriteHILO;
        bool mlwl;
        bool mlwr;
        bool mswl;
        bool mswr;
        uint32_t non_aligned_use_tregvalue;
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
        bool writeHILO;
        bool wlwl;
        bool wlwr;
        uint32_t non_aligned_use_tregvalue;
        uint32_t non_aligned_memaddr;
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