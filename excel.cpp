#include "excel.h"

#include <algorithm>

/*------------------
       Excel
-------------------*/

Excel::Excel(WINDOW *_win, int max_row, int max_col) : win(_win)
{
    current_table = new Table(max_row, max_col);
}

Excel::~Excel()
{
    current_table->~Table();
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

    // transform command to lowercase
    transform(command.begin(), command.end(), command.begin(), ::tolower);

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

    // Cell name
    int col = to[0] - 'A';
    int row = atoi(to.c_str() + 1) - 1;

    string rest = s.substr(next);

    if (command == "sets") // set string
    {
        current_table->reg_cell(new StringCell(rest, row, col, current_table, STRING), row, col);
    }
    else if (command == "setn") // set number
    {
        current_table->reg_cell(new NumberCell(atoi(rest.c_str()), row, col, current_table, NUMBER), row, col);
    }
    else if (command == "setd") // set date
    {
        current_table->reg_cell(new DateCell(rest, row, col, current_table, DATE), row, col);
    }
    else if (command == "sete") // set expression
    {
        current_table->reg_cell(new ExprCell(rest, row, col, current_table, EXPR), row, col);
    }
    // else if (command == "save")
    // {

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
    mvwprintw(win, 0, 0, current_table->print_table().c_str()); // print table
    mvwprintw(win, row - 1, 0, ">> ");
    wrefresh(win);
}

int Excel::command_line()
{
    char cstr[80]; // user's command
    int ret; // return value(attribute) of user's command
    
    print_table(); // print table
    wgetstr(win, cstr);
    string s(cstr);

    while ((ret = parse_user_input(s))) // analysis of user input
    {
        if ( ret == 2 || ret == 3 || ret == 4 ) // if user's command is "next" or "prev" or "delete" (about sheet)
            return ret;
        print_table(); // if not, keep going
        wgetstr(win, cstr);
        s = cstr;
    }
    return ret;
}