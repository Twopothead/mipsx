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