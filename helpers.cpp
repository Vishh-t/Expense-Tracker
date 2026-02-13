#include "helpers.h"
#include <iostream>
#include <limits>
#include <cctype>
#include <conio.h>
#include <vector>

using namespace std;

const WORD CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const WORD RED = FOREGROUND_RED | FOREGROUND_INTENSITY;
const WORD GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const WORD WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const WORD YELLOW =  FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;

void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

WORD save_color()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.wAttributes;
}

void do_ignore()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool get_int(int &x)
{
    cin >> x;
    if (!cin)
    {
        cin.clear();
        do_ignore();
        return false;
    }
    return true;
}

string get_string()
{
    string s;
    getline(cin, s);
    return s;
}

string convert_lower(string s)
{
    for (auto &c : s)
    {
        c = tolower(c);
    }
    return s;
}

bool verify_input_key(string key, string input)
{
    return key == input;
}

string get_hidden_password()
{
    string pass = "";
    char ch;

    while ((ch = _getch()) != '\r')
    {
        if (ch == '\b')
        {
            if (pass.length() != 0)
            {
                pass.pop_back();
                cout << "\b \b";
            }
        }
        else
        {
            pass.push_back(ch);
            cout << "*";
        }
    }

    cout << endl;
    return pass;
}

bool isleapyear(int year)
{
    if (year % 100 == 0 && year % 400 == 0)
    {
        return true;
    }
    else if (year % 100 != 0 && year % 4 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool DateValidator(string date)
{
    // FORMAT = DD/MM/YYYY
    if (date.length() != 10)
    {
        return false;
    }

    if (date[2] != '/' || date[5] != '/')
    {
        return false;
    }

    for (size_t i = 0; i < date.length(); i++)
    {
        if (i == 2 || i == 5)
        {
            continue;
        }

        if (!isdigit(date[i]))
        {
            return false;
        }
    }

    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));

    if (year < 1900 || year > 2100)
    {
        return false;
    }

    if (month < 1 || month > 12)
    {
        return false;
    }

    vector<int> number_of_days = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (isleapyear(year))
    {
        number_of_days[1] = 29;
    }

    if (day <= 0 || day > number_of_days[month - 1])
    {
        return false;
    }
    return true;
}