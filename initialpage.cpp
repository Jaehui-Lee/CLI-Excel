#include "initialpage.h"

/*------------------
    InitialPage
-------------------*/

InitialPage::InitialPage(WINDOW* _win) : win(_win) {}

InitialPage::~InitialPage() {}

int InitialPage::init_screen()
{
    int startx = 0;
    int starty = 0;

    WINDOW *menu_win;
    int highlight = 1;
    int choice = 0;
    int c;
    int row, col;

    //initscr();
    clear();
    noecho();
    cbreak(); // Line buffering disabled. pass on everything
    curs_set(0);

    getmaxyx(win, row, col);
    startx = (col - WIDTH) / 2;
    starty = (row - HEIGHT) / 2;

    menu_win = newwin(HEIGHT, WIDTH, starty, startx);
    keypad(menu_win, TRUE);
    mvwprintw(win, 0, 0, "Use arrow keys to go up and down, Press enter to select a choice");
    wrefresh(win);
    print_menu(menu_win, highlight);
    while (1)
    {
        c = wgetch(menu_win);
        switch (c)
        {
        case KEY_UP:
            if (highlight == 1)
                highlight = n_choices;
            else
                --highlight;
            break;
        case KEY_DOWN:
            if (highlight == n_choices)
                highlight = 1;
            else
                ++highlight;
            break;
        case 10:
            choice = highlight;
            echo();
            raw();
            curs_set(1);
            return choice;
            break;
        default:
            break;
        }
        print_menu(menu_win, highlight);
        if (choice != 0) /* User did a choice come out of the infinite loop */
            break;
    }
    clrtoeol();
    //endwin();
    echo();
    raw();
    curs_set(1);
    return -1;
}

void InitialPage::print_menu(WINDOW *menu_win, int highlight)
{
    int x, y, i;

    x = 20;
    y = 5;
    box(menu_win, 0, 0);
    for (i = 0; i < n_choices; ++i)
    {
        if (highlight == i + 1) /* High light the present choice */
        {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        }
        else
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        y += 2;
    }
    wrefresh(menu_win);
}