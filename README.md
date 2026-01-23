# 💰 Expense Tracker (C++)

A **Windows-based, console expense tracker** written in **C++** that helps users record, manage, and analyze expenses with secure authentication, encrypted storage, budgeting, and detailed summaries — all through an interactive, color-coded CLI.

---

## ✨ Features

- 🔐 **Master password authentication**
  - Hashed password storage
  - Masked input
  - Password change support

- 💾 **Persistent encrypted storage**
  - Expenses saved to file and auto-loaded on startup
  - Uses XOR-based obfuscation combined with Base64 encoding to ensure
    safe storage and delimiter-safe file parsing
  - Delimiter-safe parsing (supports spaces in categories)

- 🧾 **Expense management**
  - Add, view, modify, and delete expenses
  - Automatic ID assignment
  - Category-wise tracking

- 📅 **Date handling**
  - Strict `DD/MM/YYYY` validation
  - Leap year support
  - Chronological date sorting

- 🔍 **Search & sort**
  - Search by ID, amount, category, or date
  - Sort by amount, category, or date

- 📊 **Analysis & insights**
  - Total spending
  - Category breakdown
  - Monthly and yearly expense totals
  - Largest and smallest expense
  - Financial summary with budget comparison

- 💸 **Budget tracking**
  - Set a monthly budget
  - Visual warnings when budget is exceeded

- 📤 **Export**
  - Export expenses to CSV (Excel / Google Sheets)

- 🎨 **Enhanced UI**
  - Color-coded output using Windows Console API
  - Clear menus and formatted tables

---

## 🚀 Future Improvements

- 🔐 **Stronger encryption**
  - Replace XOR encryption with **AES** for secure data storage
    

- 📊 **Advanced analytics**
  - Monthly category-wise charts
  - Highest spending category per month

- ⚙️ **Configuration file**
  - Store budget limits and preferences in a config file
  - Allow custom currency and default settings


## 🛠️ Tech Stack

- **Language:** C++
- **Concepts:** STL, file I/O, hashing, encryption, sorting, validation
- **Platform:** Windows  
  (uses `<conio.h>` and `<windows.h>`)

---

## ▶️ Build & Run

This project uses **MinGW with a Makefile**.

```bash
mingw32-make
./expense_tracker.exe
