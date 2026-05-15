#include "LoginWindow.h"
#include "SystemContext.h"
#include "AdminDashboard.h"
#include "UserDashboard.h"

#include <QFont>
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
    resize(460, 260);

    QFont appFont("Poppins", 10);
    setFont(appFont);

    setStyleSheet(
        "QDialog { background-color: #272829; color: #D8D9DA; }"
        "QLabel { color: #D8D9DA; font-size: 10pt; font-weight: 500; }"
        "QLineEdit { background: #272829; color: #FFF6E0; border: 1px solid #61677A; border-radius: 7px; padding: 8px; font-size: 10pt; }"
        "QPushButton { background: #61677A; color: #FFF6E0; border: 1px solid #D8D9DA; border-radius: 9px; min-height: 40px; min-width: 220px; font-size: 10pt; font-weight: 600; padding: 8px 16px; }"
        "QPushButton:hover { background: #D8D9DA; color: #272829; }"
        "QPushButton:pressed { background: #61677A; color: #FFF6E0; }"
    );

    auto* layout = new QVBoxLayout(this);
    layout->setSpacing(12);
    layout->setContentsMargins(20, 20, 20, 20);

    auto* form = new QFormLayout();
    form->setVerticalSpacing(10);

    username = new QLineEdit(this);
    password = new QLineEdit(this);
    password->setEchoMode(QLineEdit::Password);

    form->addRow("Username", username);
    form->addRow("Password", password);
    layout->addLayout(form);

    adminButton = new QPushButton("Login as Admin", this);
    userButton = new QPushButton("Login as User", this);
    layout->addWidget(adminButton, 0, Qt::AlignHCenter);
    layout->addWidget(userButton, 0, Qt::AlignHCenter);

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

    auto* dash = new AdminDashboard(context, this);
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

    auto* dash = new UserDashboard(context, consumerID, this);
    dash->setAttribute(Qt::WA_DeleteOnClose);
    dash->show();
    hide();
}
