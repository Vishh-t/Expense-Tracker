#include "expense_tracker.h"
#include "helpers.h"
#include "auth.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <climits>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

expense_tracker::expense_tracker()
{
    ID = 1;
}

bool expense_tracker::amount_low_to_high(const expense &a, const expense &b)
{
    return a.amount < b.amount;
}

bool expense_tracker::amount_high_to_low(const expense &a, const expense &b)
{
    return a.amount > b.amount;
}

bool expense_tracker::category_low_to_high(const expense &a, const expense &b)
{
    return convert_lower(a.category) < convert_lower(b.category);
}

void expense_tracker::extract_date(const string &date, int &day, int &month, int &year)
{
    day = stoi(date.substr(0, 2));
    month = stoi(date.substr(3, 2));
    year = stoi(date.substr(6, 4));
}

bool expense_tracker::date_low_to_high(const expense &a, const expense &b)
{
    int d1, m1, y1;
    int d2, m2, y2;

    extract_date(a.date, d1, m1, y1);
    extract_date(b.date, d2, m2, y2);

    if (y1 != y2)
    {
        return y1 < y2;
    }
    if (m1 != m2)
    {
        return m1 < m2;
    }
    return d1 < d2;
}

int expense_tracker::find_idx(int ID)
{
    for (int i = 0; i < expenses.size(); i++)
    {
        if (expenses[i].ID == ID)
        {
            return i;
        }
    }
    return -1;
}

void expense_tracker::Set_Budget()
{
    WORD old = save_color();
    int amount;
    setColor(WHITE);
    cout << "Enter Budget: ";
    setColor(old);
    if (!get_int(amount))
    {
        setColor(RED);
        cout << "Enter Valid Amount!\n";
        setColor(old);
        return;
    }
    else if (amount < 0)
    {
        setColor(RED);
        cout << "Enter Valid Amount!\n";
        setColor(old);
        return;
    }
    else
    {
        default_Budget = amount;
    }
}

// Save to file
void expense_tracker::save_to_file()
{
    WORD old = save_color();
    ofstream file("expenses.dat");

    for (auto &i : expenses)
    {
        file << encrypt(to_string(i.ID)) << "|"
             << encrypt(i.category) << "|"
             << encrypt(to_string(i.amount)) << "|"
             << encrypt(i.date) << endl;
    }
    setColor(GREEN);
    cout << "Data Saved " << endl;
    setColor(old);
    file.close();
}

// Add expense
void expense_tracker::Add_expense(int amount, string category, string date)
{
    expenses.push_back({amount, category, date, ID});
    ID++;
    total += amount;
    record[category] += amount;
}

// Report functions
void expense_tracker::view_total()
{
    WORD old = save_color();
    setColor(CYAN);
    cout << "\n============================================\n";
    cout << "              TOTAL SPENT                   \n";
    cout << "============================================\n";
    setColor(old);
    cout << "  Total Amount: " << total << "\n";
    setColor(CYAN);
    cout << "============================================\n\n";
    setColor(old);
}

void expense_tracker::category_expenses()
{
    WORD old = save_color();
    setColor(CYAN);
    cout << "\n============================================\n";
    cout << "          CATEGORY BREAKDOWN                \n";
    cout << "============================================\n";
    setColor(old);
    if (record.empty())
    {
        setColor(WHITE);
        cout << "  No expenses recorded.\n";
        setColor(CYAN);
        cout << "============================================\n\n";
        setColor(old);
        return;
    }

    setColor(CYAN);
    cout << left << setw(25) << "  CATEGORY" << setw(15) << "AMOUNT" << endl;
    cout << "--------------------------------------------\n";
    setColor(old);
    for (auto &p : record)
    {
        cout << left << setw(25) << ("  " + p.first) << setw(15) << p.second << endl;
    }
    setColor(CYAN);
    cout << "============================================\n\n";
    setColor(old);
}

void expense_tracker::view_expenses()
{
    WORD old = save_color();
    setColor(CYAN);
    cout << "\n============================================\n";
    cout << "             ALL EXPENSES                   \n";
    cout << "============================================\n";
    setColor(old);
    if (expenses.empty())
    {
        setColor(WHITE);
        cout << "  No expenses recorded yet.\n";
        setColor(CYAN);
        cout << "============================================\n\n";
        setColor(old);
        return;
    }

    setColor(CYAN);
    cout << left
         << setw(8) << "  ID"
         << setw(12) << "AMOUNT"
         << setw(18) << "CATEGORY"
         << setw(15) << "DATE" << endl;

    cout << "--------------------------------------------\n";
    setColor(old);

    for (auto &p : expenses)
    {
        cout << left
             << setw(8) << ("  " + to_string(p.ID))
             << setw(12) << p.amount
             << setw(18) << p.category
             << setw(15) << p.date << endl;
    }
    setColor(CYAN);
    cout << "============================================\n\n";
    setColor(old);
}

