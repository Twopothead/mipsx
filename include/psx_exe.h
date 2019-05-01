
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
// http://www.emulatronia.com/doctec/consolas/psx/exeheader.txt
#define SHELL_MAIN 0x80030000
namespace PSX_EXE{
    typedef struct _EXE_HEADER_ {
        uint8_t id[8];
        uint32_t text_offset;			/* SCE only */
        uint32_t data_offset;			/* SCE only */
        uint32_t pc0;
        uint32_t gp0;			/* SCE only */
        uint32_t text_addr;
        uint32_t text_size;
        uint32_t data_addr;			/* SCE only */
        uint32_t data_size;			/* SCE only */
        uint32_t bss_addr;			/* SCE only */
        uint32_t bss_size;			/* SCE only */
        uint32_t stack_addr;
        uint32_t stack_size;
        uint32_t SavedSP;
        uint32_t SavedFP;
        uint32_t SavedGP;
        uint32_t SavedRA;
        uint32_t SavedS0;
    } SCE_HEADER;

    typedef struct {
        uint8_t  id[8];
        uint32_t textOffset;
        uint32_t pc;
        uint32_t textAddr;
        uint32_t textSize;
        uint32_t stackAddr;
        uint32_t stackSize;
        uint32_t savedSP;
        uint32_t savedFP;
        uint32_t savedGP;
        uint32_t savedRA;
        uint32_t savedS0;
    } PSXheader_t;

    typedef struct {
        uint8_t id[8];
        uint8_t pad1[8];
        uint32_t pc;
        uint32_t gp;
        uint32_t dst;
        uint32_t size;
        uint32_t pad2[2];
        uint32_t memfillAddr;
        uint32_t memfillSize;
        uint32_t sp;
        uint32_t spOffset;
        uint8_t pad3[20];
        uint8_t SCEI[1972];
        uint8_t data[];
    } EXEheader_t;

    void * load(const char * filename)
    {
        FILE *f = fopen(filename,"rb");
        fseek(f,0,SEEK_END);
        size_t fsize = ftell(f);
        rewind(f);
        void * data = malloc(fsize);
        fread(data,fsize,1, f);
        return data;
    }


}



// •ExeType = { 'SCE EXE' || 'PS-X EXE' }; 
// •text - Offset of the text segment 
// •data - Offset of the data segment 
// •pc0 - Program Counter. 
// •gp0 - Address of the Global Pointer 
// •t_addr - The address where the text segment is loaded 
// •t_size - The size of the text segment 
// •d_addr - The address where the text segment is loaded 
// •d_size - The size of the data segment 
// •b_addr - The address of the BSS segment 
// •b_size - The size of the BSS segment 
// •s_addr - The address of the stack 
// •s_size - The size of the stack. 
// •SavedXX -The Exec system call saves some registers to these fields before jumping to the program 

// PS1:
// - https://github.com/simias/psx-guide (Guide to get you started. Rendered .pdf here: https://goo.gl/z5zQ2W)
// - https://problemkaputt.de/psx-spx.htm (Should be your main source of info)
// - http://wiki.psxdev.ru/index.php/BIOS (translated: https://goo.gl/cDyVGq)
// - http://emulation.gametechwiki.com/index.php/PS1_Tests (see link below on how to load them)
// - http://www.emulatronia.com/doctec/consolas/psx/exeheader.txt (also see "CDROM File Formats" section in nocash's docs)
// - https://github.com/ogamespec/psxdev/tree/master/reverse
// - http://hitmen.c02.at/html/psx_docs.html
// - http://www.raphnet.net/electronique/psx_adaptor/Playstation.txt
// - https://github.com/ogamespec/pops-gte
// - https://github.com/simias/psx-hardware-tests/tree/master/tests
// PS2:
// - http://hwdocs.webs.com/ps2
// - https://github.com/ZirconiumX/ps2-clang-patches/blob/master/erratum/unofficial-erratum.md
// - http://psx-scene.com/forums/f291/gs-mode-selector-development-feedback-61808/#post457673
// - https://assemblergames.com/threads/the-playstation-2-busses-dev9.67961/
// Sega Master System:
// - http://www.smspower.org/Development/MemoryMap
// - http://www.smspower.org/Development/Z80-Index (CPU registers, instruction set, etc)
// - Cpu Test Program (doesn't require any hardware, it can log to an IO port so you can bootstrap that very easily!) http://www.smspower.org/Homebrew/ZEXALL-SMS
// - Video Hardware: http://www.smspower.org/Development/VDPRegisters
// - Sound Hardware: http://www.smspower.org/Development/SN76489?from=Development.PSG
// - Lots of simple homebrew test cases: http://www.smspower.org/Homebrew/NotOnlyWords-SMS 
