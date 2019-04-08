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
    // 17389处用到17374时sw写入的数据
    // 17766 0x40026000 mfc0 $2 , $cop012 [017763] 0xbfc03968 0x40026000 v0由 0xa000b068 变为 0x00000000
    // 121244 [121244] 0xbfc01ad4 0x1ca00003 bgtz $5 , +12
    // 17772
    // 50000 OK
    // 从80890 开始出错
    // bgtz要在ID阶段就算出来
    // 死在79218 0xacc7fffc sw，要往0xb0写入 data:3c080000
    // 079473时再写入0,其实这地方不应该写0 [079473] 0xbfc01968 WB ad400020 0xad400020 [  ERR ]  virtual_addrb0 data:0
    // ad400030
    // 有可能因为没用Cache,理论上是根本没写入memory
    // 可能是memorymap的原因 cop12 status有问题
    // the $12 register contains 0x00010000 so this MTC0 instruction
     // sets bit 16 of SR which is the “isolate cache” bit. It makes all the following read
    // and write target directly the cache instead of going through it towards the main
     //memory.
    MIPSX_SYSTEM psx;
    Monitor monitor(psx);
    int i=0;
    // T = 150000, OK tested
    for(int i=-3;i<=150000;i++)
    {
        if(i>0)
            printf("[%06d] ",i);
        psx.tick();
        if(i>0)
            monitor.showStatus();
    }
    
    // R3000_CP0::dump_cp0_regs();
    return 0;
}
