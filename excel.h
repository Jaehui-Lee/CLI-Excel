#pragma once

#include "table.h"
#include "excellist.h"
#include "ncurses.h"

#include <algorithm>

using namespace std;

class ExcelList;

/*------------------
       Excel
-------------------*/

class Excel
{
    enum COMMAND
    {
        EXIT = 0,
        OTHERS = 1,
        NEXT = 2,
        PREV = 3,
        DELETE = 4,
        FIND = 5,
    };

    Table *current_table;
    WINDOW *win;
    ExcelList *excelList;

public:
    Excel(WINDOW* _win, int max_row, int max_col, ExcelList* excelList);

    ~Excel();

    int parse_user_input(string s);
    int command_line();
    void print_table(string look_for);

    void to_txt(ofstream& writeFile, int current_excel);
    void from_txt(ifstream& readFile);
};