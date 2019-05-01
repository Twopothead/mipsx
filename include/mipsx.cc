// #include "mipsx.h"
#include <inttypes.h>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include "cmipsx.h"
#include "monitor.h"
#include "debug.h"
#include "ui.h"



int main( int argc, char* args[] )
{
    MIPSX_SYSTEM psx;
    Monitor monitor(psx); 
    // const int total_cycle =84502258;
    const int total_cycle =104502258;
    PSX_UI::psx_ui_init();
    for( mipsx_cycle=-2;mipsx_cycle<=total_cycle+1;mipsx_cycle++)
    {   
        psx.tick();
        psx.hack_intercept_BIOS_Putchar();
        if(argc>=2 && pipeline_registers::MEM_WB.debug_wbPC==SHELL_MAIN){
            psx.loadEXE(args[1]);
            // "../demos/psxtest_cpu/psxtest_cpu.exe"
        }
        // if(Log::log)
        //     monitor.showStatus();
    }
    
    PSX_UI::psx_ui_quit();

   
    return 0;
}