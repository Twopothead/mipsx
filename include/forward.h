#pragma once
#include <inttypes.h>
#include "pipeline_regs.h"
#include "wires.h"
namespace ForwardingUnit{
    using namespace pipeline_registers;
    uint32_t calcuforwardA(uint32_t rs){
       uint32_t forwardA = 0b00;// no hazards
       using namespace CrossPipelineWires;
       if( EX_ewreg
                and ( EX_ern != 0 )
                and ( EX_ern == rs )
                and ( ! EX_em2reg )     
            )
            forwardA = 0b01;// select exe_alu
       else{
            if(MEM_mwreg
                    and ( MEM_mrn != 0 )
                    and ( MEM_mrn == rs)
                    and ( ! MEM_mm2reg )
                )
                forwardA = 0b10;// select mem_alu
            else if(MEM_mm2reg
                    and ( MEM_mrn != 0)
                    and ( MEM_mrn == rs)
                    and ( MEM_mm2reg )                
                )
                forwardA = 0b11;// select mem_lw

       } 
      return forwardA;
 }
    //    if(ID_EX.eRegWrite
    //         and (ID_EX.eRegisterRn !=0 )
    //         and (ID_EX.eRegisterRn == rs)
    //         and ( !ID_EX.eMemtoReg )
    //         )
    //         forwardA = 0b01;// select exe_alu
    //     else {
    //         if(EX_MEM.mRegWrite
    //             and (EX_MEM.mRegisterRn != 0)
    //             and (EX_MEM.mRegisterRn == rs)
    //             and ( !EX_MEM.mMemtoReg )
    //         )
    //         forwardA = 0b10;// select mem_alu
    //         else if(EX_MEM.mRegWrite
    //             and(EX_MEM.mRegisterRn != 0)
    //             and(EX_MEM.mRegisterRn == rs)
    //             and ( EX_MEM.mMemtoReg )
    //         )
    //         forwardA = 0b11;// select mem_lw
        // }
        
   
    uint32_t calcuforwardB(uint32_t rt){
        uint32_t forwardB = 0b00;// no hazards
        using namespace CrossPipelineWires;
        if( EX_ewreg
                and ( EX_ern != 0 )
                and ( EX_ern == rt )
                and ( ! EX_em2reg )     
            )
            forwardB = 0b01;// select exe_alu
        else{
                if(MEM_mwreg
                        and ( MEM_mrn != 0 )
                        and ( MEM_mrn == rt)
                        and ( ! MEM_mm2reg )
                    )
                    forwardB = 0b10;// select mem_alu
                else if(MEM_mm2reg
                        and ( MEM_mrn != 0)
                        and ( MEM_mrn == rt)
                        and ( MEM_mm2reg )                
                    )
                    forwardB = 0b11;// select mem_lw
        } 
        return forwardB;
    }
}
        // if(ID_EX.eRegWrite
        //     and (ID_EX.eRegisterRn !=0 )
        //     and (ID_EX.eRegisterRn == rt)
        //     and ( !ID_EX.eMemtoReg )
        //     )
        //     forwardB = 0b01;// select exe_alu
        // else {
        //     if(EX_MEM.mRegWrite
        //         and (EX_MEM.mRegisterRn != 0)
        //         and (EX_MEM.mRegisterRn == rt)
        //         and ( !EX_MEM.mMemtoReg )
        //     )
        //     forwardB = 0b10;// select mem_alu
        //     else if(EX_MEM.mRegWrite
        //         and(EX_MEM.mRegisterRn != 0)
        //         and(EX_MEM.mRegisterRn == rt)
        //         and ( EX_MEM.mMemtoReg )
        //     )
        //     forwardB = 0b11;// select mem_lw
        // }

