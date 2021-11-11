#include "ncurses.h"

/*------------------
    InitialPage
-------------------*/

class InitialPage
{
    enum
    {
        WIDTH = 60,
        HEIGHT = 20
    };

    const char *choices[4] = {
        "Create New Excel",
        "Open Excel",
        "Manual",
        "Exit",
    };
    
    int n_choices = sizeof(choices) / sizeof(char *);

public:
    InitialPage();

    ~InitialPage();

    int init_screen();
    void print_menu(WINDOW *menu_win, int highlight);

};