// Delete
void expense_tracker::Delete(int ID)
{
    WORD old = save_color();
    int idx = find_idx(ID);
    if (idx != -1)
    {
        total -= expenses[idx].amount;
        record[expenses[idx].category] -= expenses[idx].amount;
        if (record[expenses[idx].category] == 0)
        {
            record.erase(expenses[idx].category);
        }

        expenses.erase(expenses.begin() + idx);
    }
    else
    {
        setColor(RED);
        cout << "This ID does not exist.\n";
        setColor(old);
    }
}

// Load from file
void expense_tracker::load_from_file()
{
    WORD old = save_color();
    ifstream file("expenses.dat");
    if (!file)
    {
        setColor(WHITE);
        cout << "No previous data found.\n";
        setColor(old);
    }
    else
    {
        setColor(GREEN);
        cout << "Loading your previous data...\n";
        setColor(old);
    }

    string enc_id, enc_category, enc_amount, enc_date;
    expenses.clear();
    record.clear();
    total = 0;
    int last_id = 0;

    while (true)
    {
        if (!getline(file, enc_id, '|'))
        {
            break;
        }
        if (!getline(file, enc_category, '|'))
        {
            break;
        }
        if (!getline(file, enc_amount, '|'))
        {
            break;
        }
        if (!getline(file, enc_date))
        {
            break;
        }

        int id = stoi(decrypt(enc_id));
        int amount = stoi(decrypt(enc_amount));
        string category = decrypt(enc_category);
        string date = decrypt(enc_date);

        expenses.push_back({amount, category, date, id});
        record[category] += amount;
        total += amount;
        last_id = id;
    }

    ID = last_id + 1;
    file.close();
}

// Show menu
void expense_tracker::show_menu()
{
    WORD old = save_color();
    setColor(CYAN);
    cout << "\n============================================\n";
    cout << "            COMMAND MENU                    \n";
    cout << "============================================\n";
    setColor(old);
    cout << "  0  : Show Menu\n";
    cout << "  1  : Add Expense\n";
    cout << "  2  : View Total\n";
    cout << "  3  : View Category Breakdown\n";
    cout << "  4  : View All Expenses\n";
    setColor(WHITE);
    cout << "  5  : Delete Expense\n";
    setColor(old);
    cout << "  6  : View Largest Expense\n";
    cout << "  7  : View Smallest Expense\n";
    cout << "  8  : Count Expenses\n";
    setColor(WHITE);
    cout << "  9  : Modify Expense\n";
    setColor(old);
    cout << "  10 : Search by ID\n";
    cout << "  11 : Search by Amount\n";
    cout << "  12 : Search by Category\n";
    cout << "  13 : Search by Date\n";
    cout << "  14 : Sort Amount Ascending\n";
    cout << "  15 : Sort Amount Descending\n";
    cout << "  16 : Sort Category Ascending\n";
    cout << "  17 : Sort Date Ascending\n";
    cout << "  18 : Total of a Month\n";
    cout << "  19 : Total of a Year\n";
    setColor(WHITE);
    cout << "  20 : Set Monthly Budget (Default = 5000)\n";
    setColor(old);
    cout << "  21 : View Financial Summary\n";
    setColor(RED);
    cout << "  22 : Clear ALL Data\n";
    setColor(WHITE);
    cout << "  23 : Change Master Password\n";
    setColor(old);
    cout << "  24 : Export CSV\n";
    cout << "  25 : Save to File\n";
    setColor(RED);
    cout << "  26 : Exit\n";
    setColor(CYAN);
    cout << "============================================\n\n";
    setColor(old);
}

// Count
void expense_tracker::count_expenses()
{
    cout << "Total number of expenses: " << expenses.size() << endl;
}

// Largest / smallest
void expense_tracker::show_largest()
{
    if (expenses.empty())
    {
        WORD old = save_color();
        setColor(WHITE);
        cout << "No expenses recorded.\n";
        setColor(old);
        return;
    }

    int maximum = INT_MIN;
    for (auto &i : expenses)
    {
        maximum = max(maximum, i.amount);
    }
    WORD old = save_color();
    setColor(CYAN);
    cout << "\n============================================\n";
    cout << "           LARGEST EXPENSE                  \n";
    cout << "============================================\n";
    setColor(old);
    for (auto &i : expenses)
    {
        if (i.amount == maximum)
        {
            cout << i.ID << " " << i.amount << " " << i.category << " " << i.date << endl;
        }
    }
    setColor(CYAN);
    cout << "============================================\n\n";
    setColor(old);
}

