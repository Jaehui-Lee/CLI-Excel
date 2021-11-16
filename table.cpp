#include "table.h"

/*------------------
        Table
-------------------*/

Table::Table(WINDOW *win, int max_row_size, int max_col_size)
    : win(win), max_row_size(max_row_size), max_col_size(max_col_size)
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
        delete data_table[row][col];
    }
    data_table[row][col] = c;
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

void Table::print_table()
{
    int *col_max_wide = new int[max_col_size];
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
            repeat_char(max_len-col_num_to_str(i).length(), " ");

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
        repeat_char(4-to_string(i+1).length(), " ");

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
                repeat_char(max_len-s.length(), " ");
            }
        }
        wprintw(win, "\n");
    }
}

void Table::repeat_char(int n, const char* c)
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