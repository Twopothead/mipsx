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
        pipeline_registers::Pre_IF.PC = start_Rom;
    };
    friend class Monitor;
    void IF(pr::Pre_IF_t &Pre_IF, pr::IF_ID_t &IF_ID)
    {
// IF/ID.IR ← Mem[PC];
// IF/ID.NPC,PC ← (if ((EX/MEM.opcode == branch) & EX/MEM.cond){EX/MEM.
// ALUOutput} else {PC+4});
        using namespace IF_Signals;
        using namespace CrossPipelineWires;
        using namespace Multiplexer::IFMUX;
        clear_temp_IF_signals();
        IF_ID.IR = memory.read<uint32_t>(Pre_IF.PC);
        pc4 = Pre_IF.PC + 4;
        IF_ID.dpc4 = pc4;
        setPCSRC_MUX(ID_pcsrc,pc4,ID_bpc,ID_da,ID_jpc);
        npc = PCSRC_MUX.o_npc;
        IF_npc = npc;
        printf("IF %08x\t", IF_ID.IR);
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
        bpc = calcu_bpc(dpc4,shift_left_2(imm));
        jpc = calcu_jpc(dpc4,addr);
        CTRL_UNIT.op = op;
        CTRL_UNIT.funct = funct;
        CTRL_UNIT.rs = rs;
        CTRL_UNIT.rt = rt;
        ID_rsrt_equ = (da == db);
        CTRL_UNIT.i_rsrtequ = ID_rsrt_equ;
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
        uint32_t qa,qb;// get
        qa = cpu.gp.register_file[rs];
        qb = cpu.gp.register_file[rt];
        setFWDA_MUX(fwda,qa,EX_ealu,MEM_malu,MEM_mmo);
        da = FWDA_MUX.o_ID_a;
        setFWDB_MUX(fwdb,qb,EX_ealu,MEM_malu,MEM_mmo);
        db = FWDB_MUX.o_ID_b;
        // ID/EX.A ← Regs[IF/ID.IR[rs]]; ID/EX.B ← Regs[IF/ID.IR[rt]];
        // ID/EX.NPC ← IF/ID.NPC; ID/EX.IR ← IF/ID.IR;
        // ID/EX.Imm ← sign-extend(IF/ID.IR[immediate field]);
        //     uint32_t jPC;

        // uint32_t da = Regs[rs];
        // uint32_t db = Regs[rt];
        // bool rsrt_equ = (da == db);
        if(CTRL_UNIT.o_sext)
           dimm = sign_extend(imm);
        else 
           dimm = zero_extend(imm);


        // mux
        // ID_EX.A = da;
        // ID_EX.B = db;
        // ID_EX.ePCplus4 = IF_ID.dPCplus4;
        using namespace CrossPipelineWires;
        ID_bpc = bpc;
        ID_da;
        ID_jpc = jpc;
        ID_pcsrc = CTRL_UNIT.o_pcsrc;
        ID_wpcir = CTRL_UNIT.o_wpcir;
        
        ID_EX.IR = IF_ID.IR;
        ID_EX.ewreg = wreg;
        ID_EX.em2reg = m2reg;
        ID_EX.ewmem = wmem;
        ID_EX.ejal = jal;
        ID_EX.ealuc = aluc;
        ID_EX.ealuimm = aluimm;
        ID_EX.eshift = shift;
        ID_EX.ea = da;
        ID_EX.eb = db;
        ID_EX.eimm = dimm;
        ID_EX.ern0 = drn;
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
        eALUresult = ALU::ALUOperation(eALUOp1,eALUOp2,ID_EX.ealuc);
        setEJAL_MUX(ID_EX.ejal,epc8,eALUresult);
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

        EX_MEM.IR = ID_EX.IR;
        EX_MEM.mwreg = ID_EX.ewreg;
        EX_MEM.mm2reg = ID_EX.em2reg;
        EX_MEM.mwmem = ID_EX.ewmem;
        EX_MEM.malu = ealu;
        EX_MEM.mb = ID_EX.eb;
        EX_MEM.mrn = ern;
        printf("EX %08x\t", EX_MEM.IR);
        return;
    }
    void MEM(pr::EX_MEM_t &EX_MEM, pr::MEM_WB_t &MEM_WB)
    {
        using namespace MEM_Signals;
        

        // using namespace PipelineStall;
        // MEM/WB.IR ← EX/MEM.IR;
        // MEM/WB.ALUOutput ←
        // EX/MEM.ALUOutput;
        // if(EX_MEM.mwmem)
        //     mmo = memory.read<uint32_t>();
        using namespace CrossPipelineWires;
        clear_MEM_temp_signals();
        MEM_mrn = EX_MEM.mrn;
        MEM_mm2reg = EX_MEM.mm2reg;
        MEM_mwreg = EX_MEM.mwreg;
        MEM_malu = EX_MEM.malu;


        MEM_mmo = mmo;// mmo
        
        // if(EX_MEM.mwmem)
        //     x__err("mmo:%x %x",mmo,MEM_mm2reg);
        bool WriteMem = EX_MEM.mwmem;// if no stall
        if(WriteMem)
            memory.write<uint32_t>(MEM_malu,EX_MEM.mb);


        MEM_WB.IR = EX_MEM.IR;
        MEM_WB.wwreg = EX_MEM.mwreg;
        MEM_WB.wm2reg = EX_MEM.mm2reg;
        MEM_WB.wrn = EX_MEM.mrn;
        MEM_WB.walu = EX_MEM.malu;
        MEM_WB.wmo;
    
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
        x__log("%x\t",Pre_IF.PC);
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
class Monitor{
    MIPSX_SYSTEM *sys;
    public:
    Monitor(MIPSX_SYSTEM &p){ sys = &p;};
    void showStatus(){
        sys->cpu.dump_regs();
    }
};
