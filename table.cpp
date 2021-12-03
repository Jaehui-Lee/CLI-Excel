#include "table.h"

/*------------------
        Table
-------------------*/

Table::Table(WINDOW *win, int max_row_size, int max_col_size, string table_str, vector<int> cross_pos)
    : win(win), max_row_size(max_row_size), max_col_size(max_col_size), number_of_cell(0), table_str(table_str), cross_pos(cross_pos)
{
    data_table = vector<vector<list<Cell *>>>(MAX_ROW_SIZE, vector<list<Cell *>>(MAX_COL_SIZE, list<Cell *>(0)));
}

Table::~Table() {}

void Table::reg_cell(Cell *c, int row, int col)
{
    if ( data_table[row][col].empty() )
        number_of_cell++;
    data_table[row][col].push_back(c);
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
            if (!data_table[j][i].empty())
            {
                if (get_cell_type(j, i).name() == typeid(ExprCell).name())
                {
                    dynamic_cast<ExprCell *>(data_table[j][i].back())->parse_expression();
                }
                else if (get_cell_type(j, i).name() == typeid(FuncCell).name())
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
    if (!data_table[row][col].empty())
        return true;
    return false;
}

const type_info &Table::get_cell_type(int row, int col)
{
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