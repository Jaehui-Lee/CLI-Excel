#include "table.h"

/*------------------
        Table
-------------------*/

Table::Table(WINDOW *win, int max_row_size, int max_col_size, string table_str, vector<int> cross_pos)
    : win(win), max_row_size(max_row_size), max_col_size(max_col_size), number_of_cell(0), table_str(table_str), cross_pos(cross_pos)
{
    data_table = vector<vector<list<Cell *>>>(MAX_ROW_SIZE, vector<list<Cell *>>(MAX_COL_SIZE, list<Cell *>(0)));
}

Table::~Table() 
{
    for ( int i = 0 ; i < max_row_size ; i++ )
    {
        for ( int j = 0 ; j < max_col_size ; j++ )
        {
            for ( auto iter = data_table[i][j].begin() ; iter != data_table[i][j].end() ; iter++ )
            {
                delete (*iter);
            }
        }
    }
}

void Table::reg_cell(Cell *c, int row, int col)
{
    if ( typeid(*c).name() == typeid(EmptyCell).name() )
    {
        if ( data_table[row][col].empty() )
            return;
        else if ( get_cell_type(row, col).name() == typeid(EmptyCell).name() )
            return;
        else
        {
            number_of_cell--;
            data_table[row][col].push_back(c);
            return;
        }
    }
    else
    {
        if ( data_table[row][col].empty() )
            number_of_cell++;
        else if ( get_cell_type(row, col).name() == typeid(EmptyCell).name() )
            number_of_cell++;
        data_table[row][col].push_back(c);
    }
}

