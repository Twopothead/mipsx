#pragma once
#include <stdio.h>
#include "color.h"


#define DEBUG

#ifdef DEBUG
    #define D(s,...) printf(s,##__VA_ARGS__)

    #define xdebug(format, ... ) \
        do{\
            fprintf(stdout,TERMINAL_FONT_RED  "[%s,%d,%s]" format "\33[0m",\
            __FILE__, __LINE__, __func__, ## __VA_ARGS__ \
            ); \
        }while(0);

    #define xerror(format, ... ) \
        do{\
            fprintf(stdout,TERMINAL_FONT_RED  "[%s,%d,%s]" format "\33[0m",\
            __FILE__, __LINE__, __func__, ## __VA_ARGS__ \
            ); \
        }while(0);

    #define xpanic(format, ... ) \
        do{\
            fprintf(stdout,TERMINAL_FONT_YELLOW  "[%s,%d,%s]" format "\33[0m",\
            __FILE__, __LINE__, __func__, ## __VA_ARGS__ \
            ); \
        }while(0);

    #define xwarn(format, ... ) \
        do{\
            fprintf(stdout,TERMINAL_FONT_YELLOW  "[%s,%d,%s]" format "\33[0m",\
            __FILE__, __LINE__, __func__, ## __VA_ARGS__ \
            ); \
        }while(0);

    #define xlog(format, ... ) \
        do{\
            fprintf(stdout,TERMINAL_FONT_GREEN  "[%s,%d,%s]" format "\33[0m",\
            __FILE__, __LINE__, __func__, ## __VA_ARGS__ \
            ); \
        }while(0);
    
    #define TODO()  \
        do{\
            xwarn("please implement me!\n");\
        }while(0);

    #define xinfo(format, ... ) \
        do{\
            fprintf(stdout,TERMINAL_FONT_WHITE  format "\33[0m",\
             ## __VA_ARGS__ \
            ); \
        }while(0);  

    #define x_ok(format, ... ) \
        do{\
            fprintf(stdout,TERMINAL_FONT_GREEN  "[%s,%d,%s]" format "\33[0m",\
            __FILE__, __LINE__, __func__, ## __VA_ARGS__ \
            ); \
        }while(0);

    #define ximplemented()  \
        do{\
            x_ok("implemented!\n");\
        }while(0); 
    
    #define COLOR_OK \
        do{\
            printf("\033[1;40m\033[1;37m [ \033[1;32m OK \033[1;37m ] \033[0m");\
        }while(0);  

    #define x_log(format, ... )  \
        do{\
            COLOR_OK;\
            printf(" ");\
            printf(format,##__VA_ARGS__);\
            printf("\n");\
        }while(0);

    #define COLOR_LOG \
        do{\
            printf("\033[1;40m\033[1;37m [ \033[1;32m LOG\033[1;37m ] \033[0m");\
        }while(0);

    #define x__log(format, ... )  \
        do{\
            COLOR_LOG;\
            printf(" ");\
            printf(format,##__VA_ARGS__);\
            printf("\n");\
        }while(0);

    #define COLOR_ERR \
        do{\
            printf("\033[1;40m\033[1;37m [ \033[1;31m ERR\033[1;37m ] \033[0m");\
        }while(0);
    
    #define x__err(format, ... )  \
        do{\
            COLOR_ERR;\
            printf(" ");\
            printf(format,##__VA_ARGS__);\
            printf("\n");\
        }while(0);


#else
    #define xdebug(format, ... )  ;
    #define xerror(format, ... )  ;
    #define xpanic(format, ... )  ;
    #define xlog(format, ... )    ;
    #define D(sth)                ;
#endif


