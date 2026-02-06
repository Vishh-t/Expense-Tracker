#ifndef EXPENSE_TRACKER_H
#define EXPENSE_TRACKER_H

#include <string>
#include <vector>
#include <unordered_map>

// selective std
using std::string;
using std::unordered_map;
using std::vector;

class expense_tracker
{
public:
    expense_tracker();

    void load_from_file();
    void run();

private:
    // ---------------- DATA ----------------
    int total = 0;
    unordered_map<string, int> record;

    struct expense
    {
        int amount;
        string category;
        string date;
        int ID;
    };

    vector<expense> expenses;
    int ID = 1;
    int default_Budget = 5000;

    // ------------- INTERNAL HELPERS -------------
    int find_idx(int ID);

    static void extract_date(const string &date, int &day, int &month, int &year);

    static bool amount_low_to_high(const expense &a, const expense &b);
    static bool amount_high_to_low(const expense &a, const expense &b);
    static bool category_low_to_high(const expense &a, const expense &b);
    static bool date_low_to_high(const expense &a, const expense &b);

    // ------------- CORE OPERATIONS -------------
    void Add_expense(int amount, string category, string date);
    void Delete(int ID);
    void modify(int ID, string category, string date, int amount);

    void save_to_file();

    // ------------- UI / DISPLAY -------------
    void show_menu();
    void view_total();
    void category_expenses();
    void view_expenses();

    void count_expenses();
    void show_largest();
    void show_lowest();

    // ------------- SEARCH -------------
    void search_by_ID(int ID);
    void search_by_amount(int amount);
    void search_by_category(string category);
    void search_by_date(string date);
    void SearchinRangeforamount(int low, int high);
    void SearchinRangefordate(string startDate, string endDate);

        // ------------- SORTING -------------
        void sort_amount_ascending();
    void sort_amount_descending();
    void sort_category_ascending();
    void sort_date_ascending();

    // ------------- REPORTS -------------
    void annual_expense(int year);
    void monthly_expense(int month, int year);
    void financial_summary();

    // ------------- SETTINGS / MISC -------------
    void Set_Budget();
    void ChangeMasterPassword();
    void export_csv();
    void clear_all();
};

#endif
