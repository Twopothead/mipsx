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
        namespace IF_CP0_M{
            struct {
                uint32_t npc;
                uint32_t epc;
                uint32_t base;
                uint32_t o_next_pc;
            }NEXTPC_MUX;
            void setNEXTPC_MUX(const uint32_t sel_pc,
                    uint32_t npc,uint32_t epc,uint32_t base){
                NEXTPC_MUX.npc = npc,NEXTPC_MUX.epc = epc,NEXTPC_MUX.base = base;        
                switch (sel_pc)
                {
                    case 0b00:
                        NEXTPC_MUX.o_next_pc = NEXTPC_MUX.npc;
                        break;
                    case 0b01:
                        NEXTPC_MUX.o_next_pc = NEXTPC_MUX.epc;
                        break;
                    case 0b10:// exception handler entry
                        NEXTPC_MUX.o_next_pc = NEXTPC_MUX.base;
                        break;
                    default:
                        break;
                }

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
        struct FWDB_MUX_t{
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
        namespace ID_CP0_M{
            struct SEPC_MUX_t{
                uint32_t pc;
                uint32_t pcd;
                uint32_t pce;
                uint32_t pcm;
                uint32_t o_epcin;
            }SEPC_MUX;
            void setSEPC_MUX(const uint32_t sel_sepc,
                    uint32_t pc,uint32_t pcd,uint32_t pce,uint32_t pcm){
                    SEPC_MUX.pc = pc,SEPC_MUX.pcd = pcd,SEPC_MUX.pce,SEPC_MUX.pcm;
                switch (sel_sepc)
                {
                    case 0b00:
                        SEPC_MUX.o_epcin = SEPC_MUX.pc;
                        break;
                    case 0b01:
                        SEPC_MUX.o_epcin = SEPC_MUX.pcd;
                        break;
                    case 0b10:
                        SEPC_MUX.o_epcin = SEPC_MUX.pce;
                        break;
                    case 0b11:
                        SEPC_MUX.o_epcin = SEPC_MUX.pcm;
                        break;
                    default:
                        break;
                }

            }
        }
    }
namespace EXEMUX{
        struct ESHIFT_MUX_t{
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
        struct EALUIMM_MUX_t{
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
        struct ELinkmfc0_MUX_t{
            uint32_t pc8c0r;
            uint32_t eALUresult;
            uint32_t o_ealu;
        }ELinkmfc0_MUX;
        void setELinkmfc0_MUX(const bool sel_elinkorMFC0,
            uint32_t &pc8c0r,uint32_t &eALUresult){
            ELinkmfc0_MUX.pc8c0r = pc8c0r;
            ELinkmfc0_MUX.eALUresult = eALUresult;   
            if(sel_elinkorMFC0)
                ELinkmfc0_MUX.o_ealu = ELinkmfc0_MUX.pc8c0r;
            else
                ELinkmfc0_MUX.o_ealu = ELinkmfc0_MUX.eALUresult;
        }

        struct EPC8_Cp0r_MUX_t{
            uint32_t epc8;// pc plus 8 in EX stage
            uint32_t sta;//cp0 r12
            uint32_t cau;//cp0 r13
            uint32_t epc;//cp0 r14 exception pc
            // considering taking up too much space,pipeline cp0 regs are not list here
            uint32_t o_pc8c0r_in;
        }EPC8_Cp0r_MUX;
        void setEPC8_Cp0r_MUX(const uint32_t mfc0_sel,
            uint32_t epc8,uint32_t &sta,uint32_t &cau,uint32_t &epc,uint32_t * pipeline_cp0_regs){
            EPC8_Cp0r_MUX.epc8 =  epc8;

            EPC8_Cp0r_MUX.sta = sta;
            EPC8_Cp0r_MUX.cau = cau;
            EPC8_Cp0r_MUX.epc = epc;
            // considering taking up too much space,ID_EX pipeline cp0 regs are not list here
            switch (mfc0_sel)
            {
                case 0:
                    EPC8_Cp0r_MUX.o_pc8c0r_in = EPC8_Cp0r_MUX.epc8;// pc plus 8 in EX stage
                    break;
                case 12:
                    EPC8_Cp0r_MUX.o_pc8c0r_in = EPC8_Cp0r_MUX.sta;
                    break;
                case 13:
                    EPC8_Cp0r_MUX.o_pc8c0r_in = EPC8_Cp0r_MUX.cau;
                    break;
                case 14:
                    EPC8_Cp0r_MUX.o_pc8c0r_in = EPC8_Cp0r_MUX.epc;
                    break;
                case 0xffff0000 ... 0xffff001f:
                    EPC8_Cp0r_MUX.o_pc8c0r_in = pipeline_cp0_regs[mfc0_sel & 0b11111];// 0 to 31
                default:
                    break;
            }

        }


        
        

    }
namespace MEMMUX{
    struct MWIDTH_MUX_t{
            const uint32_t word_width = 32;  
            const uint32_t half_word_width = 16;
            const uint32_t byte_width = 8;
            uint32_t o_load_store_width = 32;// default initial value = 32
        }MWIDTH_MUX;
    void setMWIDTH_MUX(const uint32_t storeload_width_sel){
            switch (storeload_width_sel)
            {
                case 0b00:// word 32
                    MWIDTH_MUX.o_load_store_width = MWIDTH_MUX.word_width;
                    break;
                case 0b01:// half word 16
                    MWIDTH_MUX.o_load_store_width = MWIDTH_MUX.half_word_width;
                    break;
                case 0b10:// byte 8
                    MWIDTH_MUX.o_load_store_width = MWIDTH_MUX.byte_width;
                    break;
                default:// default 32
                    MWIDTH_MUX.o_load_store_width = MWIDTH_MUX.word_width;
                    break;
            }

    }
}
namespace WBMUX{
        struct WM2REG_MUX_t{
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