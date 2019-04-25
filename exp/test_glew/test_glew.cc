#include <GL/glew.h>
 #include <SDL2/SDL.h>
 #include <SDL2/SDL_opengl.h>
 // https://www.cnblogs.com/CodeMIRACLE/p/5516841.html
 bool quit;
 SDL_Window* window;
 SDL_GLContext glContext;
 SDL_Event sdlEvent;
//  #define COLOR24(n)      glColor3f(CMD_R(n)/255.0, CMD_G(n)/255.0, CMD_B(n)/255.0);

 void Draw()
  {
          glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
          glLoadIdentity();
        //   glScaled(1.0f/10,1.0f/10,1.0f/10);
        //   glTranslatef(-3.0f,0.0f,6.0f);


	// glBegin(GL_QUADS);
	// glShadeModel(GL_FLAT);
	// COLOR24(0);
	// VERTEX(1);
	// VERTEX(2);
	// VERTEX(3);
	// glEnd();
	// CLEAR();



        //   glBegin(GL_TRIANGLES);
        //       glColor3f(1.0f,0.0f,0.0f);
        //       glVertex3f(0.0f,1.0f,0.0f);
        //       glColor3f(0.0f,1.0f,0.0f);
        //       glVertex3f(-1.0f,-1.0f,0.0f);
        //       glColor3f(0.0f,0.0f,1.0f);
        //       glVertex3f(1.0f,-1.0f,0.0f);
        //   glEnd();
        //   glColor3f(1.0f,1.0f,0.0f);
        //   glTranslatef(3.0f,0.0f,0.0f);
        //   glBegin(GL_QUADS);
        //       glVertex3f(-1.0f,1.0f,0.0f);
        //       glVertex3f(1.0f,1.0f,0.0f);
        //       glVertex3f(1.0f,-1.0f,0.0f);
        //       glVertex3f(-1.0f,-1.0f,0.0f);
        //   glEnd();
        //   glColor3f(0.0f,1.0f,1.0f);
        //   glTranslatef(3.0f,0.0f,0.0f);
        //   glBegin(GL_POLYGON);
        //       glVertex3f(0.0f, 3.0f,0.0f);
        //       glVertex3f(1.0f, 5.0f,0.0f);
        //       glVertex3f(2.0f, 2.0f,0.0f);
        //       glVertex3f(3.0f, 4.0f,0.0f);
        //       glVertex3f(4.0f, 1.0f,0.0f);
        //       glVertex3f(5.0f, 7.0f,0.0f);
        //   glEnd();
          SDL_GL_SwapWindow(window);
  }
 void Init()
 {
     SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
     SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
     SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);
     SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
     SDL_Init(SDL_INIT_EVERYTHING);
     window = SDL_CreateWindow("SDL+OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,400,300,SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
     glContext = SDL_GL_CreateContext(window);
     SDL_GL_SetSwapInterval(1);
     glewInit();
 }
//  void Draw()
//  {
//          glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//          glLoadIdentity();
//          glScaled(1.0f/10,1.0f/10,1.0f/10); //坐标系默认是[-1,1],这里先把坐标系拉伸一下,也可以用glViewport重置窗口大小
//          //绘制三角形
//          glTranslatef(-3.0f,0.0f,6.0f);
//          glBegin(GL_TRIANGLES);
//              glVertex3f(0.0f,1.0f,0.0f);
//              glVertex3f(-1.0f,-1.0f,0.0f);
//              glVertex3f(1.0f,-1.0f,0.0f);
//          glEnd();
//          //绘制矩形
//          glTranslatef(3.0f,0.0f,0.0f);
//          glBegin(GL_QUADS);
//              glVertex3f(-1.0f,1.0f,0.0f);
//              glVertex3f(1.0f,1.0f,0.0f);
//              glVertex3f(1.0f,-1.0f,0.0f);
//              glVertex3f(-1.0f,-1.0f,0.0f);
//          glEnd();
//          //绘制难看的多边形
//          glTranslatef(3.0f,0.0f,0.0f);
//          glBegin(GL_POLYGON);
//              glVertex3f(0.0f, 3.0f,0.0f);
//              glVertex3f(1.0f, 5.0f,0.0f);
//              glVertex3f(2.0f, 2.0f,0.0f);
//              glVertex3f(3.0f, 4.0f,0.0f);
//              glVertex3f(4.0f, 1.0f,0.0f);
//              glVertex3f(5.0f, 7.0f,0.0f);
//          glEnd();
//          //渲染
//          SDL_GL_SwapWindow(window);
//  }
 int main(int argc, char *argv[])
 {
     quit = false;
     Init();
     while (!quit)
     {
         while(SDL_PollEvent(&sdlEvent))
         {
             if(sdlEvent.type == SDL_QUIT)
             {
                 quit = true;
             }
         }
         Draw();
         SDL_Delay(16);
     }
     SDL_DestroyWindow(window);
     window = NULL;
     SDL_Quit();
     return 0;
 }