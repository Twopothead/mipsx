#pragma once
#include "bitwise.h"
#include "signals.h"
#include "memlayout.h"
#include "memory.h"
#include "cpu.h"
#include "wires.h"
#include "alu.h"
#include "mux.h"
#include "pipeline_regs.h"
#include "control.h"
#include "debug.h"
#include "cp0.h"
#include "forward.h"

namespace pr = pipeline_registers;

class MIPSX_SYSTEM
{
    R3000A_CPU cpu;
    PlayStationMemory memory;

  public:
    MIPSX_SYSTEM()
    {
        using namespace PlayStationMemLayout;
        pipeline_registers::clear_pipeline_registers();
        memset(&R3000_CP0::cp0_regs,0,sizeof(R3000_CP0::cp0_regs));
        pipeline_registers::Pre_IF.PC = start_Rom;
    };
    friend class Monitor;
    void IF(pr::Pre_IF_t &Pre_IF, pr::IF_ID_t &IF_ID)
    {
        using namespace IF_Signals;
        using namespace CrossPipelineWires;
        using namespace Multiplexer::IFMUX;
        clear_temp_IF_signals();
        uint32_t pc =  Pre_IF.PC;
        IF_pc = pc;
        IF_ID.IR = memory.read<uint32_t>(pc);
        // if(IF_ID.IR==0x8d090000)
        //     while(1){
        //         ;
        //     }
            
        pc4 = pc + 4;
        IF_ID.dpc4 = pc4;
        setPCSRC_MUX(ID_pcsrc,pc4,ID_bpc,ID_da,ID_jpc);
        npc = PCSRC_MUX.o_npc;
        IF_npc = npc;
        IF_ID.PCd = pc;
        printf("IF %08x\t", IF_ID.IR);
        using namespace R3000_CP0;
        return;
    }
    void ID(pr::IF_ID_t &IF_ID, pr::ID_EX_t &ID_EX)
    {
        using namespace ID_Signals;
        using namespace CrossPipelineWires;
        using namespace DECODE;
        using namespace Multiplexer::IDMUX;
        using namespace CONTROL;
        clear_ID_temp_signals();
        op = get_op(IF_ID.IR);
        funct = get_funct(IF_ID.IR);
        rs = get_rs(IF_ID.IR);
        rt = get_rt(IF_ID.IR);
        addr = get_target(IF_ID.IR);
        rd = get_rd(IF_ID.IR);
        imm = get_immediate(IF_ID.IR);
        dpc4 = IF_ID.dpc4;
  
        CTRL_UNIT.op = op;
        CTRL_UNIT.funct = funct;
        CTRL_UNIT.rs = rs;
        CTRL_UNIT.rt = rt;

        uint32_t qa,qb;// get
        qa = cpu.gp.register_file[rs];
        qb = cpu.gp.register_file[rt];
        fwda = ForwardingUnit::calcuforwardA(CTRL_UNIT.rs);
        fwdb = ForwardingUnit::calcuforwardB(CTRL_UNIT.rt);
        setFWDA_MUX(fwda,qa,EX_ealu,MEM_malu,MEM_mmo);
        da = FWDA_MUX.o_ID_a;
        setFWDB_MUX(fwdb,qb,EX_ealu,MEM_malu,MEM_mmo);
        db = FWDB_MUX.o_ID_b;
        // x__err("db%x qb%x rt %x %x",db,qb,rt,IF_ID.IR);

        ID_rsrt_equ = ((da == db)?true:false);
        CTRL_UNIT.i_rsrtequ = ID_rsrt_equ;
        CTRL_CP0_UNIT.cop0_ins = IF_ID.IR;
        Control();
        wreg = CTRL_UNIT.o_wreg;
        m2reg = CTRL_UNIT.o_m2reg;
        wmem = CTRL_UNIT.o_wmem;
        jal = CTRL_UNIT.o_jal;
        aluc = CTRL_UNIT.o_aluc;
        aluimm = CTRL_UNIT.o_aluimm;
        shift = CTRL_UNIT.o_shift;
        regrt = CTRL_UNIT.o_regrt;
        sext = CTRL_UNIT.o_sext;
        fwda = CTRL_UNIT.o_fwda;
        fwdb = CTRL_UNIT.o_fwdb;
        setREGRT_MUX(regrt,rd,rt);
        drn = REGRT_MUX.o_drn;
        dsl_width_sel = CTRL_UNIT.o_sl_width_sel;

        ID_pcd = IF_ID.PCd;

        if(CTRL_UNIT.o_sext)
           dimm = sign_extend(imm);
        else 
           dimm = zero_extend(imm);
        bpc = calcu_bpc(dpc4,shift_left_2(dimm));
        jpc = calcu_jpc(dpc4,addr);
        
        ID_CP0_M::setSEPC_MUX(CTRL_CP0_UNIT.o_sepc,IF_pc,ID_pcd,EX_pce,MEM_pcm);
        cp0_epcin = ID_CP0_M::SEPC_MUX.o_epcin;

//         mux2x32 sta_mx (stalr,db,mtc0,sta_in);
// // mux for status reg
// mux2x32 cau_mx (cause,db,mtc0,cau_in);
//CTRL_CP0_UNIT.o_cause,db,CTRL_CP0_UNIT.o_mtc0,cau_in )
// // mux for cause reg
// mux2x32 epc_mx (epcin,db,mtc0,epc_in);
// // mux for epc reg
        cp0_operations(db);
        // if(CTRL_CP0_UNIT.o_mfc0!=0){
        //     x__err("mfc0 %x",CTRL_CP0_UNIT.o_mfc0);
        //     // R3000_CP0::cp0_regs.SR.raw =0xcafebabe;
        //     // R3000_CP0::dump_cp0_regs();
        // }
            

        ID_EX.pipeline_cp0_regs[12] = R3000_CP0::cp0_regs.SR.raw;// sta
        ID_EX.pipeline_cp0_regs[13] = R3000_CP0::cp0_regs.CAUSE.raw;// cau
        ID_EX.pipeline_cp0_regs[14] = R3000_CP0::cp0_regs.EPC;// epc
        if(CTRL_CP0_UNIT.o_mfc0)
            ID_EX.pipeline_cp0_regs[rd] = R3000_CP0::cp0_regs.val[rd];//节省时间，不全拷，只拷用到的

        // if(CTRL_CP0_UNIT.o_mtc0){
        //     R3000_CP0::dump_cp0_regs();
        //     x__err("iscache %x ",R3000_CP0::cp0_regs.SR.IsC);
        // }
            


        using namespace CrossPipelineWires;
        ID_bpc = bpc;
        ID_da = da;
        ID_jpc = jpc;
        ID_pcsrc = CTRL_UNIT.o_pcsrc;
        ID_wpcir = CTRL_UNIT.o_wpcir;
        
        ID_EX.IR = IF_ID.IR;
        ID_EX.ewreg = wreg;
        ID_EX.em2reg = m2reg;
        ID_EX.ewmem = wmem;
        ID_EX.ejal = jal;
        ID_EX.epc4 = IF_ID.dpc4;
        ID_EX.ealuc = aluc;
        ID_EX.ealuimm = aluimm;
        ID_EX.eshift = shift;
        ID_EX.ea = da;
        ID_EX.eb = db;
        ID_EX.eimm = dimm;
        ID_EX.emfc0 = CTRL_CP0_UNIT.o_mfc0;
        // printf("[%x %x %x %x]forwada %x %x %x %x ",da,db,rs,rt,fwda,qa,EX_ealu,MEM_malu,MEM_mmo);
        // x__err("ID stage rs%x rt%x %x ",rs,rt,db);
        ID_EX.ern0 = drn;
        ID_EX.esl_width_sel = dsl_width_sel;
        ID_EX.PCe = IF_ID.PCd;
        printf("ID %08x\t", ID_EX.IR);
        return;
    }
    void EX(pr::ID_EX_t &ID_EX, pr::EX_MEM_t &EX_MEM)
    {
        using namespace EXE_Signals;
        using namespace Multiplexer::EXEMUX;
        clear_ex_temp_signals();
        // EX/MEM.IR ← ID/EX.IR;
        // EX/MEM.ALUOutput ←
        // ID/EX.A func ID/EX.B;
        // or
        // EX/MEM.ALUOutput ←
        // ID/EX.A op ID/EX.Imm;
        epc8 = ID_EX.epc4 + 4;
        // ID_EX.ea;
        sa = DECODE::get_shamt(ID_EX.eimm);

        setESHIFT_MUX(ID_EX.eshift,sa,ID_EX.ea);
        eALUOp1 = ESHIFT_MUX.o_src1;
        setEALUIMM_MUX(ID_EX.ealuimm,ID_EX.eimm,ID_EX.eb);
        eALUOp2 = EALUIMM_MUX.o_src2;
        // x__err("EX src1 %x %x %x OP2 %x ", eALUOp1 = ESHIFT_MUX.o_src1,ID_EX.ea,ID_EX.ea,ID_EX.eimm,ID_EX.eb,eALUOp2);//ID_EX.eb
        eALUresult = ALU::ALUOperation(eALUOp1,eALUOp2,ID_EX.ealuc);

        //ID_EX.pipeline_cp0_regs[]
        // pc8c0r mux
        //ID_EX.emfc0;
        // ID_EX.pipeline_cp0_regs
        setEPC8_Cp0r_MUX(ID_EX.emfc0,epc8,
            ID_EX.pipeline_cp0_regs[12],// sta
            ID_EX.pipeline_cp0_regs[13],// cau
            ID_EX.pipeline_cp0_regs[14],// epc
            ID_EX.pipeline_cp0_regs);
        pc8c0r = EPC8_Cp0r_MUX.o_pc8c0r_in;
 

        setEJALmfc0_MUX( (ID_EX.ejal) or (ID_EX.emfc0),
            pc8c0r,eALUresult);
        ealu = EJAL_MUX.o_ealu;


        if(ID_EX.ejal)
            ern = ID_EX.ern0 | 0b11111;//jal $31
        else 
            ern = ID_EX.ern0;
        
        using namespace CrossPipelineWires;
        EX_ern = ern;
        EX_em2reg = ID_EX.em2reg;
        EX_ewreg = ID_EX.ewreg;
        EX_ealu = ealu;
        EX_pce = ID_EX.PCe;

        EX_MEM.IR = ID_EX.IR;
        EX_MEM.mwreg = ID_EX.ewreg;
        EX_MEM.mm2reg = ID_EX.em2reg;
        EX_MEM.mwmem = ID_EX.ewmem;
        EX_MEM.malu = ealu;
        EX_MEM.mb = ID_EX.eb;
        EX_MEM.mrn = ern;
        EX_MEM.msl_width_sel = ID_EX.esl_width_sel;
        EX_MEM.PCm = ID_EX.PCe;
        printf("EX %08x\t", EX_MEM.IR);
        return;
    }
    void MEM(pr::EX_MEM_t &EX_MEM, pr::MEM_WB_t &MEM_WB)
    {
        using namespace MEM_Signals;
        // using namespace PipelineStall;
        using namespace CrossPipelineWires;
        clear_MEM_temp_signals();
        MEM_mrn = EX_MEM.mrn;
        MEM_mm2reg = EX_MEM.mm2reg;
        MEM_mwreg = EX_MEM.mwreg;
        MEM_malu = EX_MEM.malu;
        MEM_pcm = EX_MEM.PCm;
        
        storeload_width_sel = EX_MEM.msl_width_sel;
        using namespace Multiplexer::MEMMUX;
        setMWIDTH_MUX(storeload_width_sel);
        storeload_width = MWIDTH_MUX.o_load_store_width;
        bool WriteMem = EX_MEM.mwmem;// if no stall
        // MemWrite
        if(WriteMem)
            memory.write_wrapper(MEM_malu,EX_MEM.mb,storeload_width);//memory.write<uint32_t>(MEM_malu,EX_MEM.mb);
        // MemRead
        if(EX_MEM.mm2reg){// 若MemtoReg为true,必然要MemRead
            mmo = memory.read_wrapper(MEM_malu,storeload_width);
            switch (EX_MEM.msl_width_sel)
            {
                case 0b00:// LW 32
                    mmo = mmo;
                    break;
                case 0b01:// LH 16
                    mmo = DECODE::sign_extend(mmo);
                    break;
                case 0b10:// LB 8
                    mmo = LB::sign_extend8to32(mmo);
                default:
                    break;
            }
        }
        MEM_mmo= mmo ;// for forwardA 
        MEM_WB.IR = EX_MEM.IR;
        MEM_WB.wwreg = EX_MEM.mwreg;
        MEM_WB.wm2reg = EX_MEM.mm2reg;
        MEM_WB.wrn = EX_MEM.mrn;
        MEM_WB.walu = EX_MEM.malu;
        MEM_WB.wmo = mmo;
    

    MEM_WB.debug_wbPC = EX_MEM.PCm;
        // MEM_WB.ALUOutput = EX_MEM.ALUOutput;
        // bool WriteMem;

        // WriteMem = WriteMem and (!Stall);

        printf("MEM %08x\t", MEM_WB.IR);
        return;
    }
    void WB(pr::MEM_WB_t &MEM_WB, R3000A_General_Purpose_Registers &gp)
    {
#define Regs gp.register_file
        using namespace WB_Signals;
        using namespace Multiplexer::WBMUX;
        clear_WB_temp_signals();
//         using namespace PipelineStall;
//         // Regs[MEM/WB.IR[rd]] ←
//         // MEM/WB.ALUOutput;
//         // or
//         // Regs[MEM/WB.IR[rt]] ←
//         // MEM/WB.ALUOutput;
//         bool WriteReg;

//         WriteReg = WriteReg and (!Stall);
        setWM2REG_MUX(MEM_WB.wm2reg,MEM_WB.wmo,MEM_WB.walu);
        wdi = WM2REG_MUX.o_wdi;
        if(MEM_WB.wwreg)
            Regs[MEM_WB.wrn] = wdi; 

        printf("WB %08x\t", MEM_WB.IR);
        return;
    }
    void tick()
    {
        using namespace pipeline_registers;
        using namespace CrossPipelineWires;
        CrossPipelineWires::clearCrossPipelineWires();
        // x__log("%x\t",Pre_IF.PC);
        x__log("%x\t",MEM_WB.debug_wbPC);
        WB(MEM_WB, cpu.gp);
        MEM(EX_MEM, MEM_WB);
        EX(ID_EX, EX_MEM);
        ID(IF_ID, ID_EX);
        IF(Pre_IF, IF_ID);
        Pre_IF.PC = IF_npc;
        


        printf("\n");
        // cpu.dump_regs();
    }
};

