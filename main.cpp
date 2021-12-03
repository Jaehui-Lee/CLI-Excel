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
#include "excellist.h"

using namespace std;

int main()
{
    initscr();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    InitialPage ip(stdscr); // when program starts, show initial page
    int choice = 0;         // user's choice on initial page

    ExcelList* excelList = nullptr;

    while ((choice = ip.init_screen()) != -1) // if user's choice is 'Exit', go out
    {
        if (choice == 1) // Create New Excel
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
        else if (choice == 2) // Open Excel
        {
            char f_name[80];
            int row, col;
            getmaxyx(stdscr, row, col);
            mvwprintw(stdscr, row - 1, 0, ">> ");
            wgetstr(stdscr, f_name);
            string from(f_name);
            excelList = new ExcelList(from);
            if (excelList->from_txt(from))
            {
                while (true)
                {
                    Excel *m = excelList->get_current_excel();

                    if (!(m->command_line()))
                        break;
                }
            }
            else
            {
                wattron(stdscr, COLOR_PAIR(1));
                mvwprintw(stdscr, row - 1, 0, "File doesn't exits");
                wattroff(stdscr, COLOR_PAIR(1));
                wrefresh(stdscr);
                sleep(2);
            }
            delete excelList;
        }
        else if (choice == 3) // Manual
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