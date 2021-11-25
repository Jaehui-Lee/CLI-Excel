#include "ncurses.h"

/*------------------
    InitialPage
-------------------*/

class InitialPage
{
    // choice box size of initial page
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

    WINDOW* win;

public:
    InitialPage(WINDOW* _win);

    ~InitialPage();

    int init_screen();
    void print_menu(WINDOW *menu_win, int highlight);

};