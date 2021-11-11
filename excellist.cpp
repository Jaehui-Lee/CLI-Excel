#include "excellist.h"

ExcelList::ExcelList() : excel_count(1), current_excel(0)
{
    WINDOW *win = newwin(WIN_ROW_SIZE, WIN_COL_SIZE, 0, 0);
    excelList.push_back(new Excel(win, MAX_ROW_SIZE, MAX_COL_SIZE, 0));
}

Excel *ExcelList::get_current_excel()
{
    list<Excel *>::iterator iter = excelList.begin();
    for (int i = 0; i < current_excel; i++)
        iter++;
    return *iter;
}

void ExcelList::move_next_window()
{
    if (current_excel + 1 == excel_count)
        insert_window();
    current_excel++;
}

void ExcelList::move_prev_window()
{
    if (current_excel != 0)
        current_excel--;
}

void ExcelList::insert_window()
{
    WINDOW *win = newwin(WIN_ROW_SIZE, WIN_COL_SIZE, 0, 0);
    excelList.push_back(new Excel(win, MAX_ROW_SIZE, MAX_COL_SIZE, 0));
    excel_count++;
}

void ExcelList::delete_window()
{
    list<Excel *>::iterator iter = excelList.begin();
    for (int i = 0; i < current_excel; i++)
        iter++;
    excelList.erase(iter);
    excel_count--;
    current_excel--;
}