#include "excel.h"

/*------------------
       Excel
-------------------*/

Excel::Excel(WINDOW *_win, int max_row, int max_col, ExcelList *excelList, mutex* cell_m, mutex* win_m) : win(_win), excelList(excelList), history(vector<vector<string>>(0)), undo_history_to(vector<vector<string>>(0)), undo_history_Cell(vector<vector<Cell *>>(0)), win_m(win_m), cell_m(cell_m)
{
    current_table = new Table(win, max_row, max_col, "", vector<int>());
}

Excel::~Excel()
{
    current_table->~Table();
    delete current_table;
}

bool Excel::is_number(string str)
{
    int count = 0; // floating point counter
    if (str.length() == 0)
        return false;
    else if (str.length() == 1 && !isdigit(str[0]))
        return false;
    else if (str.length() > 1 && !( (str[0] >= '1' && str[0] <= '9') || str[0] == '-' ) )
        return false;
    else if (str.length() > 1 && str[0] == '-' && str[1] == '0')
        return false;
    for (int i = 1; i < str.length(); i++)
    {
        if (str[i] == '.')
        {
            if ( count == 1 )
                return false;
            else if ( i == str.length()-1 )
                return false;
            else
                count++;
            continue;
        }
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}

bool Excel::is_number(vector<string> v_str)
{
    for ( int i = 0 ; i < v_str.size() ; i++ )
    {
        int count = 0;
        if (v_str[i].length() == 0)
            return false;
        else if (v_str[i].length() == 1 && !isdigit(v_str[i][0]))
            return false;
        else if (v_str[i].length() > 1 && !((v_str[i][0] >= '1' && v_str[i][0] <= '9') || v_str[i][0] == '-'))
            return false;
        else if (v_str[i].length() > 1 && v_str[i][0] == '-' && v_str[i][1] == '0')
            return false;
        for (int j = 1; j < v_str[i].length(); j++)
        {
            if ( v_str[i][j] == '.' )
            {
                if ( count == 1 )
                    return false;
                else if ( j == v_str[i].length()-1 )
                    return false;
                else
                    count++;
                continue;
            }
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
    set<char> op{'+', '-', '*', '/'};
    for ( int i = 0 ; i < v_str.size() ; i++ )
    {
        if ( v_str[i].find("/0") != string::npos ) // divided by 0
            return false;
        vector<int> index_of_op{-1}; // index of operator(+,-,*,/)
        for ( int j = 0 ; j < v_str[i].length() ; j++ )
        {
            if ( isupper(v_str[i][j]) || isdigit(v_str[i][j]) ) // all character of string must be upper character or digit or operator(+,-,*,/)
                continue;
            else if ( op.find(v_str[i][j]) != op.end() )
            {
                if ( j == 0 || j == v_str[i].length()-1 || (op.find(v_str[i][j+1]) != op.end()) ) // oerators cannot be placed at the beginning or at the end
                    return false;
                index_of_op.push_back(j);
            }
            else
                return false;
        }
        if ( index_of_op.size() == 1 ) // no operator
        {
            if (!(is_cell_name(v_str[i]) || is_number(v_str[i])))
                return false;
        }
        else // at least 1 operator
        {
            index_of_op.push_back(v_str[i].length());
            for (int j = 0; j < index_of_op.size() - 1; j++)
            {
                string check = v_str[i].substr(index_of_op[j] + 1, index_of_op[j + 1] - index_of_op[j] - 1); // All strings except operators must be cell names or literal numbers
                if (!(is_cell_name(check) || is_number(check)))
                    return false;
            }
        }
    }
    return true;
}

bool Excel::is_func(vector<string> v_str)
{
    set<string> func{"SUM", "AVG", "PRODUCT", "COUNT", "MAX", "MIN", "RAND", "RANDBETWEEN", "SIN", "COS", "TAN"};
    for ( int i = 0 ; i < v_str.size() ; i++ )
    {
        if ( v_str[i].find('(') == string::npos )
            return false;
        if ( (v_str[i].find(')') == string::npos) || (v_str[i].find(')') != v_str[i].length()-1) )
            return false;
        int index_of_brk = v_str[i].find('('); // index of bracket '('
        string command = v_str[i].substr(0, index_of_brk);
        transform(command.begin(), command.end(), command.begin(), ::toupper);
        if ( func.find(command) == func.end() )
            return false;
        if ( command == "SUM" || command == "AVG" || command == "PRODUCT" || command == "MAX" || command == "MIN" )
        {
            string str = v_str[i].substr(index_of_brk+1);
            str.pop_back();
            vector<string> v_to;
            istringstream ss(str);
            string start, end;
            getline(ss, start, ':');
            getline(ss, end);

            if (!is_cell_name(start))
                return false;
            if (!is_cell_name(end))
                return false;

            for (char p = start[0]; p <= end[0]; p++)
            {
                for (int q = stoi(start.substr(1)); q <= stoi(end.substr(1)); q++)
                {
                    string cell_name(1, p);
                    cell_name += to_string(q);
                    v_to.push_back(cell_name);
                }
            }
            if ( !is_cell_name(v_to) )
                return false;
        }
        else if ( command == "COUNT")
        {
            string str = v_str[i].substr(index_of_brk+1);
            str.pop_back();
            if ( !is_number(str) )
                return false;
        }
        else if ( command == "RAND" )
        {
            string str = v_str[i].substr(index_of_brk+1);
            str.pop_back();
            if ( !str.empty() )
                return false;
        }
        else if ( command == "RANDBETWEEN" )
        {
            string str = v_str[i].substr(index_of_brk+1);
            str.pop_back();

            istringstream ss(str);
            string start, end;
            getline(ss, start, ',');
            getline(ss, end);

            if ( !is_number(start) )
                return false;
            if ( !is_number(end) )
                return false;
        }
        else if ( command == "SIN" || command == "COS" || command == "TAN" )
        {
            string str = v_str[i].substr(index_of_brk+1);
            str.pop_back();

            if ( !is_cell_name(str) && !is_number(str) )
                return false;
        }
    }
    return true;
}

int Excel::parse_user_input(string s)
{
    int row_size, col_size;
    getmaxyx(win, row_size, col_size);

    int col, row;

    if ( s == to_string(KEY_UP) )
    {
        if ( start_row > 0 )
            start_row--;
        return NORMAL;
    }
    else if ( s == to_string(KEY_DOWN) )
    {
        if ( start_row < current_table->get_row_size()-(row_size-5) )
            start_row++;
        return NORMAL;
    }
    else if ( s == to_string(KEY_LEFT) )
    {
        if ( start_col > 0 )
            start_col--;
        return NORMAL;
    }
    else if ( s == to_string(KEY_RIGHT) )
    {
        if ( start_col < current_table->get_col_size()-col_size )
            start_col++;
        return NORMAL;
    }

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
        lock_guard<mutex> win_lock(*win_m);
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
        lock_guard<mutex> win_lock(*win_m);
        excelList->delete_window();
        return DELETE;
    }
    else if (command == "exit")
    {
        lock_guard<mutex> cell_lock(*cell_m);
        lock_guard<mutex> win_lock(*win_m);
        int exit_row, exit_col;
        getmaxyx(win, exit_row, exit_col);
        char str[10];
        
        while (true)
        {
            wattron(win, COLOR_PAIR(1));
            mvwprintw(win, exit_row - 1, 0, "Do you want to save file?(Y/N)");
            wattroff(win, COLOR_PAIR(1));
            wprintw(win, " ");
            wrefresh(win);
            wgetstr(win, str);
            if (!strcmp(str, "Y") || !strcmp(str, "y"))
            {
                excelList->to_txt();
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
    }

    if (command == "save") // save to txt
    {
        lock_guard<mutex> cell_lock(*cell_m);
        lock_guard<mutex> win_lock(*win_m);
        if ( to == "" )
            excelList->to_txt();
        else
            excelList->to_txt(to);
        return SAVE;
    }
    else if (command == "find") // find cell
    {
        // "to" must be number
        if ( !is_number(to) )
            return ERROR;
        print_table(vector<string>(), to);
        return FIND;
    }
    else if (command == "goto") // go to another excel
    {
        lock_guard<mutex> win_lock(*win_m);
        // "to" must be number > 0
        if ( !is_number(to) )
            return ERROR;
        excelList->move_to_window(to);
        return GOTO;
    }
    else if (command == "get")
    {
        if ( !is_cell_name(v_to) )
            return ERROR;
        print_table(v_to, "");
        return GET;
    }

    // check cell name error
    if ( !is_cell_name(v_to) )
        return ERROR;

    if (command == "remove")
    {
        lock_guard<mutex> cell_lock(*cell_m);
        if ( !is_cell_name(v_to) )
            return ERROR;
        for (int i = 0; i < v_to.size(); i++)
        {
            col = v_to[i][0] - 'A';
            row = stoi(v_to[i].substr(1)) - 1;
            current_table->reg_cell(new EmptyCell(row, col, current_table), row, col);
        }
        undo_history_Cell.clear();
        undo_history_to.clear();
        history.push_back(v_to);
        return NORMAL;
    }
    else if (command == "sets") // set string
    {
        lock_guard<mutex> cell_lock(*cell_m);
        if (v_to.size() != v_rest.size())
            return ERROR;
        for (int i = 0; i < v_to.size(); i++)
        {
            col = v_to[i][0] - 'A';
            row = stoi(v_to[i].substr(1)) - 1;
            current_table->reg_cell(new StringCell(v_rest[i], row, col, current_table), row, col);
        }
        undo_history_Cell.clear();
        undo_history_to.clear();
        history.push_back(v_to);
        return NORMAL;
    }
    else if (command == "setn") // set number
    {
        lock_guard<mutex> cell_lock(*cell_m);
        if (v_to.size() != v_rest.size())
            return ERROR;
        // check value(number) error
        if ( !is_number(v_rest) )
            return ERROR;
        for (int i = 0; i < v_to.size(); i++)
        {
            col = v_to[i][0] - 'A';
            row = stoi(v_to[i].substr(1)) - 1;
            current_table->reg_cell(new NumberCell(stod(v_rest[i]), row, col, current_table), row, col);
        }
        undo_history_Cell.clear();
        undo_history_to.clear();
        history.push_back(v_to);
        return NORMAL;
    }
    else if (command == "setd") // set date
    {
        lock_guard<mutex> cell_lock(*cell_m);
        if (v_to.size() != v_rest.size())
            return ERROR;
        // check value(date) error
        if ( !is_date(v_rest) )
            return ERROR;
        for (int i = 0; i < v_to.size(); i++)
        {
            col = v_to[i][0] - 'A';
            row = stoi(v_to[i].substr(1)) - 1;
            current_table->reg_cell(new DateCell(v_rest[i], row, col, current_table), row, col);
        }
        undo_history_Cell.clear();
        undo_history_to.clear();
        history.push_back(v_to);
        return NORMAL;
    }
    else if (command == "sete") // set expression
    {
        lock_guard<mutex> cell_lock(*cell_m);
        if (v_to.size() != v_rest.size())
            return ERROR;
        // check expression error
        if ( !is_expr(v_rest) )
            return ERROR;
        for (int i = 0; i < v_to.size(); i++)
        {
            col = v_to[i][0] - 'A';
            row = stoi(v_to[i].substr(1)) - 1;
            current_table->reg_cell(new ExprCell(v_rest[i], row, col, current_table), row, col);
        }
        undo_history_Cell.clear();
        undo_history_to.clear();
        history.push_back(v_to);
        return NORMAL;
    }
    else if (command == "setf") // set function
    {
        lock_guard<mutex> cell_lock(*cell_m);
        if (v_to.size() != v_rest.size())
            return ERROR;
        // check function error
        if ( !is_func(v_rest) )
            return ERROR;
        for (int i = 0; i < v_to.size(); i++)
        {
            col = v_to[i][0] - 'A';
            row = stoi(v_to[i].substr(1)) - 1;
            current_table->reg_cell(new FuncCell(v_rest[i], row, col, current_table), row, col);
        }
        undo_history_Cell.clear();
        undo_history_to.clear();
        history.push_back(v_to);
        return NORMAL;
    }
    else if (command == "sort")
    {
        current_table->sort_cell(v_to, v_rest);
        undo_history_Cell.clear();
        undo_history_to.clear();
        history.push_back(v_to);
        return NORMAL;
    }

    return ERROR;
}

void Excel::print_table(vector<string> get, string look_for = "")
{
    int row, col;
    string str = to_string(excelList->get_current_page()) + "/" + to_string(excelList->get_excel_count());
    getmaxyx(win, row, col);
    wclear(win);
    current_table->print_table(start_row, start_col, look_for);
    wmove(win, row-2, 0);
    for ( int i = 0 ; i < get.size() ; i++ )
    {
        wattron(win, COLOR_PAIR(1));
        wprintw(win, get[i].c_str());
        wprintw(win, ":");
        if ( current_table->is_empty(get[i]) )
        {
            wattroff(win, COLOR_PAIR(1));
            wprintw(win, " ");
            continue;
        }
        if ( current_table->get_cell_type(get[i]).name() == typeid(FuncCell).name() || current_table->get_cell_type(get[i]).name() == typeid(ExprCell).name() )
        {
            wprintw(win, current_table->get_data(get[i]).c_str());
        }
        else
        {
            wprintw(win, current_table->stringify(get[i]).c_str());
        }
        
        wattroff(win, COLOR_PAIR(1));
        if ( i != get.size()-1 )
            wprintw(win, " ");
    }
    mvwprintw(win, row - 1, col - 10, str.c_str());
    mvwprintw(win, row - 1, 0, ">> ");
    wrefresh(win);
}

string Excel::input_command()
{
    string s;
    int ch;
    int row, col;
    while(true)
    {
        ch = wgetch(win);
        if ( ch == '\n' )
            break;
        if ( ch == KEY_BACKSPACE )
        {
            getyx(win, row, col);
            if ( col == 2 )
            {
                wmove(win, row, col+1);
            }
            else
            {
                wprintw(win, " ");
                wmove(win, row, col);
                wrefresh(win);
            }
            if ( s.empty() )
                continue;
            s.pop_back();
        }
        else
        {
            s += ch;
            if ( ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT || ch == KEY_RIGHT )
            {
                return to_string(ch);
            }
        }
    }
    return s;
}

int Excel::command_line()
{
    char cstr[80]; // user's command
    int ret;       // return value(attribute) of user's command
    int row, col;

    cbreak();
    keypad(win, TRUE);

    getmaxyx(win, row, col);

    print_table(vector<string>()); // print table

    string s;

    s = input_command();

    while ((ret = parse_user_input(s))) // analysis of user input
    {
        if (ret == NEXT || ret == PREV || ret == DELETE || ret == GOTO)
        {
            break;
        }
        else if (ret == FIND) // if user's command is "find"
        {
        }
        else if (ret == GET)
        {
        }
        else if (ret == ERROR)
        {
            string blank = string(col-10, ' ');
            mvwprintw(win, row - 1, 0, blank.c_str());
            wattron(win, COLOR_PAIR(1));
            mvwprintw(win, row - 1, 0, "        Wrong command!        ");
            wattroff(win, COLOR_PAIR(1));
            wrefresh(win);
            sleep(1);
            print_table(vector<string>());
        }
        else
            print_table(vector<string>()); // if not, keep going

        s = input_command();
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
            current_table->reg_cell(new NumberCell(stod(value), row, col, current_table), row, col);
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

void Excel::undo()
{
    if ( history.empty() )
        return;
    vector<string> h_to;
    vector<Cell *> h_cell;
    vector<string> h = history.back();
    history.pop_back();

    for ( int i = 0 ; i < h.size() ; i++ )
    {
        h_to.push_back(h[i]);
        h_cell.push_back(current_table->undo(h[i]));
    }
    undo_history_to.push_back(h_to);
    undo_history_Cell.push_back(h_cell);
    print_table(vector<string>());
}

void Excel::redo()
{   
    if(undo_history_Cell.empty())
        return;
    vector<string> h_to = undo_history_to.back();
    undo_history_to.pop_back();
    vector<Cell *> h_cell = undo_history_Cell.back();
    undo_history_Cell.pop_back();
    history.push_back(h_to);

    Cell * temp;

    for ( int i = 0 ; i < h_to.size() ; i++ )
    {
        int col = h_to[i][0] - 'A';
        int row = stoi(h_to[i].substr(1)) - 1;
        if ( h_cell[i] )
            current_table->reg_cell(h_cell[i], row, col);
    }

    print_table(vector<string>());
}