void expense_tracker::show_lowest()
{
    WORD old = save_color();
    if (expenses.empty())
    {
        setColor(WHITE);
        cout << "No expenses recorded.\n";
        setColor(old);
        return;
    }

    int minimum = INT_MAX;
    for (auto &i : expenses)
    {
        minimum = min(minimum, i.amount);
    }

    setColor(CYAN);
    cout << "\n============================================\n";
    cout << "           SMALLEST EXPENSE                 \n";
    cout << "============================================\n";
    setColor(old);
    for (auto &i : expenses)
    {
        if (i.amount == minimum)
        {
            cout << i.ID << " " << i.amount << " " << i.category << " " << i.date << endl;
        }
    }
    setColor(CYAN);
    cout << "============================================\n\n";
    setColor(old);
}

// Modify
void expense_tracker::modify(int ID, string category, string date, int amount)
{
    for (auto &i : expenses)
    {
        if (i.ID == ID)
        {
            total -= i.amount;
            record[i.category] -= i.amount;
            if (record[i.category] == 0)
            {
                record.erase(i.category);
            }

            i.amount = amount;
            i.category = category;
            i.date = date;

            total += amount;
            record[category] += amount;

            return;
        }
    }
}

// Search
void expense_tracker::search_by_ID(int ID)
{
    WORD old = save_color();
    int idx = find_idx(ID);
    if (idx != -1)
    {
        auto &i = expenses[idx];
        cout << i.ID << " " << i.amount << " " << i.category << " " << i.date << endl;
    }
    else
    {
        setColor(RED);
        cout << "ID not found.\n";
        setColor(old);
    }
}

void expense_tracker::search_by_amount(int amount)
{
    WORD old = save_color();
    bool found = false;
    for (auto &i : expenses)
    {
        if (i.amount == amount)
        {
            cout << i.ID << " " << i.amount << " " << i.category << " " << i.date << endl;
            found = true;
        }
    }

    if (!found)
    {
        setColor(RED);
        cout << "No expense found.\n";
        setColor(old);
    }
}

void expense_tracker::search_by_category(string category)
{
    WORD old = save_color();
    category = convert_lower(category);
    bool found = false;

    for (auto &i : expenses)
    {
        if (convert_lower(i.category) == category)
        {
            cout << i.ID << " " << i.amount << " " << i.category << " " << i.date << endl;
            found = true;
        }
    }

    if (!found)
    {
        setColor(RED);
        cout << "Category not found.\n";
        setColor(old);
    }
}

void expense_tracker::search_by_date(string date)
{
    WORD old = save_color();
    date = convert_lower(date);
    bool found = false;

    for (auto &i : expenses)
    {
        if (convert_lower(i.date) == date)
        {
            cout << i.ID << " " << i.amount << " " << i.category << " " << i.date << endl;
            found = true;
        }
    }

    if (!found)
    {
        setColor(RED);
        cout << "No expenses on this date.\n";
        setColor(old);
    }
}

// Sorting toolkit
void expense_tracker::sort_amount_ascending()
{
    sort(expenses.begin(), expenses.end(), amount_low_to_high);
    view_expenses();
}

void expense_tracker::sort_amount_descending()
{
    sort(expenses.begin(), expenses.end(), amount_high_to_low);
    view_expenses();
}

void expense_tracker::sort_category_ascending()
{
    sort(expenses.begin(), expenses.end(), category_low_to_high);
    view_expenses();
}

void expense_tracker::sort_date_ascending()
{
    sort(expenses.begin(), expenses.end(), date_low_to_high);
    view_expenses();
}

void expense_tracker::annual_expense(int year)
{
    int amount = 0;
    for (auto &i : expenses)
    {
        int d, m, y;
        extract_date(i.date, d, m, y);
        if (year == y)
        {
            amount += i.amount;
        }
    }
    cout << "You spent " << amount << " in the year " << year << endl;
}

void expense_tracker::monthly_expense(int month, int year)
{
    int amount = 0;

    for (auto &i : expenses)
    {
        int d, m, y;
        extract_date(i.date, d, m, y);
        if (year == y && month == m)
        {
            amount += i.amount;
        }
    }

    cout << "You spent " << amount << " in the month " << month << "/" << year << endl;
}

