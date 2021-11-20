#include "table.h"

/*------------------
        Table
-------------------*/

Table::Table(WINDOW *win, int max_row_size, int max_col_size)
    : win(win), max_row_size(max_row_size), max_col_size(max_col_size), number_of_cell(0)
{
    data_table = new Cell **[max_row_size];
    for (int i = 0; i < max_row_size; i++)
    {
        data_table[i] = new Cell *[max_col_size];
        for (int j = 0; j < max_col_size; j++)
        {
            data_table[i][j] = NULL;
        }
    }
}

Table::~Table()
{
    for (int i = 0; i < max_row_size; i++)
    {
        for (int j = 0; j < max_col_size; j++)
        {
            if (data_table[i][j])
                delete data_table[i][j];
        }
    }
    for (int i = 0; i < max_row_size; i++)
    {
        delete[] data_table[i];
    }
    delete[] data_table;
}

void Table::reg_cell(Cell *c, int row, int col)
{
    if (!(row < max_row_size && col < max_col_size))
        return;

    if (data_table[row][col])
    {
        number_of_cell--;
        delete data_table[row][col];
    }
    data_table[row][col] = c;
    number_of_cell++;
}

int Table::to_numeric(const string &s)
{
    // Cell name
    int col = s[0] - 'A';
    int row = atoi(s.c_str() + 1) - 1;

    if (row < max_row_size && col < max_col_size)
    {
        if (data_table[row][col])
        {
            return data_table[row][col]->to_numeric();
        }
    }
    return 0;
}
int Table::to_numeric(int row, int col)
{
    if (row < max_row_size && col < max_col_size && data_table[row][col])
    {
        return data_table[row][col]->to_numeric();
    }
    return 0;
}
string Table::stringify(const string &s)
{
    // Cell name
    int col = s[0] - 'A';
    int row = atoi(s.c_str() + 1) - 1;

    if (row < max_row_size && col < max_col_size)
    {
        if (data_table[row][col])
        {
            return data_table[row][col]->stringify();
        }
    }
    return 0;
}
string Table::stringify(int row, int col)
{
    if (row < max_row_size && col < max_col_size && data_table[row][col])
    {
        return data_table[row][col]->stringify();
    }
    return "";
}

void Table::print_table(string s)
{
    int *col_max_wide = new int[max_col_size];

    for (int i = 0; i < max_col_size; i++)
    {
        for (int j = 0; j < max_row_size; j++)
        {
            if (data_table[j][i])
            {
                if (get_cell_type(j, i).name() == typeid(ExprCell).name())
                {
                    dynamic_cast<ExprCell *>(data_table[j][i])->parse_expression();
                }
                else if (get_cell_type(j, i).name() == typeid(FuncCell).name())
                {
                    dynamic_cast<FuncCell *>(data_table[j][i])->parse_function();
                }
            }
        }
    }

    for (int i = 0; i < max_col_size; i++)
    {
        unsigned int max_wide = 2;
        for (int j = 0; j < max_row_size; j++)
        {
            if (data_table[j][i] &&
                data_table[j][i]->stringify().length() > max_wide)
            {
                max_wide = data_table[j][i]->stringify().length();
            }
        }
        col_max_wide[i] = max_wide;
    }

    wprintw(win, "    ");
    int total_wide = 4;
    for (int i = 0; i < max_col_size; i++)
    {
        if (col_max_wide[i])
        {
            int x, y;
            int max_len = max(2, col_max_wide[i]);
            wprintw(win, " ");
            wvline(win, ACS_VLINE, 1);
            getyx(win, y, x);
            wmove(win, y, x + 1);
            wprintw(win, " ");
            wprintw(win, col_num_to_str(i).c_str());
            repeat_char(max_len - col_num_to_str(i).length(), " ");

            total_wide += (max_len + 3);
        }
    }

    wprintw(win, "\n");

    int cross_pos[max_col_size];
    cross_pos[0] = 5;

    for (int i = 0; i <= max_col_size; i++)
    {
        int max_len = max(2, col_max_wide[i]);
        cross_pos[i + 1] = max_len + 3 + cross_pos[i];
    }

    for (int i = 0; i < max_row_size; i++)
    {
        // horizontal line
        int x, y;
        whline(win, ACS_HLINE, total_wide);
        getyx(win, y, x);
        for (int i = 0; i <= max_col_size; i++)
        {
            wmove(win, y, cross_pos[i]);
            whline(win, ACS_PLUS, 1);
        }
        wprintw(win, "\n");

        // data line
        wprintw(win, to_string(i + 1).c_str());
        repeat_char(4 - to_string(i + 1).length(), " ");

        for (int j = 0; j < max_col_size; j++)
        {
            if (col_max_wide[j])
            {
                int max_len = max(2, col_max_wide[j]);
                int x, y;

                string s = "";
                if (data_table[i][j])
                {
                    s = data_table[i][j]->stringify();
                }
                wprintw(win, " ");
                wvline(win, ACS_VLINE, 1);
                getyx(win, y, x);
                wmove(win, y, x + 1);
                wprintw(win, " ");
                wprintw(win, s.c_str());
                repeat_char(max_len - s.length(), " ");
            }
        }
        wprintw(win, "\n");
    }

    if (!(s == ""))
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
        string to = "";
        for (int i = next; i < s.length(); i++)
        {
            if (s[i] == ' ' || i == s.length() - 1)
            {
                to = s.substr(next, i - next + 1);
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
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
        attron(COLOR_PAIR(1));
        for(int i = 0; i < max_col_size; i++)
        {
            for(int j = 0; j < max_row_size; j++)
            {
                if(data_table[j][i])
                {
                    string s = "";
                    s = data_table[j][i]->stringify();
                    if (to == s)
                    {
                        int x = cross_pos[i] + 2;
                        int y = 2 * j + 2;
                        int len = s.length();
                        for(int k = 0; k < len; k++)
                        {
                            wmove(win, y+1, x+k);
                            waddch(win, ACS_HLINE | COLOR_PAIR(1));                           
                        }
                    }
                }
            }
        }
        attroff(COLOR_PAIR(1));
    }
}

void Table::repeat_char(int n, const char *c)
{
    for (int i = 0; i < n; i++)
        wprintw(win, c);
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
    if (!data_table[row][col])
        return true;
    return false;
}

const type_info &Table::get_cell_type(int row, int col)
{
    return typeid(*data_table[row][col]);
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
            if (data_table[j][i])
            {
                writeFile << col_num_to_str(i) << j + 1 << " " << m[get_cell_type(j, i).name()] << " ";
                if (get_cell_type(j, i).name() == typeid(ExprCell).name())
                {
                    writeFile << dynamic_cast<ExprCell *>(data_table[j][i])->get_data() << endl;
                }
                else if (get_cell_type(j, i).name() == typeid(FuncCell).name())
                {
                    writeFile << dynamic_cast<FuncCell *>(data_table[j][i])->get_data() << endl;
                }
                else
                {
                    writeFile << data_table[j][i]->stringify() << endl;
                }
            }
        }
    }
}