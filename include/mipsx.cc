// #include "mipsx.h"
#include <inttypes.h>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include "cmipsx.h"
#include "monitor.h"
// namespace PipelineStall{
//     using namespace pipeline_registers;
//     bool Stall = false;
//     bool calcuStall(bool use_rs_field,bool use_rt_field,uint32_t rs,uint32_t rt){
//         bool stall = false;
//         // bool i_rs = use_rs_field;// i_rs and i_rt indicate that an instruction
//         // bool i_rt = use_rt_field;// uses the contents of the rs register and the rt register,
//         // stall = (  ID_EX.eRegWrite
//         //         and (ID_EX.eMemtoReg)
//         //         and (ID_EX.eRegisterRn!=0)
//         //         and (      ( (i_rs) and (ID_EX.eRegisterRn==rs) )
//         //                or  ( (i_rt) and (ID_EX.eRegisterRn==rt) )
//         //              )
//         //         );
//         return stall;
//     }

// }

int main()
{
    // SDL_Surface *surface;
    // SDL_LockSurface(surface);
    // SDL_UnlockSurface(surface);
    // 1515 0x8d090000
    // 1548 0xbfc003ac	0x24429000	addiu  R02, R02, 0x9000 write error: virtualaddr:ffff9000,normalizedAddress:1fff9000,data:0
    //      signextend(0x9000)=0xffff9000  hex(0xa0010000+0xffff9000)='0x1a0009000' 产生一个overflow exception 0xbfc003ac	0x24429000	addiu  R02, R02, 0x9000
    // 1555 $1 at=0xfffe0000
    // 17352 从T=1555到T=17354,一直在循环,这里终于到达0xbfc003cc
    // 17367 SPU addr: 0x1f801d80 instruction:0xa5200180
    // [017372] WB 0ff00698
    MIPSX_SYSTEM psx;
    Monitor monitor(psx);
    for (int i = 0; i <=17390; i++)
    {//17389
    //17374+3
        printf("[%06d]\t",i-3);
        psx.tick();
        // if(i>=4){
        //     monitor.showStatus();
        // }
    }
    monitor.showStatus();
    // R3000_CP0::dump_cp0_regs();
    return 0;
}
