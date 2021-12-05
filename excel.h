#pragma once

#include "table.h"
#include "excellist.h"
#include "ncurses.h"

#include <cstring>
#include <algorithm>
#include <vector>
#include <cctype>
#include <set>
#include <mutex>

using namespace std;

class ExcelList;

/*------------------
       Excel
-------------------*/

class Excel
{
    enum COMMAND
    {
        ERROR = -1,
        EXIT = 0,
        NORMAL = 1,
        NEXT = 2,
        PREV = 3,
        DELETE = 4,
        FIND = 5,
        GOTO = 6,
        SAVE = 7,
    };

    Table *current_table;
    WINDOW *win;
    ExcelList *excelList;
    int start_row = 0;
    int start_col = 0;
    vector<vector<string>> history;
    vector<vector<string>> undo_history_to;
    vector<vector<Cell *>> undo_history_Cell;
    mutex* m;

    bool is_number(string str);
    bool is_number(vector<string> v_str);
    bool is_date(vector<string> v_str);
    bool is_cell_name(string str);
    bool is_cell_name(vector<string> v_str);
    bool is_expr(vector<string> v_str);
    bool is_func(vector<string> v_str);

public:
    Excel(WINDOW* _win, int max_row, int max_col, ExcelList* excelList, mutex* m);

    ~Excel();

    string input_command();

    int parse_user_input(string s);
    int command_line();
    void print_table(string look_for);

    void to_txt(ofstream& writeFile, int current_excel);
    void from_txt(ifstream& readFile);

    void undo();
    void redo();
};