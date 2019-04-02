#pragma once
#include <inttypes.h>
namespace IO_Map{
    namespace Expansion_Region{
        
    }
    namespace Scratchpad{

    }
    namespace Memory_Control_1{
        uint32_t Expansion_1_Base_Address = 0;
        uint32_t Expansion_2_Base_Address = 0;
        uint32_t Expansion_1_Delay = 0;
        uint32_t Expansion_3_Delay = 0;
        uint32_t BIOS_ROM_Delay = 0;
        uint32_t SPU_DELAY = 0;
        uint32_t CDROM_DELAY = 0;
        uint32_t Expansion_2_Delay = 0;
        uint32_t COM_DELAY = 0;
        uint32_t read(uint32_t addr){
            uint32_t data = 0;
            switch (addr)
            {
                case 0x1f801000:
                    data = Expansion_1_Base_Address;
                    break;
                case 0x1f801004:
                    data = Expansion_2_Base_Address;
                    break;
                case 0x1f801008:
                    data = Expansion_1_Delay;
                    break;     
                case 0x1f80100c:
                    data = Expansion_3_Delay;
                    break;     
                case 0x1f801010:
                    data = BIOS_ROM_Delay;
                    break; 
                case 0x1f801014:
                    data = SPU_DELAY;
                    break;
                case 0x1f801018:
                    data = CDROM_DELAY;
                    break;
                case 0x1f80101c:
                    data = Expansion_2_Delay;
                    break;
                case 0x1f801020:
                    data = COM_DELAY;
                    break;
                default:
                    break;
            }
        }
        uint32_t write(uint32_t addr,uint32_t data){ 
            switch (addr)
            {
                case 0x1f801000:
                    Expansion_1_Base_Address = data;
                    break;
                case 0x1f801004:
                    Expansion_2_Base_Address = data;
                    break;
                case 0x1f801008:
                    Expansion_1_Delay = data;
                    break;     
                case 0x1f80100c:
                    Expansion_3_Delay = data;
                    break;     
                case 0x1f801010:
                    BIOS_ROM_Delay = data;
                    break; 
                case 0x1f801014:
                    SPU_DELAY = data;
                    break;
                case 0x1f801018:
                    CDROM_DELAY = data;
                    break;
                case 0x1f80101c:
                    Expansion_2_Delay = data;
                    break;
                case 0x1f8010020:
                    COM_DELAY = data;
                    break;
                default:
                    break;
            }
        }
        
    }
    namespace Peripheral_IO_Ports{

    }
    namespace Memory_Control_2{
        uint32_t RAM_SIZE = 0;
        uint32_t read(uint32_t addr,uint32_t width){
            uint32_t data = 0;
            if(addr==0x1f801060)
                data = RAM_SIZE;
            return data;
        }
        void write(uint32_t addr,uint32_t data,uint32_t width){
            if(addr==0x1f801060)
                RAM_SIZE = data;
            return;
        }
    }
    namespace Interrupt_Control{
        uint32_t I_STAT = 0;
        uint32_t I_MASK = 0;
        uint32_t read(uint32_t addr){
            uint32_t data = 0;
            switch (addr)
            {
                case 0x1f801070:
                    data = I_STAT;
                    break;
                case 0x1f801074:
                    data = I_MASK;
                    break;
                default:
                    break;
            }
            return data;
        }
        void write(uint32_t addr,uint32_t data){
            switch (addr)
            {
                case 0x1f801070:
                    I_STAT = data;
                    break;
                case 0x1f801074:
                    I_MASK = data;
                    break;
                default:
                    break;
            }
            return;
        }
    }
    // namespace DMA_Registers{

    // }
    // namespace Timers_Root_counters{
    //     uint32_t read(uint32_t addr){
    //         uint32_t data = 0;

    //         return data;
    //     }
    //     uint32_t write(uint32_t addr,uint32_t data){
            
    //         return;
    //     }
    // }
    // namespace CDROM_Registers{
    //     uint32_t read(uint32_t addr){
    //         uint32_t data = 0;


    //         return data;
    //     }
    //     void write(uint32_t addr,uint32_t data){
            
    //         return;
    //     }
    // }
    // namespace GPU_Registers{
    //     // uint32_t GP0 = 0;
    //     // uint32_t GP1 = 0;
    //     // uint32_t GPUREAD = 0;
    //     // uint32_t GPUSTAT =0;
    //     // uint32_t read(uint32_t addr){
    //     //     uint32_t data = 0;
    //     //     switch (addr)
    //     //     {
    //     //         case 0x1f801810:
    //     //             GP0;
    //     //             break;
    //     //         case 0x1f801814:
    //     //             GP1;
    //     //             break;
    //     //         case 0x1f801810;
            
    //     //         default:
    //     //             break;
    //     //     }

    //     //     return data;
    //     // }
    //     // void write(uint32_t addr,uint32_t data){
            
    //     //     return;
    //     // }
    // }
    // namespace MDEC_Registers{
    //     uint32_t read(uint32_t addr){
    //         uint32_t data = 0;

    //         return data;
    //     }
    //     void write(uint32_t addr,uint32_t data){
            
    //         return;
    //     }
    // }
    // namespace SPU_Voice_0to23_Registers{

    // }
    // namespace SPU_Control_Registers{

    // }
    // namespace SPU_Reverb_Configuration_Area{

    // }
    // namespace SPU_Internal_Registers{

    // }
    // namespace Expansion_Region_2{

    // }
    // namespace Expansion_Region_2_DUART{

    // }
    // namespace Expansion_Region_2_Int_Dip_Post{

    // }
    // namespace Expansion_Region_3{

    // }
    // namespace Memory_Control_3{

    // } 


    uint32_t io_read(uint32_t addr,int width){
        width = 32;
        uint32_t data = 0;
        uint32_t vaddr = addr + 0x1f800000;  
        switch (vaddr)
        {
            case 0x1f801000 ... 0x1f801020:
                Memory_Control_1::read(addr);
                break;
            case 0x1f801060:
                Memory_Control_2::read(addr,32);
                break;
            default:
                x__err("io read error:%x ",addr);
                break;
        }
        return data;
    }
    void io_write(uint32_t addr,uint32_t data,int width){
        width = 32;
        uint32_t vaddr = addr + 0x1f800000;  
        switch (vaddr)
        {
            case 0x1f801000 ... 0x1f801020:
                Memory_Control_1::write(vaddr,data);
                break;
            case 0x1f801060:
                Memory_Control_2::write(vaddr,data,32);
                break;
            default:
                x__err("io write error: addr:%x data:%x",vaddr,data);
                break;
        }   
    }
}
