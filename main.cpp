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

#define WIN_ROW_SIZE 40
#define WIN_COL_SIZE 120

using namespace std;

int main()
{
    int rt;
    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        perror("fork failed");
        break;
    case 0:
        execlp("printf", "printf", "\"\033[8;40;120t\"", NULL); // change winsize -> row:col = 40:120
        exit(1);
        break;
    default:
        wait(&rt);

        usleep(100000);

        initscr();

        InitialPage ip(stdscr); // when program starts, show initial page
        int choice = 0; // user's choice on initial page

        ExcelList excelList; // n excel sheet

        while ((choice = ip.init_screen()) != -1) // if user's choice is 'Exit', go out
        {
            if (choice == 1) // Create New Excel
            {
                while (true)
                {
                    Excel *m = excelList.get_current_excel();
                    /*
                        if user enter command 'next' or 'prev' or 'delete' or 'exit', m->command_line() return
                    */
                    int ret = m->command_line();
                    if (ret == 2) // next
                    {
                        excelList.move_next_window();
                    }
                    else if (ret == 3) // prev
                    {
                        excelList.move_prev_window();
                    }
                    else if (ret == 4) // delete
                    {
                        excelList.delete_window();
                    }
                    else // exit
                    {
                        break;
                    }
                }
            }
            else if (choice == 2) // Open Excel
            {
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
}