void Table::sort_cell(vector<string> where_vec, vector<string> how_vec)
{
    int num = where_vec.size();
    int counting = 0;
    int Expr_count = 0;
    int Func_count = 0;
    for (int i = 0; i < num; i++)
    {
        int col = where_vec[i][0] - 'A';
        int row = atoi(where_vec[i].c_str() + 1) - 1;
        if (!data_table[row][col].empty() && get_cell_type(row, col).name() != typeid(EmptyCell).name() && get_cell_type(row, col).name() != typeid(DateCell).name() && get_cell_type(row, col).name() != typeid(StringCell).name()) 
        {
            counting++;
            if (get_cell_type(row, col).name() == typeid(ExprCell).name())
            {
                Expr_set.insert(data_table[row][col].back()->to_numeric());
                Expr_count++;
            }
            if (get_cell_type(row, col).name() == typeid(FuncCell).name())
            {
                Func_set.insert(data_table[row][col].back()->to_numeric());
                Func_count++;
            }
        }
    }
    
    int sort_value[counting];
    int sort_row[counting];
    int sort_col[counting];
    string func_data[Func_count];
    string expr_data[Expr_count];
    int func_value[Func_count];
    int expr_value[Expr_count];
    int func_row[Func_count];
    int func_col[Func_count];
    int expr_row[Expr_count];
    int expr_col[Expr_count];
    int Func_size = Func_set.size();
    int Expr_size = Expr_set.size();
    int func_set_int[Func_size];
    int expr_set_int[Expr_size];
    copy(Func_set.begin(), Func_set.end(), func_set_int);
    copy(Expr_set.begin(), Expr_set.end(), expr_set_int);
    
    int k = 0;
    for (int i = 0; i < num; i++)
    {
        int col = where_vec[i][0] - 'A';
        int row = atoi(where_vec[i].c_str() + 1) - 1;
        if (!data_table[row][col].empty() && get_cell_type(row, col).name() != typeid(EmptyCell).name() && get_cell_type(row, col).name() != typeid(DateCell).name() && get_cell_type(row, col).name() != typeid(StringCell).name()) 
        {
            sort_value[k] = data_table[row][col].back()->to_numeric();
            sort_row[k] = row;
            sort_col[k] = col;
            k++;
        }
    }
    
    k = 0;
    for (int i = 0; i < Func_size; i++)
    {
        for (int j = 0; j < num; j++)
        {
            int col = where_vec[j][0] - 'A';
            int row = atoi(where_vec[j].c_str() + 1) - 1;
            if (!data_table[row][col].empty() && get_cell_type(row, col).name() == typeid(FuncCell).name() && func_set_int[i] == data_table[row][col].back()->to_numeric())
            {
                func_value[k] = data_table[row][col].back()->to_numeric();
                func_data[k] =  dynamic_cast<FuncCell *>(data_table[row][col].back())->get_data();
                k++;
            }
        }
    }

    k = 0;
    for (int i = 0; i < Expr_size; i++)
    {
        for (int j = 0; j < num; j++)
        {
            int col = where_vec[j][0] - 'A';
            int row = atoi(where_vec[j].c_str() + 1) - 1;
            if (!data_table[row][col].empty() && get_cell_type(row, col).name() == typeid(ExprCell).name() && expr_set_int[i] == data_table[row][col].back()->to_numeric())
            {
                expr_value[k] = data_table[row][col].back()->to_numeric();
                expr_data[k] =  dynamic_cast<ExprCell *>(data_table[row][col].back())->get_data();
                k++;
            }
        }
    }

    if (!how_vec.empty() && how_vec[0] == "desc") 
    {
        sort(sort_value, sort_value+counting, greater<int>());
        sort(func_value, func_value+Func_count, greater<int>());
        sort(expr_value, expr_value+Expr_count, greater<int>());
        reverse(func_data, func_data + Func_count);
        reverse(expr_data, expr_data + Expr_count);
    }
    else 
    {
        sort(sort_value, sort_value+counting);
    }

    k = 0;
    for (int i = 0; i < Func_count; i++)
    {
        for (int j = k; j < counting; j++)
        {
            if (sort_value[j] == func_value[i])
            {
                func_row[i] = sort_row[j];
                func_col[i] = sort_col[j];
                k = j + 1;
                break;
            }
        }
    }

    k = 0;
    for (int i = 0; i < Expr_count; i++)
    {
        for (int j = k; j < counting; j++)
        {
            if (sort_value[j] == expr_value[i])
            {
                int expr = expr_value[i];
                int inter_count = count(func_value, func_value+Func_count, expr);
                expr_row[i] = sort_row[j + inter_count];
                expr_col[i] = sort_col[j + inter_count];
                k = j + 1;
                break;
            }
        }
    }

    int j = 0;
    k = 0;
    for (int i = 0; i < counting; i++)
    {
        if (sort_row[i] == func_row[j] && sort_col[i] == func_col[j])
        {
            j++;
            continue;
        }
        else if (sort_row[i] == expr_row[k] && sort_col[i] == expr_col[k])
        {
            k++;
            continue;
        }
        reg_cell(new NumberCell(sort_value[i], sort_row[i], sort_col[i], this), sort_row[i], sort_col[i]);
    }
    for (int i = 0; i < Func_count; i++)
    {
        reg_cell(new FuncCell(func_data[i], func_row[i], func_col[i], this), func_row[i], func_col[i]);
    }
    for (int i = 0; i < Expr_count; i++)
    {
        reg_cell(new ExprCell(expr_data[i], expr_row[i], expr_col[i], this), expr_row[i], expr_col[i]);
    }
}

string Table::get_data(const string& s)
{
    int col = s[0] - 'A';
    int row = stoi(s.substr(1))-1;

    if (get_cell_type(row, col).name() == typeid(ExprCell).name())
    {
        return dynamic_cast<ExprCell *>(data_table[row][col].back())->get_data();
    }
    else if (get_cell_type(row, col).name() == typeid(FuncCell).name())
    {
        return dynamic_cast<FuncCell *>(data_table[row][col].back())->get_data();
    }
    return "";
}

double Table::to_numeric(const string &s)
{
    // Cell name
    int col = s[0] - 'A';
    int row = stoi(s.substr(1))-1;

    if (row < max_row_size && col < max_col_size)
    {
        if (!data_table[row][col].empty())
        {
            return data_table[row][col].back()->to_numeric();
        }
    }
    return 0;
}
double Table::to_numeric(int row, int col)
{
    if (row < max_row_size && col < max_col_size && !data_table[row][col].empty())
    {
        return data_table[row][col].back()->to_numeric();
    }
    return 0;
}
string Table::stringify(const string &s)
{
    // Cell name
    int col = s[0] - 'A';
    int row = stoi(s.substr(1))-1;

    if (row < max_row_size && col < max_col_size)
    {
        if (!data_table[row][col].empty())
        {
            return data_table[row][col].back()->stringify();
        }
    }
    return 0;
}
string Table::stringify(int row, int col)
{
    if (row < max_row_size && col < max_col_size && !data_table[row][col].empty())
    {
        return data_table[row][col].back()->stringify();
    }
    return "";
}

