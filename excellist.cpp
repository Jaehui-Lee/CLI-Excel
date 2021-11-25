#include "excellist.h"

ExcelList::ExcelList() : excel_count(1), current_excel(1)
{
    WINDOW *win = newwin(WIN_ROW_SIZE, WIN_COL_SIZE, 0, 0);
    excelList.push_back(new Excel(win, MAX_ROW_SIZE, MAX_COL_SIZE, this));
}

Excel *ExcelList::get_current_excel()
{
    list<Excel *>::iterator iter = excelList.begin();
    for (int i = 1; i < current_excel; i++)
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
    if (current_excel == excel_count)
        insert_window();
    current_excel++;
}

void ExcelList::move_prev_window()
{
    if (current_excel != 1)
        current_excel--;
}

void ExcelList::move_to_window(string to)
{
    int go_excel = std::stoi(to);
    if ((current_excel != go_excel)&&(go_excel <= excel_count)&&(go_excel >= 1))
    {
        current_excel = go_excel;
    }
}

void ExcelList::insert_window()
{
    WINDOW *win = newwin(WIN_ROW_SIZE, WIN_COL_SIZE, 0, 0);
    excelList.push_back(new Excel(win, MAX_ROW_SIZE, MAX_COL_SIZE, this));
    excel_count++;
}

void ExcelList::delete_window()
{
    if (excel_count == 1)
        return;
    list<Excel *>::iterator iter = excelList.begin();
    for (int i = 1; i < current_excel; i++)
        iter++;
    iter = excelList.erase(iter);
    if (excel_count > 1)
        excel_count--;
    if (current_excel > 1)
        current_excel--;
}

void ExcelList::to_txt(string to)
{
    ofstream writeFile(to.data());
    if (writeFile.is_open())
    {
        writeFile << excel_count << endl;
        list<Excel *>::iterator iter;
        int i = 1;
        for (iter = excelList.begin(); iter != excelList.end(); iter++)
        {
            (*iter)->to_txt(writeFile, i++);
        }
    }
    writeFile.close();
}

bool ExcelList::from_txt(string from)
{
    ifstream readFile(from.data());
    if ( readFile.is_open() )
    {
        int count = 0;
        readFile >> count;
        for ( int i = 0 ; i < count-1 ; i++ )
            insert_window();
        for ( int i = 0 ; i < count ; i++ )
        {
            readFile >> current_excel;
            Excel* excel = get_current_excel();
            excel->from_txt(readFile);
        }
        current_excel = 1;
        return true;
    }
    else
        return false;
}