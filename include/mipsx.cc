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
    MIPSX_SYSTEM psx;
    Monitor monitor(psx);
    for (int i = 0; i < 110; i++)
    {
        printf("[%06d]\t",i-3);
        psx.tick();
        if(i>=4){
            monitor.showStatus();
        }
    }
    return 0;
}
