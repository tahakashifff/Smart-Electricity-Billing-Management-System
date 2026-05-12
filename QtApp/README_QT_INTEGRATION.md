# Qt Integration Guide

## What was added

This workspace now includes a Qt-ready controller and a basic QWidget GUI structure:

- `QtApp/QtSystemController.h`
- `QtApp/QtSystemController.cpp`
- `QtApp/MainWindow.h`
- `QtApp/MainWindow.cpp`
- `QtApp/main.cpp`

The GUI reuses your existing business logic from the console project.

## How Qt is integrated

The core idea is to keep your current system logic in the existing C++ classes:

- `Admin`
- `Consumer`
- `FileManager`
- `LoginManager`
- `MonthlyRecords`

Then the Qt layer calls those classes from a GUI window instead of a console menu.

### Flow

1. Qt UI collects input from text boxes and buttons.
2. `QtSystemController` validates and forwards the request to your existing classes.
3. Your existing file-based storage is still used:
   - `Consumers.txt`
   - `users.txt`
   - `rates.txt`
   - `bills.txt`
4. Results are shown in the GUI output panel.

## How to use Qt

### 1) Install Qt

Install Qt 6 with Qt Creator and a compiler that matches your project setup.

### 2) Create a Qt Widgets project

Create a new Qt Widgets Application and add your existing source files plus the `QtApp` files.

### 3) Add the existing source files

Add these files to the Qt project:

- `Admin.cpp/.h`
- `Consumer.cpp/.h`
- `FileManager.cpp/.h`
- `Input.cpp/.h`
- `LoginManager.cpp/.h`
- `MonthlyRecords.cpp/.h`

### 4) Add the Qt files

Add:

- `QtApp/main.cpp`
- `QtApp/MainWindow.cpp/.h`
- `QtApp/QtSystemController.cpp/.h`

### 5) Link Qt Widgets

Make sure your project links Qt Widgets and includes Qt headers.

## Build setup example

### If using qmake

Add Qt Widgets and the source files to your `.pro` file.

### If using CMake

Link against Qt::Widgets and include all source files in the target.

## How to run

After build, open the app and use the form fields to:

- log in as admin/user
- add consumers
- register users
- change rates
- search consumers
- view all consumers
- show statistics
- view bill history
- pay a bill

## Notes

- The current GUI is a starting point and can be improved visually.
- Console input helpers are still present in the old code, but the Qt layer does not use them.
- The GUI currently shares the same file storage, so data is preserved across runs.

## Suggested next step

If you want, the next improvement should be a cleaner multi-window Qt design with:

- separate login window
- admin dashboard
- user dashboard
- dialogs for add/update/pay actions
