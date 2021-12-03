#include <ncurses.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <list>

#include "initialpage.h"
#include "filemanager.h"
#include "excellist.h"

using namespace std;

int main()
{
    initscr();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    InitialPage ip(stdscr); // when program starts, show initial page
    int ip_choice = 0;         // user's choice on initial page

    int row, col;
    getmaxyx(stdscr, row, col);
    WINDOW *fm_win = newwin(row, col, 0, 0);

    FileManager fm(fm_win);
    string fm_choice;

    ExcelList* excelList = nullptr;

    while ((ip_choice = ip.init_screen()) != -1) // if user's choice is 'Exit', go out
    {
        if (ip_choice == 1) // Create New Excel
        {
            char f_name[80];
            int row, col;
            getmaxyx(stdscr, row, col);
            mvwprintw(stdscr, row - 1, 0, ">> ");
            wgetstr(stdscr, f_name);
            string to(f_name);
            excelList = new ExcelList(to);
            while (true)
            {
                Excel *m = excelList->get_current_excel();
                if (!(m->command_line()))
                    break;
            }
            delete excelList;
        }
        else if (ip_choice == 2) // Open Excel
        {
            if ((fm_choice = fm.init_screen()) == "")
                continue;

            excelList = new ExcelList(fm_choice);
            if (excelList->from_txt(fm_choice))
            {
                while (true)
                {
                    Excel *m = excelList->get_current_excel();

                    if (!(m->command_line()))
                        break;
                }
            }
            // else
            // {
            //     wattron(stdscr, COLOR_PAIR(1));
            //     mvwprintw(stdscr, row - 1, 0, "File doesn't exits");
            //     wattroff(stdscr, COLOR_PAIR(1));
            //     wrefresh(stdscr);
            //     sleep(2);
            // }
            delete excelList;
        }
        else if (ip_choice == 3) // Manual
        {
        }
        else // Exit
        {
            break;
        }
    }
    endwin();

    return 0;
}