int Table::get_row_size()
{
    return row_size;
}

int Table::get_col_size()
{
    return col_size;
}

void Table::print_table(int start_row, int start_col, string look_for)
{
    make_table();
    int row, col;
    getmaxyx(win, row, col);

    istringstream ss(table_str);
    string line;
    for ( int i = 0 ; i < start_row ; i++ )
        getline(ss, line, '\n');
    row_size = count(table_str.begin(), table_str.end(), '\n');
    col_size = (table_str.length()-row_size)/row_size;
    for ( int i = 0 ; i < row-5 ; i++ )
    {
        getline(ss, line, '\n');
        int start_x, start_y;
        getyx(win, start_y, start_x);
        if ( (start_row+i) % 2 == 1 )
        {
            // horizontal line
            int x, y;
            whline(win, ACS_HLINE, col);
            getyx(win, y, x);
            for (int i = 0; i <= max_col_size; i++)
            {
                if ( cross_pos[i] >= start_col && cross_pos[i] <= start_col+col )
                {
                    wmove(win, y, cross_pos[i]-start_col);
                    whline(win, ACS_PLUS, 1);
                }
            }
            wmove(win, start_y+1, 0);
            continue;
        }
        string sub_line = line.substr(start_col, col);
        for ( int j = 0 ; j < sub_line.length() ; j++ )
        {
            if ( sub_line[j] == '|' )
            {
                int x, y;
                wvline(win, ACS_VLINE, 1);
                getyx(win, y, x);
                wmove(win, y, x + 1);
            }
            else
            {
                if ( j > 4 && look_for != "" && sub_line.substr(j, look_for.length()) == look_for )
                {
                    wattron(win, COLOR_PAIR(1));
                    wprintw(win, look_for.c_str());
                    wattroff(win, COLOR_PAIR(1));
                    j += look_for.length()-1;
                    continue;
                }
                waddch(win, sub_line[j]);
            }
        }
        wmove(win, start_y+1, 0);
    }
}

void Table::make_table()
{
    row_size = 0;
    col_size = 0;
    table_str.clear();
    cross_pos.clear();
    
    int *col_max_wide = new int[max_col_size];

    for (int i = 0; i < max_col_size; i++)
    {
        for (int j = 0; j < max_row_size; j++)
        {
            if (!is_empty(j, i))
            {
                if (get_cell_type(j, i).name() == typeid(ExprCell).name())
                {
                    dynamic_cast<ExprCell *>(data_table[j][i].back())->parse_expression();
                }
            }
        }
    }
    
    for (int i = 0; i < max_col_size; i++)
    {
        for (int j = 0; j < max_row_size; j++)
        {
            if (!is_empty(j, i))
            {
                if (get_cell_type(j, i).name() == typeid(FuncCell).name())
                {
                    dynamic_cast<FuncCell *>(data_table[j][i].back())->parse_function();
                }
            }
        }
    }

    // COUNT() error...
    for (int i = 0; i < max_col_size; i++)
    {
        for (int j = 0; j < max_row_size; j++)
        {
            if (!is_empty(j, i))
            {
                if (get_cell_type(j, i).name() == typeid(FuncCell).name())
                {
                    dynamic_cast<FuncCell *>(data_table[j][i].back())->parse_function();
                }
            }
        }
    }


    for (int i = 0; i < max_col_size; i++)
    {
        unsigned int max_wide = 2;
        for (int j = 0; j < max_row_size; j++)
        {
            if (!data_table[j][i].empty() &&
                data_table[j][i].back()->stringify().length() > max_wide)
            {
                max_wide = data_table[j][i].back()->stringify().length();
            }
        }
        col_max_wide[i] = max_wide;
    }

    table_str += "    ";
    int total_wide = 4;
    for (int i = 0; i < max_col_size; i++) {
        if (col_max_wide[i]) {
        int max_len = max(2, col_max_wide[i]);
        table_str += " | " + col_num_to_str(i);
        table_str += repeat_char(max_len - col_num_to_str(i).length(), ' ');

        total_wide += (max_len + 3);
        }
    }

    table_str += " |\n";
    total_wide += 2;

    cross_pos.push_back(5);

    for (int i = 0; i <= max_col_size; i++)
    {
        int max_len = max(2, col_max_wide[i]);
        cross_pos.push_back(max_len+3+cross_pos[i]);
    }

    for (int i = 0; i < max_row_size; i++) {
        table_str += repeat_char(total_wide, '-');
        table_str += "\n" + to_string(i + 1);
        table_str += repeat_char(4 - to_string(i + 1).length(), ' ');

        for (int j = 0; j < max_col_size; j++) {
        if (col_max_wide[j]) {
            int max_len = max(2, col_max_wide[j]);

            string s = "";
            if (!data_table[i][j].empty()) {
            s = data_table[i][j].back()->stringify();
            }
            table_str += " | " + s;
            table_str += repeat_char(max_len - s.length(), ' ');
        }
        }
        table_str += " |\n";
    }
    table_str += repeat_char(total_wide, '-');
    table_str += " |\n";
}

