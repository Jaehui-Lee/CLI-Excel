#include "cell.h"

/*------------------
        Cell
-------------------*/

Cell::Cell(int x, int y, Table *table, Type type) : x(x), y(y), type(NONE), table(table) {}

/*------------------
     StringCell
-------------------*/

StringCell::StringCell(string data, int x, int y, Table *t, Type type)
    : data(data), Cell(x, y, t, type) {}
string StringCell::stringify() { return data; }
int StringCell::to_numeric() { return 0; }

/*------------------
     NumberCell
-------------------*/

NumberCell::NumberCell(int data, int x, int y, Table *t, Type type)
    : data(data), Cell(x, y, t, type) {}

string NumberCell::stringify() { return to_string(data); }
int NumberCell::to_numeric() { return data; }

/*------------------
      DateCell
-------------------*/

string DateCell::stringify()
{
    char buf[50];

    tm *temp;
    temp = localtime(&data);

    strftime(buf, 50, "%F", temp);

    return string(buf);
}

int DateCell::to_numeric()
{
    return static_cast<int>(data);
}

DateCell::DateCell(string s, int x, int y, Table *t, Type type) : Cell(x, y, t, type)
{
    // Date format : yyyy-mm-dd (Ex. 2021-01-01)
    int year = atoi(s.c_str());
    int month = atoi(s.c_str() + 5);
    int day = atoi(s.c_str() + 8);

    tm timeinfo;

    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = day;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    data = mktime(&timeinfo);
}

/*------------------
      ExprCell
-------------------*/

ExprCell::ExprCell(string data, int x, int y, Table *t, Type type)
    : data(data), Cell(x, y, t, type) {}

string ExprCell::stringify()
{ 
    parse_expression();
    return to_string(to_numeric());
}

int ExprCell::to_numeric()
{
    double result = 0;
    stack<int> st;

    for (int i = 0; i < exp_vec.size(); i++)
    {
        string s = exp_vec[i];

        // if Cell
        if (isalpha(s[0]))
        {
            st.push(table->to_numeric(s));
        }
        // if Number ( 0 ~ 9 )
        else if (isdigit(s[0]))
        {
            st.push(atoi(s.c_str()));
        }
        else
        {
            double y = st.top();
            st.pop();
            double x = st.top();
            st.pop();
            switch (s[0])
            {
            case '+':
                st.push(x + y);
                break;
            case '-':
                st.push(x - y);
                break;
            case '*':
                st.push(x * y);
                break;
            case '/':
                st.push(x / y);
                break;
            }
        }
    }
    return st.top();
}

int ExprCell::precedence(char c)
{
    switch (c)
    {
    case '(':
    case '[':
    case '{':
        return 0;
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    }
    return 0;
}

void ExprCell::parse_expression()
{
    stack<string> st;

    // expression is surrounded by Parenthesis
    // Ex. A1+A2+A3 -> (A1+A2+A3)
    data.insert(0, "(");
    data.push_back(')');

    for (int i = 0; i < data.length(); i++)
    {
        if (isalpha(data[i]))
        {
            exp_vec.push_back(data.substr(i, 2));
            i++;
        }
        else if (isdigit(data[i]))
        {
            exp_vec.push_back(data.substr(i, 1));
        }
        else if (data[i] == '(' || data[i] == '[' || data[i] == '{')
        { // Parenthesis
            st.push(data.substr(i, 1));
        }
        else if (data[i] == ')' || data[i] == ']' || data[i] == '}')
        {
            string t = st.top();
            st.pop();
            while (t != "(" && t != "[" && t != "{")
            {
                exp_vec.push_back(t);
                t = st.top();
                st.pop();
            }
        }
        else if (data[i] == '+' || data[i] == '-' || data[i] == '*' || data[i] == '/')
        {
            while (!st.empty() && precedence(st.top()[0]) >= precedence(data[i]))
            {
                exp_vec.push_back(st.top());
                st.pop();
            }
            st.push(data.substr(i, 1));
        }
    }
}