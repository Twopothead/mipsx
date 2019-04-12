// #include "mipsx.h"
#include <inttypes.h>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include "cmipsx.h"
#include "monitor.h"
// http://emulation.gametechwiki.com/index.php/PS1_Tests
// PS1 Test:https://psx.amidog.se/doku.php?id=psx:download:misc
// https://www.ngemu.com/threads/pcsx-issues.143000/
int recode_cycle = 20;
// syscall 
// [2695642] 0xbfc0d964 0x0000000c
// [2766784] 0x8005aa94 0x0000000c
// lwl 24502246
// 0xbfc00000	0x88c10003	lwl  R01, [R06 + 0x3]
// 到15000000为止，pc和IR都不错
// 19246121出问题了
// 正确[19246538] 0x80052a30 0x00004012
// [19246541] 0x80052a30 0x00004012
// [19246537] 0x80052a2c 0x00c1001a

// [19246537] 0x80052a2c 0x00c1001a 0x00c1001a	div R06, R01
// PSX HI=0x0000005e LO=0x00005ebc
// GPR00: r0 00000000 at 0000000c v0 00000024 v1 0000003c
// GPR04: a0 00000003 a1 00000000 a2 00000024 a3 00000063
// GPR08: t0 1f801c00 t1 00000000 t2 0000dff1 t3 00000000
// GPR12: t4 00000200 t5 800699e0 t6 1f801c00 t7 800697e0
// GPR16: s0 00000000 s1 00000000 s2 00000002 s3 00000001
// GPR20: s4 00000000 s5 800dea68 s6 00000001 s7 00000000
// GPR24: t8 0000dff1 t9 1f801c00 k0 8005aa18 k1 00000f1c
// GPR28: gp a0010ff0 sp 801ffc28 s8 00000024 ra 80052fe0
// [19246538] 0x80052a30 0x00004012 0x00004012	mflo R08
// PSX HI=0000000000 LO=0x00000003
// GPR00: r0 00000000 at 0000000c v0 00000024 v1 0000003c
// GPR04: a0 00000003 a1 00000000 a2 00000024 a3 00000063
// GPR08: t0 1f801c00 t1 00000000 t2 0000dff1 t3 00000000
// GPR12: t4 00000200 t5 800699e0 t6 1f801c00 t7 800697e0
// GPR16: s0 00000000 s1 00000000 s2 00000002 s3 00000001
// GPR20: s4 00000000 s5 800dea68 s6 00000001 s7 00000000
// GPR24: t8 0000dff1 t9 1f801c00 k0 8005aa18 k1 00000f1c
// GPR28: gp a0010ff0 sp 801ffc28 s8 00000024 ra 80052fe0
// [19246539] 0x80052a34 0x30aeffff
// PSX HI=0000000000 LO=0x00000003
// GPR00: r0 00000000 at 0000000c v0 00000024 v1 0000003c
// GPR04: a0 00000003 a1 00000000 a2 00000024 a3 00000063
// GPR08: t0 00000003 t1 00000000 t2 0000dff1 t3 00000000
// GPR12: t4 00000200 t5 800699e0 t6 1f801c00 t7 800697e0
// GPR16: s0 00000000 s1 00000000 s2 00000002 s3 00000001
// GPR20: s4 00000000 s5 800dea68 s6 00000001 s7 00000000
// GPR24: t8 0000dff1 t9 1f801c00 k0 8005aa18 k1 00000f1c
// GPR28: gp a0010ff0 sp 801ffc28 s8 00000024 ra 80052fe0
int main()
{
    MIPSX_SYSTEM psx;
    Monitor monitor(psx); 
    //  const int total_cycle = 19246560;
    // const int total_cycle = 19246567;
    // const int total_cycle = 25000000;
    // const int total_cycle = 24502246;
    // 死在T=19246567
    // const int total_cycle = 19246540;
    const int total_cycle =19246562;
    for( mipsx_cycle=-3;mipsx_cycle<=total_cycle+1;mipsx_cycle++)
    {   
        psx.tick();
        
        // if(mipsx_cycle>=19246564){
        // if(mipsx_cycle>=19246550){
        // if(mipsx_cycle>=19246530){
            // if(mipsx_cycle>=19246119){
            if(mipsx_cycle>=19246530){
            Log::log = true;
             monitor.showStatus();
        }
        
           

        // if(mipsx_cycle>=2766783)
        //     monitor.showStatus();
    }
    
   
    return 0;
}

// PS-X Realtime Kernel Ver.2.5
// Copyright 1993,1994 (C) Sony Computer Entertainment Inc. 
// KERNEL SETUP!

// Configuration : EvCB	0x10		TCB	0x04
// System ROM Version 2.2 12/04/95 A
// Copyright 1993,1994,1995 (C) Sony Computer Entertainment Inc.
// ResetCallback: _96_remove ..
// VSync: timeout (1:0)
// VSync: timeout (1:0)
// VSync: timeout (1:0)
// VSync: timeout (1:0)
// VSync: timeout (1:0)
// VSync: timeout (1:0)
// VSync: timeout (1:0)
// VSync: timeout (1:0)