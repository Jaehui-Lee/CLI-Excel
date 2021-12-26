#include "filemanager.h"

/*------------------
    FileManager
-------------------*/

FileManager::FileManager(WINDOW* _win, string start_dir) : win(_win), start_dir(start_dir), n_choices(0), choices(vector<string>(0)) { }

FileManager::~FileManager() {}

bool FileManager::is_dir(string s)
{
    struct stat info;
    struct stat current_dir_info;

    stat(s.c_str(), &info);
    stat(".", &current_dir_info);

    if ( S_ISDIR(info.st_mode) )
    {
        if (info.st_ino != current_dir_info.st_ino)
        {
            chdir(s.c_str());
            make_file_list();
        }
        return true;
    }
    return false;
}

string FileManager::init_screen()
{
    chdir(start_dir.c_str());

    make_file_list();

    int startx = 0;
    int starty = 0;

    WINDOW *file_win;
    int highlight = 0;
    int choice = 0;
    int c;
    int row, col;

    int start_choice = 0;

    wclear(win);
    noecho();
    cbreak(); // Line buffering disabled. pass on everything
    curs_set(0);

    getmaxyx(win, row, col);
    startx = (col - WIDTH) / 2;
    starty = (row - HEIGHT) / 2;

    file_win = newwin(HEIGHT, WIDTH, starty, startx);
    keypad(file_win, TRUE);
    mvwprintw(win, 0, 0, "Use arrow keys to go up and down, Press enter to select a file");
    mvwprintw(win, 1, 0, "To return to the initial screen, press 'q' or 'Q'");
    wrefresh(win);
    print_file(file_win, highlight, start_choice);
    while (1)
    {
        c = wgetch(file_win);
        switch (c)
        {
        case KEY_UP:
            if (highlight != 0)
                --highlight;
            if ( start_choice > highlight )
                --start_choice;
            break;
        case KEY_DOWN:
            if (highlight != n_choices-1)
                ++highlight;
            if ( start_choice+14 < highlight )
                ++start_choice;
            break;
        case 10:
            choice = highlight;
            if ( is_dir(choices[choice]) )
            {
                start_choice = 0;
                highlight = 0;
            }
            else
            {
                echo();
                raw();
                curs_set(1);
                return choices[choice];
            }
            break;
        case 'q':
            return "q";
        case 'Q':
            return "q";
        default:
            break;
        }
        print_file(file_win, highlight, start_choice);
    }
    clrtoeol();
    echo();
    raw();
    curs_set(1);

    return "";
}

inline void FileManager::rtrim(string &s)
{
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !isspace(ch);
    }).base(), s.end());
}

void FileManager::make_file_list()
{
    choices.clear();

    choices.push_back(string("."));
    choices.push_back(string(".."));

    char str[30];

    FILE *fpipe;
    char command[3] = "ls";

    fpipe = (FILE*)popen(command, "r");
    while (fgets(str, sizeof(str), fpipe))
    {
        string f_name = string(str);
        rtrim(f_name);
        choices.push_back(f_name);
    }

    pclose(fpipe);

    n_choices = choices.size();
}



void FileManager::print_file(WINDOW *file_win, int highlight, int  start_choice)
{
    wclear(file_win);
    int x, y, i;

    x = 2;
    y = 2;
    box(file_win, 0, 0);
    for (i = start_choice; i < (start_choice+15) && i < n_choices ; ++i)
    {
        if (highlight == i) /* High light the present choice */
        {
            wattron(file_win, A_REVERSE);
            mvwprintw(file_win, y, x, "%s", choices[i].c_str());
            wattroff(file_win, A_REVERSE);
        }
        else
        {
            mvwprintw(file_win, y, x, "%s", choices[i].c_str());
        }
        y += 1;
    }
    wrefresh(file_win);
}