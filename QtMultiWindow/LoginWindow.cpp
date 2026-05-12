#include "LoginWindow.h"
#include "SystemContext.h"
#include "AdminDashboard.h"
#include "UserDashboard.h"

#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

LoginWindow::LoginWindow(QWidget* parent) : QDialog(parent), context(new SystemContext())
{
    buildUi();
}

void LoginWindow::buildUi()
{
    setWindowTitle("Login Window");
    auto* layout = new QVBoxLayout(this);
    auto* form = new QFormLayout();

    username = new QLineEdit(this);
    password = new QLineEdit(this);
    password->setEchoMode(QLineEdit::Password);

    form->addRow("Username", username);
    form->addRow("Password", password);
    layout->addLayout(form);

    adminButton = new QPushButton("Login as Admin", this);
    userButton = new QPushButton("Login as User", this);
    layout->addWidget(adminButton);
    layout->addWidget(userButton);

    connect(adminButton, &QPushButton::clicked, this, &LoginWindow::openAdminDashboard);
    connect(userButton, &QPushButton::clicked, this, &LoginWindow::openUserDashboard);
}

void LoginWindow::openAdminDashboard()
{
    if (!context->adminLogin(username->text(), password->text()))
    {
        QMessageBox::warning(this, "Login Failed", "Invalid admin credentials.");
        return;
    }

    auto* dash = new AdminDashboard(context);
    dash->setAttribute(Qt::WA_DeleteOnClose);
    dash->show();
    hide();
}

void LoginWindow::openUserDashboard()
{
    int consumerID = context->userLogin(username->text(), password->text());
    if (consumerID <= 0)
    {
        QMessageBox::warning(this, "Login Failed", "Invalid user credentials.");
        return;
    }

    auto* dash = new UserDashboard(context, consumerID);
    dash->setAttribute(Qt::WA_DeleteOnClose);
    dash->show();
    hide();
}
