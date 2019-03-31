#ifndef __MIPSX_COLOR_H__
#define __MIPSX_COLOR_H__
/* for code in {0..255}
 * > do echo -e "\e[38;5;${code}m"'\\e[38;5;'"$code"m"\e[0m"
 * > done
 * http://jafrog.com/2013/11/23/colors-in-terminal.html 
 */

#define TERMINAL_FONT_BLACK       "\033[1;30m"
#define TERMINAL_FONT_L_RED       "\033[0;31m"    /* light red */
#define TERMINAL_FONT_RED         "\033[1;31m"    /* red */
#define TERMINAL_FONT_GREEN       "\033[1;32m"
#define TERMINAL_FONT_YELLOW      "\033[1;33m"
#define TERMINAL_FONT_BLUE        "\033[1;34m"
#define TERMINAL_FONT_PURPLE      "\033[1;35m"
#define TERMINAL_FONT_CYAN        "\033[1;36m"
#define TERMINAL_FONT_WHITE       "\033[1;37m"

/* background color */
#define TERMINAL_BACK_BLACK       "\033[1;40m"
#define TERMINAL_BACK_L_RED       "\033[0;41m"    /* light red */
#define TERMINAL_BACK_RED         "\033[1;41m"    /* red */
#define TERMINAL_BACK_GREEN       "\033[1;42m"
#define TERMINAL_BACK_YELLOW      "\033[1;43m"
#define TERMINAL_BACK_BLUE        "\033[1;44m"
#define TERMINAL_BACK_PURPLE      "\033[1;45m"
#define TERMINAL_BACK_CYAN        "\033[1;46m"
#define TERMINAL_BACK_WHITE       "\033[1;47m"

#define TERMINAL_No_Color         "\033[0m"     /* reset all */
#define TERMINAL_Blink            "\033[5m"
#define TERMINAL_Bold             "\033[1m"
#define TERMINAL_Underline        "\033[4m"

#endif