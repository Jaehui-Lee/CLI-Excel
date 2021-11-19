#pragma once

#include "table.h"

#include "ctime"
#include <vector>
#include <stack>
#include <string>
#include <random>

#define MAX_ROW_SIZE 18
#define MAX_COL_SIZE 20

using namespace std;

class Table;

/*------------------
        Cell
-------------------*/

class Cell
{
protected:
    int x, y;
    Table *table;

public:
    virtual string stringify() = 0;
    virtual int to_numeric() = 0;

    Cell(int x, int y, Table *table);
};

/*------------------
     StringCell
-------------------*/

class StringCell : public Cell
{
    string data;

public:
    string stringify();
    int to_numeric();

    StringCell(string data, int x, int y, Table *t);
};

/*------------------
     NumberCell
-------------------*/

class NumberCell : public Cell
{
    int data;

public:
    string stringify();
    int to_numeric();

    NumberCell(int data, int x, int y, Table *t);
};

/*------------------
      DateCell
-------------------*/

class DateCell : public Cell
{
    time_t data;

public:
    string stringify();
    int to_numeric();

    DateCell(string s, int x, int y, Table *t);
};

/*------------------
      ExprCell
-------------------*/

class ExprCell : public Cell
{
    string data;

    vector<string> exp_vec;

    // return operator precedence
    int precedence(char c);

public:
    ExprCell(string data, int x, int y, Table *t);

    // Analyze the expression
    void parse_expression();

    string stringify();
    int to_numeric();
};


/*------------------
      FuncCell
-------------------*/

class FuncCell : public Cell
{
    string data;
    int value; // for RAND, RANDBETWEEN
    bool valid; // for RAND, RANDBETWEEN

    vector<string> func_vec;
    

public:
    FuncCell(string data, int x, int y, Table *t);

    // Analyze the function
    void parse_function();
    void calculate();

    string stringify();
    int to_numeric();
};