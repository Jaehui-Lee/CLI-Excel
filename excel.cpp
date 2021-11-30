#include "excel.h"

/*------------------
       Excel
-------------------*/

Excel::Excel(WINDOW *_win, int max_row, int max_col, ExcelList *excelList) : win(_win), excelList(excelList)
{
    current_table = new Table(win, max_row, max_col);
}

Excel::~Excel()
{
    current_table->~Table();
    delete current_table;
}

bool Excel::is_number(string str)
{
    if (str.length() == 0)
        return false;
    else if (str.length() == 1 && !isdigit(str[0]))
        return false;
    else if (str.length() > 1 && str[0] == '0')
        return false;
    for (int i = 0; i < str.length(); i++)
    {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}

bool Excel::is_number(vector<string> v_str)
{
    for ( int i = 0 ; i < v_str.size() ; i++ )
    {
        if (v_str[i].length() == 0)
            return false;
        else if (v_str[i].length() == 1 && !isdigit(v_str[i][0]))
            return false;
        else if (v_str[i].length() > 1 && v_str[i][0] == '0')
            return false;
        for (int j = 1; j < v_str[i].length(); j++)
        {
            if (!isdigit(v_str[i][j]))
                return false;
        }
    }
    return true;
}

bool Excel::is_date(vector<string> v_str)
{
    for ( int i = 0 ; i < v_str.size() ; i++ )
    {
        if (v_str[i].length() != 10)
            return false;
        for (int j = 0; j < 10; j++)
        {
            if (j == 4 || j == 7)
            {
                if (v_str[i][j] != '-')
                    return false;
                continue;
            }
            if (!isdigit(v_str[i][j]))
                return false;
        }
    }
    return true;
}

bool Excel::is_cell_name(string str)
{
    if (str.length() < 2)
        return false;
    else if (!isupper(str[0]))
        return false;
    else if (!(str[1] >= '1' && str[1] <= '9'))
        return false;
    else if (stoi(str.substr(1)) > MAX_ROW_SIZE)
        return false;
    for (int j = 2; j < str.length(); j++)
    {
        if (!isdigit(str[j]))
            return false;
    }
    return true;
}

bool Excel::is_cell_name(vector<string> v_str)
{
    for (int i = 0; i < v_str.size(); i++)
    {
        if (v_str[i].length() < 2)
            return false;
        else if (!isupper(v_str[i][0]))
            return false;
        else if (!(v_str[i][1] >= '1' && v_str[i][1] <= '9'))
            return false;
        else if (stoi(v_str[i].substr(1)) > MAX_ROW_SIZE)
            return false;
        for (int j = 2; j < v_str[i].length(); j++)
        {
            if (!isdigit(v_str[i][j]))
                return false;
        }
    }
    return true;
}

bool Excel::is_expr(vector<string> v_str)
{
    for ( int i = 0 ; i < v_str.size() ; i++ )
    {
        // TODO
    }
    return true;
}

bool Excel::is_func(vector<string> v_str)
{
    for ( int i = 0 ; i < v_str.size() ; i++ )
    {
        // TODO
    }
    return true;
}

int Excel::parse_user_input(string s)
{
    string command = "";
    string to = "";
    string rest = "";

    // ex. setn A1:A5 1 2 3 4 5
    vector<string> v_to;   //  In case of many 'to' (ex. A1:A5)
    vector<string> v_rest; // In case of many 'rest' (ex. 1 2 3 4 5)

    // divide input-string into 3 parts by blank
    istringstream ss(s);
    getline(ss, command, ' ');
    getline(ss, to, ' ');
    getline(ss, rest);

    // transform command to lowercase
    transform(command.begin(), command.end(), command.begin(), ::tolower);

    if (command == "next")
    {
        excelList->move_next_window();
        return NEXT;
    }
    else if (command == "prev")
    {
        excelList->move_prev_window();
        return PREV;
    }
    else if (command == "delete")
    {
        excelList->delete_window();
        return DELETE;
    }
    else if (command == "exit")
    {
        int row, col;
        char str[10];
        getmaxyx(stdscr, row, col);
        while (true)
        {
            wattron(win, COLOR_PAIR(1));
            mvwprintw(win, row - 1, 0, "Do you want to save file?(Y/N)");
            wattroff(win, COLOR_PAIR(1));
            wprintw(win, " ");
            wrefresh(win);
            wgetstr(win, str);
            if (!strcmp(str, "Y") || !strcmp(str, "y"))
            {
                mvwprintw(win, row - 1, 0, "                                "); // remove "Do you want to save file?(Y/N)" on screen
                wattron(win, COLOR_PAIR(1));
                mvwprintw(win, row - 1, 0, "Enter the name of file :");
                wattroff(win, COLOR_PAIR(1));
                wprintw(win, " ");
                wrefresh(win);
                wgetstr(win, str);
                to = str;
                // file name can be only nothing except of .txt ( not .csv, .hwp, .xlsx, ...)
                if (to.find('.') != string::npos)
                {
                    if (to.substr(to.find('.')) != ".txt")
                        return ERROR;
                }
                excelList->to_txt(to);
                break;
            }
            else if (!strcmp(str, "N") || !strcmp(str, "n"))
            {
                break; // don't save and return to initial page
            }
            else
            {
                return NORMAL; // don't save and return to command line
            }
        }
        return EXIT;
    }
    
    if (to.find(':') == string::npos)
    {
        v_to.push_back(to);
        v_rest.push_back(rest);
    }
    else
    {
        // Cells based on range -> independent Cell
        // A1:A5 -> A1 A2 A3 A4 A5
        // push back to v_to
        ss = istringstream(to);
        string start, end;
        getline(ss, start, ':');
        getline(ss, end);

        // check cell name error
        if ( !is_cell_name(start) )
            return ERROR;
        if ( !is_cell_name(end) )
            return ERROR;

        for (char p = start[0]; p <= end[0]; p++)
        {
            for (int q = stoi(start.substr(1)); q <= stoi(end.substr(1)); q++)
            {
                string cell_name(1, p);
                cell_name += to_string(q);
                v_to.push_back(cell_name);
            }
        }
        // push back to v_rest
        ss = istringstream(rest);
        string value;
        while (getline(ss, value, ' '))
        {
            v_rest.push_back(value);
        }
        if (v_to.size() != v_rest.size())
            return ERROR;
    }

    if (command == "save") // save to txt
    {
        // file name can be only nothing except of .txt ( not .csv, .hwp, .xlsx, ...)
        if (to.find('.') != string::npos)
        {
            if (to.substr(to.find('.')) != ".txt")
                return ERROR;
        }
        excelList->to_txt(to);
        return SAVE;
    }
    else if (command == "find") // find cell
    {
        // "to" must be number
        if ( !is_number(to) )
            return ERROR;
        print_table(to);
        return FIND;
    }
    else if (command == "goto") // go to another excel
    {
        // "to" must be number > 0
        if ( !is_number(to) )
            return ERROR;
        excelList->move_to_window(to);
        return GOTO;
    }

    // check cell name error
    if ( !is_cell_name(v_to) )
        return ERROR;

    int col, row;

    if (command == "sets") // set string
    {
        for (int i = 0; i < v_to.size(); i++)
        {
            col = v_to[i][0] - 'A';
            row = stoi(v_to[i].substr(1)) - 1;
            current_table->reg_cell(new StringCell(v_rest[i], row, col, current_table), row, col);
        }
    }
    else if (command == "setn") // set number
    {
        // check value(number) error
        if ( !is_number(v_rest) )
            return ERROR;
        for (int i = 0; i < v_to.size(); i++)
        {
            col = v_to[i][0] - 'A';
            row = stoi(v_to[i].substr(1)) - 1;
            current_table->reg_cell(new NumberCell(stoi(v_rest[i]), row, col, current_table), row, col);
        }
        return NORMAL;
    }
    else if (command == "setd") // set date
    {
        // check value(date) error
        if ( !is_date(v_rest) )
            return ERROR;
        for (int i = 0; i < v_to.size(); i++)
        {
            col = v_to[i][0] - 'A';
            row = stoi(v_to[i].substr(1)) - 1;
            current_table->reg_cell(new DateCell(v_rest[i], row, col, current_table), row, col);
        }
        return NORMAL;
    }
    else if (command == "sete") // set expression
    {
        // check expression error
        if ( !is_expr(v_rest) )
            return ERROR;
        for (int i = 0; i < v_to.size(); i++)
        {
            col = v_to[i][0] - 'A';
            row = stoi(v_to[i].substr(1)) - 1;
            current_table->reg_cell(new ExprCell(v_rest[i], row, col, current_table), row, col);
        }
        return NORMAL;
    }
    else if (command == "setf") // set function
    {
        // check function error
        if ( !is_func(v_rest) )
            return ERROR;
        for (int i = 0; i < v_to.size(); i++)
        {
            col = v_to[i][0] - 'A';
            row = stoi(v_to[i].substr(1)) - 1;
            current_table->reg_cell(new FuncCell(v_rest[i], row, col, current_table), row, col);
        }
        return NORMAL;
    }

    return ERROR;
}

void Excel::print_table(string look_for = "")
{
    int row, col;
    string str = to_string(excelList->get_current_page()) + "/" + to_string(excelList->get_excel_count());
    getmaxyx(win, row, col);
    wclear(win);
    current_table->print_table(look_for);
    mvwprintw(win, row - 1, col - 10, str.c_str());
    mvwprintw(win, row - 1, 0, ">> ");
    wrefresh(win);
}

int Excel::command_line()
{
    char cstr[80]; // user's command
    int ret;       // return value(attribute) of user's command
    int row, col;

    getmaxyx(win, row, col);

    print_table(); // print table
    wgetstr(win, cstr);
    string s(cstr);

    while ((ret = parse_user_input(s))) // analysis of user input
    {
        if (ret == NEXT || ret == PREV || ret == DELETE || ret == GOTO)
        {
            return ret;
        }
        else if (ret == FIND) // if user's command is "find"
        {
        }
        else if (ret == ERROR)
        {
            mvwprintw(win, row - 1, 0, "                                ");
            wattron(stdscr, COLOR_PAIR(1));
            mvwprintw(stdscr, row - 1, 0, "        Wrong command!        ");
            wattroff(stdscr, COLOR_PAIR(1));
            wrefresh(stdscr);
            sleep(2);
            print_table();
        }
        else
            print_table(); // if not, keep going
        wgetstr(win, cstr);
        s = cstr;
    }
    return ret;
}

void Excel::to_txt(ofstream &writeFile, int current_excel)
{
    writeFile << current_excel << " ";
    current_table->to_txt(writeFile);
}

void Excel::from_txt(ifstream &readFile)
{
    int number_of_data = 0;
    readFile >> number_of_data;

    string name;  // A1, A2, B1, ...
    string type;  // STRING, NUMBER, DATE, EXPRESSION, FUNCTION
    string value; // 10, abc, 2021-01-01, A1+A2, SUM(A1:A2), ...
    for (int i = 0; i < number_of_data; i++)
    {
        readFile >> name >> type >> value;
        int col = name[0] - 'A';
        int row = stoi(name.substr(1)) - 1;
        if (type == "STRING") // set string
        {
            current_table->reg_cell(new StringCell(value, row, col, current_table), row, col);
        }
        else if (type == "NUMBER") // set number
        {
            current_table->reg_cell(new NumberCell(stoi(value), row, col, current_table), row, col);
        }
        else if (type == "DATE") // set date
        {
            current_table->reg_cell(new DateCell(value, row, col, current_table), row, col);
        }
        else if (type == "EXPRESSION") // set expression
        {
            current_table->reg_cell(new ExprCell(value, row, col, current_table), row, col);
        }
        else if (type == "FUNCTION") // set function
        {
            current_table->reg_cell(new FuncCell(value, row, col, current_table), row, col);
        }
    }
}