string Table::repeat_char(int n, char c)
{
  string s = "";
  for (int i = 0; i < n; i++) s.push_back(c);

  return s;
}

// change number to column name ( 0->A, 1->B, 2->C, ... , 25->Z )
string Table::col_num_to_str(int n)
{
    string s = "";
    if (n < 26)
    {
        s.push_back('A' + n);
    }
    else
    {
        char first = 'A' + n / 26 - 1;
        char second = 'A' + n % 26;

        s.push_back(first);
        s.push_back(second);
    }

    return s;
}

bool Table::is_empty(int row, int col)
{
    if (data_table[row][col].empty() || get_cell_type(row, col).name() == typeid(EmptyCell).name())
        return true;
    return false;
}

bool Table::is_empty(const string& s)
{
    int col = s[0] - 'A';
    int row = stoi(s.substr(1))-1;
    if (data_table[row][col].empty() || get_cell_type(row, col).name() == typeid(EmptyCell).name())
        return true;
    return false;
}

const type_info &Table::get_cell_type(int row, int col)
{
    return typeid(*(data_table[row][col].back()));
}

const type_info &Table::get_cell_type(const string& s)
{
    int col = s[0] - 'A';
    int row = stoi(s.substr(1))-1;
    return typeid(*(data_table[row][col].back()));
}

void Table::to_txt(ofstream& writeFile)
{
    map<string, string> m;
    m.insert(pair<string, string>(string(typeid(NumberCell).name()), "NUMBER"));
    m.insert(pair<string, string>(string(typeid(StringCell).name()), "STRING"));
    m.insert(pair<string, string>(string(typeid(DateCell).name()), "DATE"));
    m.insert(pair<string, string>(string(typeid(ExprCell).name()), "EXPRESSION"));
    m.insert(pair<string, string>(string(typeid(FuncCell).name()), "FUNCTION"));

    writeFile << number_of_cell << endl;
    for (int i = 0; i < max_col_size; i++)
    {
        for (int j = 0; j < max_row_size; j++)
        {
            if (!data_table[j][i].empty())
            {
                if (get_cell_type(j, i).name() == typeid(EmptyCell).name())
                {
                    continue;
                }
                writeFile << col_num_to_str(i) << j + 1 << " " << m[get_cell_type(j, i).name()] << " ";
                if (get_cell_type(j, i).name() == typeid(ExprCell).name())
                {
                    writeFile << dynamic_cast<ExprCell *>(data_table[j][i].back())->get_data() << endl;
                }
                else if (get_cell_type(j, i).name() == typeid(FuncCell).name())
                {
                    writeFile << dynamic_cast<FuncCell *>(data_table[j][i].back())->get_data() << endl;
                }
                else
                {
                    writeFile << data_table[j][i].back()->stringify() << endl;
                }
            }
        }
    }
}

Cell * Table::undo(const string &s)
{
    // Cell name
    int col = s[0] - 'A';
    int row = stoi(s.substr(1))-1;

    if ( !data_table[row][col].empty() ){
        Cell * temp = data_table[row][col].back();
        data_table[row][col].pop_back();
        if ( typeid(*temp).name() == typeid(EmptyCell).name() )
            number_of_cell++;
        else if ( data_table[row][col].empty() )
            number_of_cell--;
        else if ( get_cell_type(row, col).name() == typeid(EmptyCell).name() )
            number_of_cell--;
        return temp;
    }
    else{
        return NULL;
    }
    
}