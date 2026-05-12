#pragma once

#include <QDialog>

class QLineEdit;
class QPushButton;
class SystemContext;

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget* parent = nullptr);

private slots:
    void openAdminDashboard();
    void openUserDashboard();

private:
    SystemContext* context;
    QLineEdit* username;
    QLineEdit* password;
    QPushButton* adminButton;
    QPushButton* userButton;

    void buildUi();
};
