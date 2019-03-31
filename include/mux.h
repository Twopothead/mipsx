#pragma once
#include <inttypes.h>
namespace Multiplexer{
    namespace IFMUX{
        struct {
            uint32_t pc4;
            uint32_t bpc;
            uint32_t da;
            uint32_t jpc;
            uint32_t o_npc;
        }PCSRC_MUX;
        void setPCSRC_MUX(const uint32_t sel_pcsrc,
                    uint32_t pc4,uint32_t bpc,uint32_t da,uint32_t jpc){
            PCSRC_MUX.pc4 = pc4,PCSRC_MUX.bpc=bpc,PCSRC_MUX.da=da,PCSRC_MUX.jpc=jpc;
            switch (sel_pcsrc)
            {
                case 0b00:
                    PCSRC_MUX.o_npc = PCSRC_MUX.pc4;
                    break;
                case 0b01:
                    PCSRC_MUX.o_npc = PCSRC_MUX.bpc;
                    break;
                case 0b10:
                    PCSRC_MUX.o_npc = PCSRC_MUX.da;
                    break;
                case 0b11:
                    PCSRC_MUX.o_npc = PCSRC_MUX.jpc;
                    break;
                default:
                    break;
            }

        }

    }
    namespace IDMUX{
        struct {
            uint32_t rd;
            uint32_t rt;
            uint32_t o_drn; 
        }REGRT_MUX;
        void setREGRT_MUX(const bool sel_regrt,
                        const uint32_t &rd,const uint32_t &rt){
            REGRT_MUX.rd = rd,REGRT_MUX.rt = rt;
            if(sel_regrt)
                REGRT_MUX.o_drn = rt;
            else
                REGRT_MUX.o_drn = rd;
        }
        struct FWDA_MUX_t{
            uint32_t qa;
            uint32_t exe_alu;
            uint32_t mem_alu;
            uint32_t mem_lw;
            uint32_t o_ID_a;
        }FWDA_MUX;
        void setFWDA_MUX(const uint32_t sel_fwada,
                uint32_t &qa,uint32_t &exe_alu,uint32_t &mem_alu,uint32_t &mem_lw){
                FWDA_MUX.qa = qa;
                FWDA_MUX.exe_alu = exe_alu;
                FWDA_MUX.mem_alu = mem_alu;
                FWDA_MUX.mem_lw = mem_lw;
                switch (sel_fwada)
                {
                    case 0b00:
                        FWDA_MUX.o_ID_a = FWDA_MUX.qa;
                        break;
                    case 1:
                        FWDA_MUX.o_ID_a = FWDA_MUX.exe_alu;
                        break;
                    case 0b10:
                        FWDA_MUX.o_ID_a = FWDA_MUX.mem_alu;
                        break;
                    case 0b11:
                        FWDA_MUX.o_ID_a = FWDA_MUX.mem_lw;
                    default:
                        break;
                }
        }
        struct {
            uint32_t qb;
            uint32_t exe_alu;
            uint32_t mem_alu;
            uint32_t mem_lw;
            uint32_t o_ID_b;
        }FWDB_MUX;
        void setFWDB_MUX(const uint32_t sel_fwadb,
                uint32_t &qb,uint32_t &exe_alu,uint32_t &mem_alu,uint32_t &mem_lw){
                FWDB_MUX.qb = qb;
                FWDB_MUX.exe_alu = exe_alu;
                FWDB_MUX.mem_alu = mem_alu;
                FWDB_MUX.mem_lw = mem_lw;
                switch (sel_fwadb)
                {
                    case 0b00:
                        FWDB_MUX.o_ID_b = FWDB_MUX.qb;
                        break;
                    case 0b01:
                        FWDB_MUX.o_ID_b = FWDB_MUX.exe_alu;
                        break;
                    case 0b10:
                        FWDB_MUX.o_ID_b = FWDB_MUX.mem_alu;
                        break;
                    case 0b11:
                        FWDB_MUX.o_ID_b = FWDB_MUX.mem_lw;
                    default:
                        break;
                }
        }
    }
namespace EXEMUX{
        struct {
            uint32_t sa;
            uint32_t ea;
            uint32_t o_src1;
        }ESHIFT_MUX;
        void setESHIFT_MUX(const bool sel_eshift,
            uint32_t &sa,uint32_t &ea){
                ESHIFT_MUX.sa = sa;
                ESHIFT_MUX.ea = ea;
                if(sel_eshift)
                    ESHIFT_MUX.o_src1 = ESHIFT_MUX.sa;
                else
                    ESHIFT_MUX.o_src1 = ESHIFT_MUX.ea;
        }
        struct {
            uint32_t _eimm;
            uint32_t eb;
            uint32_t o_src2;
        }EALUIMM_MUX;
        void setEALUIMM_MUX(const bool sel_ealuimm,           
            uint32_t &_eimm,uint32_t &eb){
                EALUIMM_MUX._eimm = _eimm;
                EALUIMM_MUX.eb = eb;
                if(sel_ealuimm)
                    EALUIMM_MUX.o_src2 = EALUIMM_MUX._eimm;
                else
                    EALUIMM_MUX.o_src2 = EALUIMM_MUX.eb;
        }
        struct {
            uint32_t epc8;
            uint32_t eALUresult;
            uint32_t o_ealu;
        }EJAL_MUX;
        void setEJAL_MUX(const bool sel_ejal,
            uint32_t &epc8,uint32_t &eALUresult){
            EJAL_MUX.epc8 = epc8;
            EJAL_MUX.eALUresult = eALUresult;   
            if(sel_ejal)
                EJAL_MUX.o_ealu = EJAL_MUX.epc8;
            else
                EJAL_MUX.o_ealu = EJAL_MUX.eALUresult;
        }
    }
namespace WBMUX{
        struct {
            uint32_t wmo;
            uint32_t walu;
            uint32_t o_wdi;
        }WM2REG_MUX;
        void setWM2REG_MUX(const bool sel_wm2reg,
             uint32_t wmo,uint32_t walu){
                 WM2REG_MUX.wmo = wmo;
                 WM2REG_MUX.walu = walu;
                 if(sel_wm2reg)
                    WM2REG_MUX.o_wdi = wmo;
                 else
                    WM2REG_MUX.o_wdi = walu;
        }
    }
}