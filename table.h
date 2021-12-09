#pragma once

#include "cell.h"
#include <set>
#include "ncurses.h"
#include <iostream>
#include <string>
#include <typeinfo>
#include <fstream>
#include <ncurses.h>
#include <map>
#include <unistd.h>
#include <vector>
#include <list>
#include <iterator>

using namespace std;

class Cell;

/*------------------
        Table
-------------------*/

class Table
{
protected:
    
    WINDOW* win;
    int max_row_size, max_col_size;
    int number_of_cell;
    string table_str;
    int row_size = 0;
    int col_size = 0;
    vector<int> cross_pos;

    // A two-dimensional array that stores Cell*
    vector<vector<list<Cell *>>> data_table;

public:
    Table(WINDOW* win, int max_row_size, int max_col_size, string table_str, vector<int> cross_pos);

    virtual ~Table();

    // Register a new cell ( including removing )
    void reg_cell(Cell *c, int row, int col);
    void sort_cell(vector<string> where_vec, vector<string> how_vec);
    set<int> Expr_set;
    set<int> Func_set;

    // Get data of FuncCell or ExpreCell
    string get_data(const string& s);

    // Returns the integer value of the cell
    // s : cell name (such as A3, B6)
    double to_numeric(const string &s);

    // Call the cell by row and column number.
    double to_numeric(int row, int col);

    // Returns the string of the cell
    string stringify(const string &s);
    string stringify(int row, int col);

    // void repeat_char(int n, const char* c);

    // change number to column name ( 0->A, 1->B, 2->C, ... , 25->Z )
    string col_num_to_str(int n);

    bool is_empty(int row, int col);
    bool is_empty(const string& s);

    const type_info& get_cell_type(int row, int col);
    const type_info& get_cell_type(const string& s);

    void print_table(int start_row, int start_col, string look_for);
    void make_table();
    string repeat_char(int n, char c);

    int get_row_size();
    int get_col_size();

    void to_txt(ofstream& writeFile);

    Cell * undo(const string &s);

};