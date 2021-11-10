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
    // 행 및 열의 최대 크기
    int max_row_size, max_col_size;

    // 데이터를 보관하는 테이블
    // Cell* 을 보관하는 2차원 배열이라 생각하면 편하다
    Cell ***data_table;

public:
    Table(int max_row_size, int max_col_size);

    ~Table();

    // 새로운 셀을 row 행 col 열에 등록한다.
    void reg_cell(Cell *c, int row, int col);

    // 해당 셀의 정수값을 반환한다.
    // s : 셀 이름 (Ex. A3, B6 과 같이)
    int to_numeric(const string &s);

    // 행 및 열 번호로 셀을 호출한다.
    int to_numeric(int row, int col);

    // 해당 셀의 문자열을 반환한다.
    string stringify(const string &s);
    string stringify(int row, int col);

    virtual string print_table() = 0;
};