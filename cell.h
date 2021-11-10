#pragma once

#include "table.h"

#include "time.h"
#include <vector>
#include <stack>
#include <string>

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
    string *parsed_expr;

    vector<string> exp_vec;

    // 연산자 우선 순위를 반환
    int precedence(char c);

    // 수식을 분석
    void parse_expression();

public:
    ExprCell(string data, int x, int y, Table *t);

    string stringify();
    int to_numeric();
};