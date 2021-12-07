# CLI-Excel

## Introduction
> Excel program running in CLI environment

## Member
* PM
> 이재희

* Architect
> 서명원 강다빈

* Developer
> 이재희 전주형 서명원 강다빈 오수현

* Operator
> 전주형 오수현

## How to use

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

# remove cell
>> remove A1

# move to next sheet
>> next

# move to n-th sheet
>> goto 5

# move to prev sheet
>> prev

# delete current sheet
>> delete

# save to txt
>> save filename.txt

# find value
>> find 10

# exit (return to initial page)
>> exit
```

## History
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
        - FuncCell (2021/11/17) (2021/12/05)
            - This Cell stores string data (Excel Function) such as "SUM(A1:A4)"
            - parsing function
            - Kinds : (SUM, AVG, PRODUCT, COUNT, MIN, MAX, RAND, RANDBETWEEN), (SIN, COS, TAN)
        - NumberCell (Int to Double) (2021/12/02)
        - EmptyCell (2021/12/04)
            - This Cell was created to implement the 'remove' command
    - Table
        - We changed the return type from string to void in the print_table() function and changed it to print to a specific WINDOW* using the ncurses function (2021/11/17)
        - Each time the print_table() function is called, the parser are called only once (reducing the amount of unnecessary computation) using RTTI pattern (2021/11/19)
        - If you move the table with the arrow keys, the moved table is displayed (2021/12/01)
        - The table has been changed to a two-dimensional array with a cell list to implement the Undo function (2021/12/03)
    - InitialPage (2021/11/10)
        - When the program starts, it displays the following user menu
            - Create New Excel
            - Open Excel (2021/11/23) (2021/12/03)
                - If you enter a file name, an Excel list in which data is stored is created
                - We changed the way to open by entering a file name so that you can select and open the desired file through File Explorer
            - Manual (Not yet)
            - Exit
    - FileManager (2021/12/03)
        - We got directory data using popen(), stat(), etc., and visualized the directory structure
    - Excel
        - Save (2021/11/19)
            - When input is received in the format of "SAVE filename" or "SAVE", the to_txt() function is called in the order of ExcelList→Excel→Table to save the data to the file
        - Find (2021/11/20)
            - You can find what you want
            - Fixed Find command mechanism related to moving table with arrow keys (2021/12/01)
        - Set value based on range (2021/11/26)
            - Ex : setn A1:A5 1 2 3 4 5
        - In order to recognize the arrow keys, the method has been changed from receiving input line by line to receiving input one character at a time (2021/12/01)
        - Remove (2021/12/04)
            - Delete the data of the existing cell
            - Considering the Undo function and the table structure, create and assign EmptyCells that contain no data, and implement EmptyCells so that they are not allocated consecutively
            - Ex : Remove A1
        - Sort (2021/12/07)
            - You can sort in ascending or descending order by entering the range of cells you want to sort
            - Ex : Sort A1:A4 desc
    - ExcelList
        - Sheet (2021/11/11)
            - You can create, move, and delete sheets with the next, prev, and delete commands
        - Goto (2021/11/25)
            - You can go to nth-sheet
            - goto 5
    - main
        - We implemented so that Excel functions can be used until the user selects exit in InitialPage
        - Signal (2021/12/05)
            - SIGINT, SIGKILL, SIGQUIT, SIGSTOP are ignored
        - Undo (2021/12/05)
            - If you press Ctrl+Z, Excel returns to its previous state
        - Redo (2021/12/05)
            - If you press Ctrl+C and there is a record of 'Undo' before, it goes back to that state
        - Auto Save (2021/12/06)
            - Excel files are automatically saved every 30 seconds (thread)
    - Error Handling
        - Add Error checking about (string, number, date) cell name, value(number, date), ... in command (2021/11/30)
        - Add Error checking about formula of expression cell, function cell (2021/12/01)

## Reference
- Meeting minutes : https://sunset-athlete-d53.notion.site/2021-2-839c0b4ec9c84092aa4fd4ac420f47d7
