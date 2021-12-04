#include <ncurses.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <list>
#include <thread>

#include "initialpage.h"
#include "filemanager.h"
#include "excellist.h"

using namespace std;

inline void rtrim(string &s)
{
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !isspace(ch);
    }).base(), s.end());
}

void auto_save(ExcelList* excelList);

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

    // current directory path
    FILE *fpipe;
    char command[4] = "pwd";
    char str[30];

    fpipe = (FILE*)popen(command, "r");
    fgets(str, sizeof(str), fpipe);
    string start_dir = string(str);
    rtrim(start_dir);
    pclose(fpipe);

    FileManager fm(fm_win, start_dir);
    string fm_choice;

    ExcelList* excelList = nullptr;

    while ((ip_choice = ip.init_screen()) != -1) // if user's choice is 'Exit', go out
    {
        if (ip_choice == 1) // Create New Excel
        {
            // change path to directory(when program starts)
            chdir(start_dir.c_str());
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
            if ((fm_choice = fm.init_screen()) == "q")
                    continue;
            if ( fm_choice.substr(fm_choice.length()-4) != ".txt" )
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

void auto_save(ExcelList* excelList)
{
    
}