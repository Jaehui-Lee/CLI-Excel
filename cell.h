#pragma once

#include "table.h"

#include "ctime"
#include <vector>
#include <stack>
#include <string>

using namespace std;

class Table;

/*------------------
        Cell
-------------------*/
enum Type
{
    NONE = -1,
    STRING = 0,
    NUMBER = 1,
    DATE = 2,
    EXPR = 3,
    FUNC = 4
};

class Cell
{
protected:
    int x, y;
    Table *table;
    Type type;

public:
    virtual string stringify() = 0;
    virtual int to_numeric() = 0;

    Cell(int x, int y, Table *table, Type type);
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

    StringCell(string data, int x, int y, Table *t, Type type);
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

    NumberCell(int data, int x, int y, Table *t, Type type);
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

    DateCell(string s, int x, int y, Table *t, Type type);
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

    // Analyze the expression
    void parse_expression();

public:
    ExprCell(string data, int x, int y, Table *t, Type type);

    string stringify();
    int to_numeric();
};


/*------------------
      ExprCell
-------------------*/

class FuncCell : public Cell
{
    string data;

    vector<string> func_vec;
    
        // Analyze the function
    void parse_function();

public:
    FuncCell(string data, int x, int y, Table *t, Type type);

    string stringify();
    int to_numeric();
};