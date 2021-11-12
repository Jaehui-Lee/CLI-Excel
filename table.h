#pragma once

#include "cell.h"

#include <iostream>
#include <string>

using namespace std;

class Cell;

/*------------------
        Table
-------------------*/

class Table
{
protected:
    
    int max_row_size, max_col_size;

    // A two-dimensional array that stores Cell*
    Cell ***data_table;

public:
    Table(int max_row_size, int max_col_size);

    virtual ~Table();

    // Register a new cell
    void reg_cell(Cell *c, int row, int col);

    // Returns the integer value of the cell
    // s : cell name (such as A3, B6)
    int to_numeric(const string &s);

    // Call the cell by row and column number.
    int to_numeric(int row, int col);

    // Returns the string of the cell
    string stringify(const string &s);
    string stringify(int row, int col);

    string repeat_char(int n, char c);

    // change number to column name ( 0->A, 1->B, 2->C, ... , 25->Z )
    string col_num_to_str(int n);

    string print_table();
};