#pragma once
// #include <allegro5/allegro.h>
// #include <allegro5/system.h>
// #include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// #include <GLES3/gl3.h>
// sudo apt install libglew-dev
// http://lazyfoo.net/tutorials/SDL/01_hello_SDL/linux/cli/index.php
// https://liballeg.org/a5docs/5.0.11/refman.pdf
// https://moddb.fandom.com/wiki/SDL:Tutorials:Using_SDL_with_OpenGL

namespace PSX_UI{
    SDL_Window * window = NULL;
    SDL_Surface * surface = NULL;
    SDL_GLContext glContext;
    SDL_Event sdlEvent;
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    void Init_GL(){
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_Init(SDL_INIT_EVERYTHING);
        window = SDL_CreateWindow("mipsx", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, \
                    SCREEN_WIDTH,SCREEN_HEIGHT,\
                            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
        glContext = SDL_GL_CreateContext(window);
        SDL_GL_SetSwapInterval(1);
        glewInit();
    }

    int psx_ui_init(){
        Init_GL();
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();/*原点重置正中心*/
        glTranslatef(-1,-1,0);/*把坐标原点调整到左下角*/
        glScalef(2,2,0); /*放大两倍*/
// 平移：glTranslatef(x,y,z) 
// (x,y,z)为平移向量 
// 在z轴上平移有缩放的味道 
// 缩放：glScalef(x,y,z) 
// (x,y,z)为各方向上放大倍数
    }
    int psx_ui_quit(){
        SDL_DestroyWindow(window);
        window = NULL;
        SDL_Quit();
    }
}
// https://packages.ubuntu.com/search?searchon=contents&keywords=GL/glew.h&mode=exactfilename&suite=trusty&arch=any
// /usr/include/GL/glew.h	libglew-dev
// sudo apt install libglew-dev