#pragma once 
#include "cmipsx.h"
class Monitor{
    MIPSX_SYSTEM *sys;
    public:
    Monitor(MIPSX_SYSTEM &p){ sys = &p;};
    void showStatus(){
        sys->cpu.dump_regs();
    }
};
// http://emulation.gametechwiki.com/index.php/PS1_Tests
// PS1 Test:https://psx.amidog.se/doku.php?id=psx:download:misc
// https://www.ngemu.com/threads/pcsx-issues.143000/