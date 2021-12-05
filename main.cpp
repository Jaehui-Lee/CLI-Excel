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
#include <chrono>
#include <mutex>

#include "initialpage.h"
#include "filemanager.h"
#include "excellist.h"

using namespace std;

ExcelList* excelList = nullptr;

mutex* cell_m = new mutex();
mutex* win_m = new mutex();


inline void rtrim(string &s)
{
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !isspace(ch);
    }).base(), s.end());
}

void undo(int signum);
void redo(int signum);

void auto_save();

int main()
{
    signal(SIGINT, SIG_IGN);
    signal(SIGKILL, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGSTOP, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

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
            excelList = new ExcelList(to, cell_m, win_m);
            signal(SIGTSTP, undo);
            signal(SIGINT, redo);
            thread th(auto_save);
            while (true)
            {
                Excel *m = excelList->get_current_excel();
                if (!(m->command_line()))
                    break;
            }
            delete excelList;
            excelList = nullptr;
            signal(SIGTSTP, SIG_IGN);
            signal(SIGINT, SIG_IGN);
            th.join();
        }
        else if (ip_choice == 2) // Open Excel
        {
            if ((fm_choice = fm.init_screen()) == "q")
                    continue;
            if ( fm_choice.substr(fm_choice.length()-4) != ".txt" )
                    continue;

            excelList = new ExcelList(fm_choice, cell_m, win_m);
            signal(SIGTSTP, undo);
            signal(SIGINT, redo);
            thread th(auto_save);
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
            excelList = nullptr;
            signal(SIGTSTP, SIG_IGN);
            signal(SIGINT, SIG_IGN);
            th.join();
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

void auto_save()
{
    if ( excelList == nullptr )
        return;
    while(true)
    {
        for ( int i = 0 ; i < 30 ; i++ )
        {
            this_thread::sleep_for(1s);
            if ( excelList == nullptr )
                return;
        }
        lock_guard<mutex> cell_lock(*cell_m);
        lock_guard<mutex> win_lock(*win_m);
        if ( excelList == nullptr )
            return;
        excelList->to_txt();
    }
}

void undo(int signum)
{
    if ( excelList != nullptr )
    {
        lock_guard<mutex> cell_lock(*cell_m);
        Excel *m = excelList->get_current_excel();
        m->undo();
    }
}

void redo(int signum)
{
    if ( excelList != nullptr )
    {
        lock_guard<mutex> cell_lock(*cell_m);
        Excel *m = excelList->get_current_excel();
        m->redo();
    }
}