void expense_tracker::financial_summary()
{
    unordered_map<string, int> totals;
    string key;
    for (auto &i : expenses)
    {
        int day, month, year;

        extract_date(i.date, day, month, year);
        switch (month)
        {
        case 1:
        {
            key = "Jan " + to_string(year);
            break;
        }
        case 2:
        {
            key = "Feb " + to_string(year);
            break;
        }
        case 3:
        {
            key = "Mar " + to_string(year);
            break;
        }
        case 4:
        {
            key = "Apr " + to_string(year);
            break;
        }
        case 5:
        {
            key = "May " + to_string(year);
            break;
        }
        case 6:
        {
            key = "Jun " + to_string(year);
            break;
        }
        case 7:
        {
            key = "Jul " + to_string(year);
            break;
        }
        case 8:
        {
            key = "Aug " + to_string(year);
            break;
        }
        case 9:
        {
            key = "Sep " + to_string(year);
            break;
        }
        case 10:
        {
            key = "Oct " + to_string(year);
            break;
        }
        case 11:
        {
            key = "Nov " + to_string(year);
            break;
        }
        case 12:
        {
            key = "Dec " + to_string(year);
            break;
        }
        }

        totals[key] += i.amount;
    }

    vector<pair<string, int>> report(totals.begin(), totals.end());

    sort(report.begin(), report.end());

    for (auto &p : report)
    {
        cout << p.first << " : " << p.second << "   ";
        int diff = default_Budget - p.second;
        if (diff > 0)
        {
            WORD oldColour = save_color();
            setColor(GREEN);
            cout << "Under Budget" << endl;
            setColor(oldColour);
        }
        else if (diff < 0)
        {
            WORD oldcolor = save_color();
            setColor(RED);
            double percent = (-1 * diff * 100) / default_Budget;
            cout << "Monthly Budget Exceeded by " << percent << "%" << endl;
            setColor(oldcolor);
        }
        else
        {
            WORD oldcolor = save_color();
            setColor( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            cout << "Monthly Budget Reached" << endl;
            setColor(oldcolor);
        }
    }
}

void expense_tracker::ChangeMasterPassword()
{
    WORD old = save_color();
    if (!Authenticate())
    {
        return;
    }
    string password;

    setColor(WHITE);
    cout << "Enter New Password: ";
    setColor(old);
    do_ignore();
    password = get_string();
    SaveMasterHash(HashString(password));
    setColor(GREEN);
    cout << "Password Changed Successfully!\n";
    setColor(old);
    return;
}

// Export CSV
void expense_tracker::export_csv()
{
    WORD old = save_color();
    ofstream file("expenses.csv");
    file << "ID,Amount,Category,Date\n";
    for (auto &i : expenses)
    {
        file << i.ID << "," << i.amount << "," << i.category << "," << i.date << endl;
    }

    file.close();
    setColor(GREEN);
    cout << "CSV exported to expenses.csv\n";
    setColor(old);
}

// Clear all
void expense_tracker::clear_all()
{
    WORD old = save_color();
    setColor(WHITE);
    cout << "Confirm delete ALL data (yes/no): ";
    setColor(old);
    do_ignore();
    string ans = get_string();
    ans = convert_lower(ans);

    if (ans == "yes")
    {
        expenses.clear();
        record.clear();
        total = 0;
        save_to_file();
        export_csv();
    }
    else
    {
        setColor(RED);
        cout << "Cancelled.\n";
        setColor(old);
    }
}

// RUN LOOP
void expense_tracker::run()
{
    show_menu();
    int choice;
    bool running = true;

    while (running)
    {
        WORD old = save_color();
        setColor(WHITE);
        cout << "\nEnter choice: ";
        setColor(old);
        if (!get_int(choice))
        {
            setColor(RED);
            cout << "Invalid input.\n";
            setColor(old);
            continue;
        }

        switch (choice)
        {
        case 0:
        {
            show_menu();
            break;
        }
        case 1:
        {
            int amount;
            string category, date;
            setColor(WHITE);
            cout << "Amount: ";
            setColor(old);
            if (!get_int(amount) || amount <= 0)
            {
                setColor(RED);
                cout << "Invalid.\n";
                setColor(old);
                break;
            }

            do_ignore();
            setColor(WHITE);
            cout << "Category: ";
            setColor(old);
            category = get_string();

            setColor(WHITE);
            cout << "Date: ";
            setColor(old);
            date = get_string();
            if (!DateValidator(date))
            {
                setColor(RED);
                cout << "Invalid Date...\n";
                cout << "Format must be DD/MM/YYYY\n";
                setColor(old);
                break;
            }

            Add_expense(amount, category, date);
            save_to_file();
            export_csv();
            break;
        }
        case 2:
        {
            view_total();
            break;
        }
        case 3:
        {
            category_expenses();
            break;
        }
        case 4:
        {
            view_expenses();
            break;
        }
        case 5:
        {
            int id;
            setColor(WHITE);
            cout << "Enter ID: ";
            setColor(old);
            if (!get_int(id))
            {
                break;
            }
            Delete(id);
            save_to_file();
            export_csv();
            break;
        }
        case 6:
        {
            show_largest();
            break;
        }
        case 7:
        {
            show_lowest();
            break;
        }
        case 8:
        {
            count_expenses();
            break;
        }
        case 9:
        {
            int id, amount;
            string category, date;

            setColor(WHITE);
            cout << "New amount: ";
            setColor(old);
            if (!get_int(amount) || amount <= 0)
            {
                setColor(RED);
                cout << "Invalid.\n";
                setColor(old);
                break;
            }

            do_ignore();
            setColor(WHITE);
            cout << "New category: ";
            setColor(old);
            category = get_string();

            setColor(WHITE);
            cout << "New date: ";
            setColor(old);
            date = get_string();
            if (!DateValidator(date))
            {
                setColor(RED);
                cout << "Invalid Date...\n";
                cout << "Format must be DD/MM/YYYY\n";
                setColor(old);
                break;
            }
            setColor(WHITE);
            cout << "ID to modify: ";
            setColor(old);
            if (!get_int(id))
            {
                break;
            }

            modify(id, category, date, amount);
            save_to_file();
            export_csv();
            break;
        }
        case 10:
        {
            int id;
            setColor(WHITE);
            cout << "Enter ID: ";
            setColor(old);
            if (!get_int(id))
            {
                break;
            }
            search_by_ID(id);
            break;
        }
        case 11:
        {
            int amt;
            setColor(WHITE);
            cout << "Enter amount: ";
            setColor(old);
            if (!get_int(amt))
            {
                break;
            }
            search_by_amount(amt);
            break;
        }
        case 12:
        {
            do_ignore();
            string cat;
            setColor(WHITE);
            cout << "Enter category: ";
            setColor(old);
            cat = get_string();
            search_by_category(cat);
            break;
        }
        case 13:
        {
            do_ignore();
            string d;
            setColor(WHITE);
            cout << "Enter date: ";
            setColor(old);
            d = get_string();
            if (!DateValidator(d))
            {
                setColor(RED);
                cout << "WRONG FORMAT...\n";
                cout << "Format must be DD/MM/YYYY\n";
                setColor(old);
                break;
            }
            search_by_date(d);
            break;
        }
        case 14:
        {
            sort_amount_ascending();
            break;
        }
        case 15:
        {
            sort_amount_descending();
            break;
        }
        case 16:
        {
            sort_category_ascending();
            break;
        }
        case 17:
        {
            sort_date_ascending();
            break;
        }
        case 18:
        {
            int year, month;
            setColor(WHITE);
            cout << "Enter year: ";
            setColor(old);
            if (!get_int(year))
            {
                setColor(RED);
                cout << "Enter a valid input.\n";
                setColor(old);
                break;
            }
            setColor(WHITE);
            cout << "Enter month: ";
            setColor(old);
            if (!get_int(month))
            {
                setColor(RED);
                cout << "Enter a valid input.\n";
                setColor(old);
                break;
            }
            monthly_expense(month, year);
            break;
        }
        case 19:
        {
            int year;
            setColor(WHITE);
            cout << "Enter year: ";
            setColor(old);
            if (!get_int(year))
            {
                setColor(RED);
                cout << "Enter a valid input.\n";
                setColor(old);
                break;
            }

            annual_expense(year);
            break;
        }
        case 20:
        {
            Set_Budget();
            break;
        }
        case 21:
        {
            financial_summary();
            break;
        }
        case 22:
        {
            clear_all();
            save_to_file();
            export_csv();
            break;
        }
        case 23:
        {
            ChangeMasterPassword();
            break;
        }
        case 24:
        {
            export_csv();
            break;
        }
        case 25:
        {
            save_to_file();
            break;
        }
        case 26:
        {
            running = false;
            save_to_file();
            export_csv();
            setColor(WHITE);
            cout << "Exiting. Bye!\n";
            setColor(old);
            break;
        }
        default:
        {
            setColor(RED);
            cout << "Invalid choice.\n";
            setColor(old);
            break;
        }
        }
    }
};
