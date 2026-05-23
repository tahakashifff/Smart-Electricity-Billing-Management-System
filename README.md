# Voltly (A Smart Electricity Billing & Management System)

It is a comprehensive electricity billing and management system with solar net-metering capabilities, peak-hour tracking, and OOP design principles using C++ and Qt Framework.

---

## 🖥️ Project Overview

This is a multi-window Qt application for managing electricity billing with features for both admins and users.  
The app provides:
- A login window (for admin and users)
- Admin dashboard (manage users, consumers, and tariff rates)
- User dashboard (view bills, pay, see usage/history)
- GUI built using Qt
- Data stored in text files

---

## 🚀 How to Run the Project

### 1. Prerequisites

- [Qt Creator](https://www.qt.io/product/development-tools) installed (with Qt Widgets support)
- C++ compiler (such as MSVC, MinGW, GCC/Clang)
- The source code and project files (including `QtMultiWindow.pro`)

### 2. Compilation Steps

1. Open **Qt Creator**.
2. Click **File > Open File or Project**.
3. Locate and select `QtMultiWindow/QtMultiWindow.pro`.
4. Choose a Qt kit that supports Widgets and click **Configure Project**.
5. Build the project using **Ctrl+B** or by clicking the build/run icons.
6. To start the application, press **Ctrl+R** or click the green Run button.

---

## 📝 Usage Notes

- Log in as admin to access management features (managing consumers, users, and rates).
- Log in as a user to view/payload bills and see your history.
- The admin dashboard window can open a user dashboard window for quick switching.
- All billing and user data is saved in text files (no database required).

---

## 💾 Data Storage

- Data is stored using plain text files for persistence.

---

## 📂 Project Structure

- `QtMultiWindow.pro` &mdash; Main Qt project file
- `src/` &mdash; Application source code (C++ & Qt)
- `data/` &mdash; Stored files for users, billing, and consumption data

---

## 👍 Suggested Workflow

1. Compile and run the application.
2. Log in (admin or user).
3. Use the dashboards to manage or view electrical usage and billing.

---

## ❓ Help

For issues or bugs, please create an issue in this repository or contact the project maintainers.
