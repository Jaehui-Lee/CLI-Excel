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
    Table *current_table;
    WINDOW *win;
    ExcelList *excelList;

public:
    Excel(WINDOW* _win, int max_row, int max_col, ExcelList* excelList);

    ~Excel();

    int parse_user_input(string s);
    int command_line();
    void print_table();

    void to_txt(ofstream& writeFile, int current_excel);
};