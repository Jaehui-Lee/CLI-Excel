#include "ncurses.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>

using namespace std;

/*------------------
    FileManager
-------------------*/

class FileManager
{
    // choice box size of initial page
    enum
    {
        WIDTH = 60,
        HEIGHT = 20
    };

    // vector<string> choices;
    vector<string> choices;
    int n_choices;
    WINDOW* win;
    string start_dir;

    inline void rtrim(string &s);
    bool is_dir(string s);

public:
    FileManager(WINDOW* _win);

    ~FileManager();

    string init_screen();
    void make_file_list();
    void print_file(WINDOW *file_win, int highlight, int start_choice);

};