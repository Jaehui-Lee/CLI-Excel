#include "txttable.h"
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
    Excel(WINDOW* _win, int max_row, int max_col, int choice);

    ~Excel();

    int parse_user_input(string s);
    int command_line();
    void print_table();
};