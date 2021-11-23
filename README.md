# CLI-Excel

* How to use

```bash
$ sudo apt-get install libncurses5-dev libncursesw5-dev
$ make main
$ ./main
```

* In excution, you can use following command
```bash
# In InitialPage, you can choose 4 menus

 - Create New Excel

 - Open Excel

 - Manual

 - Exit
```
```bash
# In Excel

# set number
>> setn A1 10

# set string
>> sets A1 abc

# set date
>> setd A1 2021-11-11

# set expression (Arithmetic operation)
>> sete A5 A1+A2*A3/A4

# set function (Excel Function)
>> setf A5 SUM(A1:A4)

# move to next sheet
>> next

# move to prev sheet
>> prev

# delete current sheet
>> delete

# save to txt
>> save filename.txt

# exit (return to initial page)
>> exit
```

- Base Source
    - Cell
        - NumberCell
            - This Cell stores number data
        - StringCell
            - This Cell stores string data
        - DateCell
            - This Cell stores date data (time_t)
        - ExprCell
            - This Cell stores string data (Arithmetic command) such as "A1+A2+A3"
            - parsing expression (user-command)
        - You can use to_numeric(), stringify() function to transform data from original to number or string
    - Table
        - Table has 2-dimensional array of Cell*
        - You can print the table to the screen using print_table() function
        - print_table() uses just character '-', '|' as table contour not using "ncurses.h"
    - Excel
        - You can print the screen including Table and command line
        - You can use user-command such as "setn A1 10"
        - parse_user_input() function analyzes user-command and create appropriate Cell object
    - There is code but no code-splitting

- What we made
    - Code-splitting by class
    - Cell
        - Add FuncCell (2021/11/17)
            - This Cell stores string data (Excel Function) such as "SUM(A1:A4)"
            - parsing function
            - Kinds : SUM, AVG, PRODUCT, COUNT, MIN, MAX, RAND, RANDBETWEEN
    - Table
        - We changed the return type from string to void in the print_table() function and changed it to print to a specific WINDOW* using the ncurses function (2021/11/17)
        - Each time the print_table() function is called, the parser are called only once (reducing the amount of unnecessary computation) using RTTI pattern (2021/11/19)
    - InitialPage (2021/11/10)
        - When the program starts, it displays the following user menu
            - Create New Excel
            - Open Excel (2021/11/23)
                - If you enter a file name, an Excel list in which data is stored is created
            - Manual (Not yet)
            - Exit
    - Excel
        - Add Save (2021/11/19)
            - When input is received in the format of "SAVE filename", the to_txt() function is called in the order of ExcelList→Excel→Table to save the data to the file
    - ExcelList
        - Add Sheet (2021/11/11)
            - You can create, move, and delete sheets with the next, prev, and delete commands
    - main
        - When starting the program, use fork() and execlp() to set the terminal window size to 120 width and 40 height
        - We implemented so that Excel functions can be used until the user selects exit in InitialPage
