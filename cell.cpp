#include "cell.h"

/*------------------
        Cell
-------------------*/

Cell::Cell(int x, int y, Table *table) : x(x), y(y), table(table) {}

/*------------------
     StringCell
-------------------*/

StringCell::StringCell(string data, int x, int y, Table *t)
    : data(data), Cell(x, y, t) {}
string StringCell::stringify() { return data; }
double StringCell::to_numeric() { return 0; }

/*------------------
     NumberCell
-------------------*/

NumberCell::NumberCell(double data, int x, int y, Table *t)
    : data(data), Cell(x, y, t) {}

string NumberCell::stringify()
{
    string str = to_string(data);
    for ( int i = str.length()-1 ; i > 0 ; i-- )
    {
        if ( str[i] == '0' )
        {
            str.pop_back();
        }
        else if  ( str[i] == '.' )
        {
            str.pop_back();
            break;
        }
        else
            break;
    }
    return str;
}
double NumberCell::to_numeric() { return data; }

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

double DateCell::to_numeric()
{
    return 0;
}

DateCell::DateCell(string s, int x, int y, Table *t) : Cell(x, y, t)
{
    // Date format : yyyy-mm-dd (Ex. 2021-01-01)
    int year = stoi(s);
    int month = stoi(s.substr(5));
    int day = stoi(s.substr(8));

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

ExprCell::ExprCell(string data, int x, int y, Table *t)
    : data(data), Cell(x, y, t) {}

string ExprCell::get_data()
{
    return data;
}

string ExprCell::stringify()
{
    string str = to_string(to_numeric());
    for ( int i = str.length()-1 ; i > 0 ; i-- )
    {
        if ( str[i] == '0' )
        {
            str.pop_back();
        }
        else if  ( str[i] == '.' )
        {
            str.pop_back();
            break;
        }
        else
            break;
    }
    return str;
}

double ExprCell::to_numeric()
{
    double result = 0;
    stack<double> st;

    for (int i = 0; i < exp_vec.size(); i++)
    {
        string s = exp_vec[i];

        // if Cell
        if (isalpha(s[0]))
        {
            st.push(table->to_numeric(s));
        }
        // if Number
        else if (isdigit(s[0]))
        {
            st.push(stod(s));
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
    exp_vec.clear();
    stack<string> st;

    string data_exp = data;

    // expression is surrounded by Parenthesis
    // Ex. A1+A2+A3 -> (A1+A2+A3)
    data_exp.insert(0, "(");
    data_exp.push_back(')');

    for (int i = 0; i < data_exp.length(); i++)
    {
        if (isalpha(data_exp[i]))
        {
            int count = 0;
            while(isalnum(data_exp[i+count]) || data_exp[i+count] == '.')
                count++;
            exp_vec.push_back(data_exp.substr(i, count));
            i += count-1;
        }
        else if (isdigit(data_exp[i]))
        {
            int count = 0;
            while(isdigit(data_exp[i+count]) || data_exp[i+count] == '.')
                count++;
            exp_vec.push_back(data_exp.substr(i, count));
            i += count-1;
        }
        else if (data_exp[i] == '(' || data_exp[i] == '[' || data_exp[i] == '{')
        { // Parenthesis
            st.push(data_exp.substr(i, 1));
        }
        else if (data_exp[i] == ')' || data_exp[i] == ']' || data_exp[i] == '}')
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
        else if (data_exp[i] == '+' || data_exp[i] == '-' || data_exp[i] == '*' || data_exp[i] == '/')
        {
            while (!st.empty() && precedence(st.top()[0]) >= precedence(data_exp[i]))
            {
                exp_vec.push_back(st.top());
                st.pop();
            }
            st.push(data_exp.substr(i, 1));
        }
    }
}

/*------------------
      FuncCell
-------------------*/

FuncCell::FuncCell(string data, int x, int y, Table *t)
    : data(data), value(0), valid(false), Cell(x, y, t) {}

string FuncCell::get_data()
{
    return data;
}

string FuncCell::stringify()
{
    string str = to_string(to_numeric());
    for ( int i = str.length()-1 ; i > 0 ; i-- )
    {
        if ( str[i] == '0' )
        {
            str.pop_back();
        }
        else if  ( str[i] == '.' )
        {
            str.pop_back();
            break;
        }
        else
            break;
    }
    return str;
}

double FuncCell::to_numeric()
{
    return value;
}

void FuncCell::parse_function()
{
    func_vec.clear();
    int i, j;

    for (i = 0; i < data.length(); i++)
    {
        if (data[i] == '(')
        {
            func_vec.push_back(data.substr(0, i));
            transform(func_vec[0].begin(), func_vec[0].end(), func_vec[0].begin(), ::toupper);
            break;
        }
    }

    // SUM, AVG, PRODUCT, MAX, MIN
    if (func_vec.back() == "SUM" || func_vec.back() == "AVG" || func_vec.back() == "PRODUCT" || func_vec.back() == "MAX" || func_vec.back() == "MIN")
    {
        istringstream ss(data.substr(i+1, data.length()-i-2));
        string start, end;
        getline(ss, start, ':');
        getline(ss, end);
        for ( char p = start[0] ; p <= end[0] ; p++ )
        {
            for ( int q = stoi(start.substr(1)) ; q <= stoi(end.substr(1)) ; q++ )
            {
                string cell_name(1,p);
                cell_name += to_string(q);
                func_vec.push_back(cell_name);
            }
        }
    }
    // COUNT
    else if (func_vec.back() == "COUNT")
    {
        func_vec.push_back(data.substr(i + 1, data.length() - i - 2));
    }
    // RAND ( 0 ~ 100 )
    else if (func_vec.back() == "RAND")
    {
        // Nothing to do
    }
    // RANDBETWEEN
    else if (func_vec.back() == "RANDBETWEEN")
    {
        for (j = i + 1; j < data.length(); j++)
        {
            if (data[j] == ',')
            {
                func_vec.push_back(data.substr(i + 1, j - i - 1));
                func_vec.push_back(data.substr(j + 1, data.length() - j - 2));
            }
        }
    }

    calculate();
}

void FuncCell::calculate()
{
    if (func_vec[0] == "SUM" || func_vec[0] == "AVG")
    {
        value = 0.0;
        for (int i = 1; i < func_vec.size(); i++)
        {
            value += table->to_numeric(func_vec[i]);
        }
        if (func_vec[0] == "AVG")
            value /= func_vec.size() - 1;
    }
    else if (func_vec[0] == "PRODUCT")
    {
        value = 1;
        for (int i = 1; i < func_vec.size(); i++)
        {
            value *= table->to_numeric(func_vec[i]);
        }
    }
    else if (func_vec[0] == "COUNT")
    {
        value = 0;

        for (int i = 0; i < MAX_ROW_SIZE; i++)
        {
            for (int j = 0; j < MAX_COL_SIZE; j++)
            {
                double f_value = stod(func_vec[1]); // value to find
                // RTTI
                if (!table->is_empty(i, j) && table->to_numeric(i, j) == f_value)
                {
                    value++;
                }
            }
        }
    }
    else if (func_vec[0] == "MAX" || func_vec[0] == "MIN")
    {
        value = table->to_numeric(func_vec[1]);

        for (int i = 2; i < func_vec.size(); i++)
        {
            double temp = table->to_numeric(func_vec[i]);
            if (func_vec[0] == "MAX")
                value = (value < temp) ? temp : value;
            else if (func_vec[0] == "MIN")
                value = (value > temp) ? temp : value;
        }
    }
    else if (func_vec[0] == "RAND" || func_vec[0] == "RANDBETWEEN")
    {
        if (!valid)
        {
            random_device rd;
            mt19937_64 gen(rd());

            uniform_real_distribution<double> dis(0.0, 100.0);
            if (func_vec[0] == "RANDBETWEEN")
            {
                double start = stod(func_vec[1]);
                double end = stod(func_vec[2]);
                dis = uniform_real_distribution<double>(start, end);
            }

            value = dis(gen);
            valid = true;
        }
    }
}