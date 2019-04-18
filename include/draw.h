#pragma once
#include <inttypes.h>
namespace Graphics{
    typedef struct{
        float x,y;
        uint8_t r,g,b,a;
    }vertex;
    typedef struct{
        float x,y,u,v;
        uint8_t r,g,b,a;
    }texturedVertex;

    
    
}