# Multi-window Qt App

## Windows

- Login window
- Admin dashboard
- User dashboard

## Qt project file

Use `QtMultiWindow.pro` in Qt Creator.

## How to compile in Qt Creator

1. Open Qt Creator.
2. Click **File > Open File or Project**.
3. Select `QtMultiWindow/QtMultiWindow.pro`.
4. Choose a Qt kit that has Widgets support.
5. Click **Configure Project**.
6. Build the project with **Ctrl+B**.
7. Run it with **Ctrl+R**.

## Notes

- The Qt UI uses your existing console-system logic.
- Data is still saved in the same text files.
- The admin dashboard can open a user dashboard window too.

## Suggested workflow

- Login as admin to manage consumers, users, and rates.
- Login as user to view bills, history, and pay bills.
