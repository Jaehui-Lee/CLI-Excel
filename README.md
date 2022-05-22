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

## Architecture
![Group 208](https://user-images.githubusercontent.com/71120389/169697061-7bdfc8e0-f1ee-43c4-89c0-26bc8ee50c94.svg)

<br />

![Group 210](https://user-images.githubusercontent.com/71120389/169697094-f275c691-7be1-4927-b5b5-361ea90abff6.svg)

<br />

## How to use

### Download and Run
```bash
$ git clone https://github.com/Jaehui-Lee/CLI-Excel.git
# ubuntu
$ sudo apt-get install libncurses5-dev libncursesw5-dev
# centos
$ sudo yum -y install ncurses-devel
$ make all
$ make clean
$ ./main
```

### If you run the program, you can choose from 4 menus

<br />

![Group 164](https://user-images.githubusercontent.com/71120389/147382539-7febd4f1-6da0-4cf3-9871-5f48e4b5b156.jpg)

<br />

- Create New Excel

![image 48](https://user-images.githubusercontent.com/71120389/147382565-3b8cc438-6e0b-4655-9433-9de4a46c9c17.jpg)

You can create Excel to perform a variety of tasks

<br />
<br />

- Open Excel

![Group 165](https://user-images.githubusercontent.com/71120389/147382574-db092c40-00f0-40b9-b43a-82d49404dd92.jpg)

Through the file manager, you can open the previously saved Excel file and continue working

<br />
<br />

- Manual

![image 68](https://user-images.githubusercontent.com/71120389/147397913-945e0faa-9486-42dd-ae58-e23ab650750b.jpg)

Through the manual, you can get help on how to use it

<br />
<br />

- Exit

End the program

<br />
<br />

### Commands

- Set Value

![Group 199](https://user-images.githubusercontent.com/71120389/147381977-5b15b022-cc68-4451-9955-799876c7e344.jpg)

You can set 5 types of values (numbers, strings, dates, formulas including arithmetic operations, and Excel functions), and you can set multiple values as well as single values.

> Setn A4:A13 10 20 30 40 50 60 70 80 90 100

> Setd C1 2021-12-13

> Sets F1 CLI-Excel

> Sete G12 (A4+A5+A6)/A7*A8

> Setf D4 SUM(A4:A13)

<br />
<br />

- Remove Cell

![Group 200](https://user-images.githubusercontent.com/71120389/147382083-e342aa5f-8dc8-4a39-b62e-0671c0844b0e.jpg)

You can remove the value of a specified cell, and you can remove multiple values as well as single values.

> Remove A4

<br />
<br />

- Get Cell Information

![Group 201](https://user-images.githubusercontent.com/71120389/147382213-980dd095-9dfe-4e28-91e7-3e2325b3777f.jpg)

you can get the actual command value stored in the cell, and you can get multiple values as well as single values.

> Get D4:D9

<br />
<br />

- Sheet

![Group 202](https://user-images.githubusercontent.com/71120389/147382303-77317a08-703a-4fea-a471-71a2d158b94b.jpg)

You can move to next, prev, n-th sheet, and also delete sheet.

> Next

> Prev

> Delete

> Goto 5

<br />
<br />

- Save

You can save the excel file you created as a txt file. Saved files can also be opened and written continuously. When creating a new Excel file for the first time, if you enter a file name, it is automatically saved every 30 seconds while working.

> Save

> Save filename.txt

<br />
<br />

- Find

![Group 204](https://user-images.githubusercontent.com/71120389/147382410-bb5c0c8b-efa7-4848-b232-d694df9cae72.jpg)

If you enter the data you are looking for, the stored values are searched based on text and highlighted.

> Find 50

<br />
<br />

- Sort

![Group 203](https://user-images.githubusercontent.com/71120389/147382377-b4159866-c27a-440c-9463-2bdcc87f9708.jpg)

You can sort cells in a specified range in ascending or descending order. The type of data being sorted is limited to Number.

> Sort A4:A13 desc

<br />
<br />

- Undo / Redo

![Group 205](https://user-images.githubusercontent.com/71120389/147382411-d60b56e4-5cb6-411a-bd91-3408b5591a56.jpg)

If you want to undo what you've done, just press Ctrl+Z and Excel will go back to its previous state. However, if you want to go back to the state before the revert after reverting Excel, just press Ctrl+C.

> Ctrl+Z

> Ctrl+C

<br />
<br />

- Table Moving

You can move the Excel table by pressing the arrow keys.

<br />
<br />

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
    - Manual (2021/12/26)
        - It was implemented so that the txt file with the manual was read and printed on the screen, and the contents could be further checked with the arrow keys
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
        - Get (2021/12/10)
            - Ex : Get A1:A5
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
