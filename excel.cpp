#include "excel.h"

/*------------------
       Excel
-------------------*/

Excel::Excel(WINDOW *_win, int max_row, int max_col, int choice = 0) : win(_win)
{
    switch (choice)
    {
    case 0:
        current_table = new TxtTable(max_row, max_col);
        break;
        // case 1:
        //     current_table = new CSVTable(max_row, max_col);
        //     break;
        // default:
        //     current_table = new HtmlTable(max_row, max_col);
    }
}

Excel::~Excel()
{
    delete current_table;
}

int Excel::parse_user_input(string s)
{
    int next = 0;
    string command = "";
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == ' ')
        {
            command = s.substr(0, i);
            next = i + 1;
            break;
        }
        else if (i == s.length() - 1)
        {
            command = s.substr(0, i + 1);
            next = i + 1;
            break;
        }
    }

    if ( command == "next" )
        return 2;

    if ( command == "prev" )
        return 3;
    
    if ( command == "delete" )
        return 4;

    string to = "";
    for (int i = next; i < s.length(); i++)
    {
        if (s[i] == ' ' || i == s.length() - 1)
        {
            to = s.substr(next, i - next);
            next = i + 1;
            break;
        }
        else if (i == s.length() - 1)
        {
            to = s.substr(0, i + 1);
            next = i + 1;
            break;
        }
    }

    // Cell 이름으로 받는다.
    int col = to[0] - 'A';
    int row = atoi(to.c_str() + 1) - 1;

    string rest = s.substr(next);

    if (command == "sets")
    {
        current_table->reg_cell(new StringCell(rest, row, col, current_table), row,
                                col);
    }
    else if (command == "setn")
    {
        current_table->reg_cell(
            new NumberCell(atoi(rest.c_str()), row, col, current_table), row, col);
    }
    else if (command == "setd")
    {
        current_table->reg_cell(new DateCell(rest, row, col, current_table), row,
                                col);
    }
    else if (command == "sete")
    {
        current_table->reg_cell(new ExprCell(rest, row, col, current_table), row,
                                col);
    }
    // else if (command == "out")
    // {
    //     ofstream out(to);
    //     out << *current_table;
    //     std::cout << to << " 에 내용이 저장되었습니다" << std::endl;
    // }
    else if (command == "exit")
    {
        return 0;
    }

    return 1;
}

void Excel::print_table()
{
    int row, col;
    getmaxyx(win, row, col);
    wclear(win);
    wprintw(win, current_table->print_table().c_str());
    mvwprintw(win, row - 1, 0, ">> ");
    wrefresh(win);
}

int Excel::command_line()
{
    char cstr[80];
    
    print_table();
    wgetstr(win, cstr);
    string s(cstr);

    int ret;
    while ((ret = parse_user_input(s)))
    {
        if ( ret == 2 || ret == 3 || ret == 4 )
            return ret;
        print_table();
        wgetstr(win, cstr);
        s = cstr;
    }
    return ret;
}