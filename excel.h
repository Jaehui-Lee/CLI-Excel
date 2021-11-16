#include "table.h"
#include "ncurses.h"

using namespace std;

/*------------------
       Excel
-------------------*/

class Excel
{
    Table *current_table;
    WINDOW *win;

public:
    Excel(WINDOW* _win, int max_row, int max_col);

    ~Excel();

    int parse_user_input(string s);
    int command_line(int current_page, int excel_count);
    void print_table(int current_page, int excel_count);
};