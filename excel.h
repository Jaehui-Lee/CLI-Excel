#pragma once

#include "table.h"
#include "excellist.h"
#include "ncurses.h"

#include <algorithm>

using namespace std;

//서명원 추가 내용///////////
typedef struct command_stack * CSPointer;
typedef struct command_stack{
    CSPointer back, front;
    char command[80];
}CS;

void save_stack(char * cstr);
void delete_stack();
char * pop_stack();
char * repush_stack();
void back(int signum);
void redo(int signum);
/////////////////////////////

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