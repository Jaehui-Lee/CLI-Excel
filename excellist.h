#include "excel.h"

#include <list>

#define MAX_ROW_SIZE 18
#define MAX_COL_SIZE 18

#define WIN_ROW_SIZE 40
#define WIN_COL_SIZE 120

using namespace std;

class ExcelList
{
private:
    list<Excel *> excelList;
    int excel_count;   // from 1
    int current_excel; // from 0

public:
    ExcelList();

    Excel *get_current_excel();
    void move_next_window();
    void move_prev_window();
    void insert_window();
    void delete_window();
};