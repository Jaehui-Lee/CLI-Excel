#include "ncurses.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include <unistd.h>
#include <stdlib.h>

using namespace std;

/*------------------
       Manual
-------------------*/

class Manual
{
    WINDOW* win;
    vector<string> content;

public:
    Manual(WINDOW* _win);

    ~Manual();

    void init_screen();
    void print_file(int row, int start);
};