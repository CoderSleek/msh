#ifndef MSHCONSTANTS_H
#define MSHCONSTANTS_H

#define BUF_SIZE 500
#define MSH_ENV "/.mshenv"
#define MSH_HISTORY "/.msh_history"

#define RED_COLOR "\033[1;31m"
#define YELLOW_COLOR "\033[1;33m"
#define NO_COLOR "\033[0m"
#define GREEN_COLOR "\033[1;32m"
#define BLUE_COLOR "\033[1;34m"

// 32 for uname, 255 for pwd, 4 for (space, \0) 6 for emoji, 7 for each color code
// 2 extra :)
#define PS1_SIZE 320

static const char *MSH_STARTUP = "Welcome to MSH the Modern Shell🙏\n";
static const char *MSH_FINISH = "Bye Bye 👋\n";

#endif