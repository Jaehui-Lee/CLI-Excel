#include "excellist.h"

ExcelList::ExcelList() : excel_count(1), current_excel(0)
{
    WINDOW *win = newwin(WIN_ROW_SIZE, WIN_COL_SIZE, 0, 0);
    excelList.push_back(new Excel(win, MAX_ROW_SIZE, MAX_COL_SIZE));
}

Excel *ExcelList::get_current_excel()
{
    list<Excel *>::iterator iter = excelList.begin();
    for (int i = 0; i < current_excel; i++)
        iter++;
    return *iter;
}

int ExcelList::get_current_page()
{
    return current_excel;
}

int ExcelList::get_excel_count()
{
    return excel_count;
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
    excelList.push_back(new Excel(win, MAX_ROW_SIZE, MAX_COL_SIZE));
    excel_count++;
}

void ExcelList::delete_window()
{
    if ( excel_count == 1 )
        return;
    list<Excel *>::iterator iter = excelList.begin();
    for (int i = 0; i < current_excel; i++)
        iter++;
    excelList.erase(iter);
    if ( excel_count > 1 )
        excel_count--;
    if ( current_excel > 0 )
        current_excel--;
}