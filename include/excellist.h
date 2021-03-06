#pragma once

#include "excel.h"

#include <string>
#include <iostream>
#include <list>
#include <sstream>
#include <mutex>

#define MAX_ROW_SIZE 30
#define MAX_COL_SIZE 26

using namespace std;

class Excel;

class ExcelList
{
private:
    list<Excel *> excelList;
    int excel_count;   // from 1
    int current_excel; // from 1
    string f_name;
    mutex* cell_m;
    mutex* win_m;
    

public:
    ExcelList(string f_name, mutex* cell_m, mutex* win_m);

    Excel *get_current_excel();
    int get_current_page();
    int get_excel_count();
    void move_next_window();
    void move_prev_window();
    void insert_window();
    void delete_window();
    void move_to_window(string to);

    void to_txt(string to = "");
    bool from_txt(string from);
};