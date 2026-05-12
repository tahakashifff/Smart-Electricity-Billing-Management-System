#include "AdminDashboard.h"
#include "SystemContext.h"

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

AdminDashboard::AdminDashboard(SystemContext* ctx, QWidget* parent) : QDialog(parent), context(ctx)
{
    buildUi();
}

void AdminDashboard::buildUi()
{
    setWindowTitle("Admin Dashboard");
    resize(1000, 700);

    auto* layout = new QVBoxLayout(this);

    auto* userSearchBox = new QGroupBox("Search for User", this);
    auto* userSearchLayout = new QGridLayout(userSearchBox);
    searchUserName = new QLineEdit(userSearchBox);
    auto* searchUserBtn = new QPushButton("Search User", userSearchBox);
    userSearchLayout->addWidget(new QLabel("Username"), 0, 0);
    userSearchLayout->addWidget(searchUserName, 0, 1);
    userSearchLayout->addWidget(searchUserBtn, 0, 2);

    auto* registerBox = new QGroupBox("Register User and Save Units", this);
    auto* registerLayout = new QGridLayout(registerBox);
    regUser = new QLineEdit(registerBox);
    regPass = new QLineEdit(registerBox);
    regConsumerId = new QLineEdit(registerBox);
    consumerType = new QComboBox(registerBox);
    consumerType->addItems({"Residential", "Commercial", "Solar"});
    consumerName = new QLineEdit(registerBox);
    consumerMonth = new QLineEdit(registerBox);
    consumerUnits = new QLineEdit(registerBox);
    consumerPeakUnits = new QLineEdit(registerBox);
    consumerExported = new QLineEdit(registerBox);
    auto* registerBtn = new QPushButton("Register and Save", registerBox);

    registerLayout->addWidget(new QLabel("Username"), 0, 0);
    registerLayout->addWidget(regUser, 0, 1);
    registerLayout->addWidget(new QLabel("Password"), 0, 2);
    registerLayout->addWidget(regPass, 0, 3);
    registerLayout->addWidget(new QLabel("Consumer ID"), 0, 4);
    registerLayout->addWidget(regConsumerId, 0, 5);
    registerLayout->addWidget(new QLabel("Type"), 1, 0);
    registerLayout->addWidget(consumerType, 1, 1);
    registerLayout->addWidget(new QLabel("Name"), 1, 2);
    registerLayout->addWidget(consumerName, 1, 3);
    registerLayout->addWidget(new QLabel("Month"), 1, 4);
    registerLayout->addWidget(consumerMonth, 1, 5);
    registerLayout->addWidget(new QLabel("Units"), 2, 0);
    registerLayout->addWidget(consumerUnits, 2, 1);
    registerLayout->addWidget(new QLabel("Peak Units"), 2, 2);
    registerLayout->addWidget(consumerPeakUnits, 2, 3);
    registerLayout->addWidget(new QLabel("Exported Units"), 2, 4);
    registerLayout->addWidget(consumerExported, 2, 5);
    registerLayout->addWidget(registerBtn, 2, 6);

    output = new QTextEdit(this);
    output->setReadOnly(true);

    layout->addWidget(userSearchBox);
    layout->addWidget(registerBox);
    layout->addWidget(output);

    connect(searchUserBtn, &QPushButton::clicked, this, &AdminDashboard::onSearchUser);
    connect(registerBtn, &QPushButton::clicked, this, &AdminDashboard::onRegisterUser);
}

void AdminDashboard::appendOutput(const QString& text)
{
    output->append(text);
}

void AdminDashboard::onSearchUser()
{
    appendOutput(context->searchUserText(searchUserName->text()));
}

void AdminDashboard::onRegisterUser()
{
    appendOutput(context->registerUserAndSaveUnits(
        regUser->text(),
        regPass->text(),
        regConsumerId->text().toInt(),
        consumerType->currentIndex() + 1,
        consumerName->text(),
        consumerMonth->text(),
        consumerUnits->text().toFloat(),
        consumerPeakUnits->text().toFloat(),
        consumerExported->text().toFloat()));
}
