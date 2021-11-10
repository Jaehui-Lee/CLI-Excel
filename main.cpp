#pragma once

#include <ncurses.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <list>

#include "excel.h"

#define MAX_ROW_SIZE 18
#define MAX_COL_SIZE 18

using namespace std;

// class WinList
// {
// private:
//     list<WINDOW*> winList;
//     int wCount;
//     int currentWin;

// public:
//     WinList() : wCount(1), currentWin(0)
//     {
//         winList.push_back(new WINDOW);
//     }

//     WINDOW* getCurrentWindow() const
//     {
//         list<WINDOW*>::iterator iter = winList.begin()+currentWin;
//         return *iter;
//     }

//     WINDOW* insertWindow()
//     {
//         winList.push_back(new WINDOW);
//         wCount++;
//         currentWin++;
//         return winList.back();
//     }

//     WINDOW* deleteWindow()
//     {
//         list<WINDOW*>::iterator iter = winList.begin()+currentWin;
//         list<WINDOW*>::iterator next;
//         next = winList.erase(iter);
//         return *next;
//     }
// };

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
        execlp("printf", "printf", "\"\033[8;40;120t\"", NULL);
        exit(1);
        break;
    default:
        wait(&rt);
        // winsize 변경 후 아래 코드 적용
        usleep(1000);

        initscr();

        Excel m(stdscr, MAX_ROW_SIZE, MAX_COL_SIZE, 0);
        m.command_line();

        endwin();

        return 0;
    }
}