#include "manual.h"

/*------------------
       Manual
-------------------*/

Manual::Manual(WINDOW* _win) : win(_win) { content.clear(); }

Manual::~Manual() {}

void Manual::init_screen()
{
    int start = 0;
    int row, col;
    getmaxyx(win, row, col);

    ifstream readFile("manual.txt");
    if ( readFile.is_open() )
    {
        string str;
        for ( int i = 0 ; !readFile.eof() ; i++ )
        {
            getline(readFile, str);
            content.push_back(str);
        }
        readFile.close();
    }
    else
        return;

    wclear(win);
    noecho();
    cbreak();
    curs_set(0);
    keypad(win, TRUE);

    mvwprintw(win, 0, 0, "Use arrow keys to go up and down");
    mvwprintw(win, 1, 0, "To return to the initial screen, press 'q' or 'Q'");
    wrefresh(win);
    print_file(row, start);

    int ch;
    while (1)
    {
        ch = wgetch(win);

        if ( ch == KEY_UP )
        {
            if ( start > 0 )
                start--;
        }
        else if ( ch == KEY_DOWN )
        {
            if ( start+row-1 < content.size() )
                start++;
        }
        if ( ch == 'q' || ch == 'Q' )
        {
            echo();
            raw();
            curs_set(1);
            return;
        }
        print_file(row, start);
    }
    echo();
    raw();
    curs_set(1);
}


void Manual::print_file(int row, int start)
{
    wclear(win);
    for ( int i = start ; i < start+row-1 ; i++ )
        mvwprintw(win, i-start, 0, content[i].c_str());

    wattron(win, A_REVERSE);
    mvwprintw(win, row-1, 0, "Manual page line %d (press q to quit)", start+1);
    wattroff(win, A_REVERSE);

    wrefresh(win);
}