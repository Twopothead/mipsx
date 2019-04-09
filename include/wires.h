#pragma once
#include <inttypes.h>
namespace CrossPipelineWires
{
    uint32_t next_pc;/* Do not clear it */
    uint32_t IF_npc;
    uint32_t ID_selpc;
    uint32_t ID_bpc;
    uint32_t ID_da;
    uint32_t ID_jpc;
    bool ID_rsrt_equ;
    uint32_t ID_pcsrc;
    bool ID_wpcir;
    uint32_t EX_ern;
    bool EX_em2reg;
    bool EX_ewreg;
    uint32_t EX_ealu;
    uint32_t MEM_mrn;
    bool MEM_mm2reg;
    bool MEM_mwreg;
    uint32_t MEM_malu;
    uint32_t MEM_mmo;
    uint32_t IF_pc;
    uint32_t ID_pcd;
    uint32_t EX_pce;
    uint32_t MEM_pcm;
    bool EX_cancel;
    void clearCrossPipelineWires(){
        IF_npc = 0,ID_selpc = 0,ID_bpc = 0,ID_da = 0,ID_jpc = 0;
        ID_rsrt_equ = false;
        ID_pcsrc = 0,ID_wpcir = 0,EX_ern = 0;
        EX_em2reg = false,EX_ewreg = false;
        EX_ealu = 0,MEM_mrn = 0;
        MEM_mm2reg = false,MEM_mwreg = false;
        MEM_malu = 0,MEM_mmo = 0;
        IF_pc = 0,ID_pcd = 0,EX_pce = 0,MEM_pcm = 0;
        EX_cancel = false;
    }
} 