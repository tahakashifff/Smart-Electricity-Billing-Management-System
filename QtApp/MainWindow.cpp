#include "MainWindow.h"
#include "QtSystemController.h"

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), controller(new QtSystemController())
{
    buildUi();
}

void MainWindow::buildUi()
{
    auto* central = new QWidget(this);
    auto* root = new QVBoxLayout(central);

    auto* loginBox = new QGroupBox("Login", central);
    auto* loginLayout = new QGridLayout(loginBox);
    adminUser = new QLineEdit(loginBox);
    adminPass = new QLineEdit(loginBox);
    userName = new QLineEdit(loginBox);
    userPass = new QLineEdit(loginBox);
    adminPass->setEchoMode(QLineEdit::Password);
    userPass->setEchoMode(QLineEdit::Password);
    auto* adminLoginBtn = new QPushButton("Admin Login", loginBox);
    auto* userLoginBtn = new QPushButton("User Login", loginBox);
    loginLayout->addWidget(new QLabel("Admin Username"), 0, 0);
    loginLayout->addWidget(adminUser, 0, 1);
    loginLayout->addWidget(new QLabel("Admin Password"), 0, 2);
    loginLayout->addWidget(adminPass, 0, 3);
    loginLayout->addWidget(adminLoginBtn, 0, 4);
    loginLayout->addWidget(new QLabel("User Username"), 1, 0);
    loginLayout->addWidget(userName, 1, 1);
    loginLayout->addWidget(new QLabel("User Password"), 1, 2);
    loginLayout->addWidget(userPass, 1, 3);
    loginLayout->addWidget(userLoginBtn, 1, 4);
    connect(adminLoginBtn, &QPushButton::clicked, this, &MainWindow::onAdminLogin);
    connect(userLoginBtn, &QPushButton::clicked, this, &MainWindow::onUserLogin);

    auto* consumerBox = new QGroupBox("Consumer Management", central);
    auto* consumerLayout = new QGridLayout(consumerBox);
    consumerType = new QComboBox(consumerBox);
    consumerType->addItems({"Residential", "Commercial", "Solar"});
    consumerId = new QLineEdit(consumerBox);
    consumerName = new QLineEdit(consumerBox);
    consumerMonth = new QLineEdit(consumerBox);
    consumerUnits = new QLineEdit(consumerBox);
    consumerPeakUnits = new QLineEdit(consumerBox);
    consumerExported = new QLineEdit(consumerBox);
    auto* addConsumerBtn = new QPushButton("Add Consumer", consumerBox);
    auto* searchBtn = new QPushButton("Search Consumer", consumerBox);
    consumerLayout->addWidget(new QLabel("Type"), 0, 0);
    consumerLayout->addWidget(consumerType, 0, 1);
    consumerLayout->addWidget(new QLabel("ID"), 0, 2);
    consumerLayout->addWidget(consumerId, 0, 3);
    consumerLayout->addWidget(new QLabel("Name"), 1, 0);
    consumerLayout->addWidget(consumerName, 1, 1);
    consumerLayout->addWidget(new QLabel("Month"), 1, 2);
    consumerLayout->addWidget(consumerMonth, 1, 3);
    consumerLayout->addWidget(new QLabel("Units"), 2, 0);
    consumerLayout->addWidget(consumerUnits, 2, 1);
    consumerLayout->addWidget(new QLabel("Peak Units"), 2, 2);
    consumerLayout->addWidget(consumerPeakUnits, 2, 3);
    consumerLayout->addWidget(new QLabel("Exported Units"), 3, 0);
    consumerLayout->addWidget(consumerExported, 3, 1);
    consumerLayout->addWidget(addConsumerBtn, 3, 2);
    consumerLayout->addWidget(searchBtn, 3, 3);
    connect(addConsumerBtn, &QPushButton::clicked, this, &MainWindow::onAddConsumer);
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::onSearchConsumer);

    auto* userBox = new QGroupBox("User Registration & Billing", central);
    auto* userLayout = new QGridLayout(userBox);
    regUser = new QLineEdit(userBox);
    regPass = new QLineEdit(userBox);
    regConsumerId = new QLineEdit(userBox);
    ratePeak = new QLineEdit(userBox);
    rateOffPeak = new QLineEdit(userBox);
    rateCommercial = new QLineEdit(userBox);
    rateSolar = new QLineEdit(userBox);
    historyId = new QLineEdit(userBox);
    payId = new QLineEdit(userBox);
    payMonth = new QLineEdit(userBox);
    auto* registerBtn = new QPushButton("Register User", userBox);
    auto* setRatesBtn = new QPushButton("Set Rates", userBox);
    auto* allBtn = new QPushButton("Show All Consumers", userBox);
    auto* statsBtn = new QPushButton("Statistics", userBox);
    auto* historyBtn = new QPushButton("View History", userBox);
    auto* payBtn = new QPushButton("Pay Bill", userBox);
    regPass->setEchoMode(QLineEdit::Password);
    userLayout->addWidget(new QLabel("Reg Username"), 0, 0);
    userLayout->addWidget(regUser, 0, 1);
    userLayout->addWidget(new QLabel("Reg Password"), 0, 2);
    userLayout->addWidget(regPass, 0, 3);
    userLayout->addWidget(new QLabel("Consumer ID"), 0, 4);
    userLayout->addWidget(regConsumerId, 0, 5);
    userLayout->addWidget(registerBtn, 0, 6);
    userLayout->addWidget(new QLabel("Peak Rate"), 1, 0);
    userLayout->addWidget(ratePeak, 1, 1);
    userLayout->addWidget(new QLabel("Off-Peak Rate"), 1, 2);
    userLayout->addWidget(rateOffPeak, 1, 3);
    userLayout->addWidget(new QLabel("Commercial Rate"), 1, 4);
    userLayout->addWidget(rateCommercial, 1, 5);
    userLayout->addWidget(new QLabel("Solar Rate"), 1, 6);
    userLayout->addWidget(rateSolar, 1, 7);
    userLayout->addWidget(setRatesBtn, 1, 8);
    userLayout->addWidget(allBtn, 2, 0);
    userLayout->addWidget(statsBtn, 2, 1);
    userLayout->addWidget(new QLabel("History ID"), 2, 2);
    userLayout->addWidget(historyId, 2, 3);
    userLayout->addWidget(historyBtn, 2, 4);
    userLayout->addWidget(new QLabel("Pay ID"), 2, 5);
    userLayout->addWidget(payId, 2, 6);
    userLayout->addWidget(new QLabel("Pay Month"), 2, 7);
    userLayout->addWidget(payMonth, 2, 8);
    userLayout->addWidget(payBtn, 2, 9);
    connect(registerBtn, &QPushButton::clicked, this, &MainWindow::onRegisterUser);
    connect(setRatesBtn, &QPushButton::clicked, this, &MainWindow::onSetRates);
    connect(allBtn, &QPushButton::clicked, this, &MainWindow::onAllConsumers);
    connect(statsBtn, &QPushButton::clicked, this, &MainWindow::onStatistics);
    connect(historyBtn, &QPushButton::clicked, this, &MainWindow::onViewHistory);
    connect(payBtn, &QPushButton::clicked, this, &MainWindow::onPayBill);

    output = new QTextEdit(central);
    output->setReadOnly(true);

    root->addWidget(loginBox);
    root->addWidget(consumerBox);
    root->addWidget(userBox);
    root->addWidget(output);
    setCentralWidget(central);
    setWindowTitle("Smart Electricity Billing Management System");
    resize(1200, 800);
}

void MainWindow::appendOutput(const QString& text)
{
    output->append(text);
}

void MainWindow::onAdminLogin()
{
    appendOutput(controller->adminLogin(adminUser->text(), adminPass->text()) ? "Admin login successful." : "Invalid admin credentials.");
}

void MainWindow::onUserLogin()
{
    int id = controller->userLogin(userName->text(), userPass->text());
    appendOutput(id > 0 ? QString("User login successful. Linked Consumer ID: %1").arg(id) : "Invalid user credentials.");
}

void MainWindow::onAddConsumer()
{
    appendOutput(controller->addConsumerRecord(consumerType->currentIndex() + 1, consumerId->text().toInt(), consumerName->text(), consumerMonth->text(), consumerUnits->text().toFloat(), consumerPeakUnits->text().toFloat(), consumerExported->text().toFloat()));
}

void MainWindow::onRegisterUser()
{
    appendOutput(controller->registerUserRecord(regUser->text(), regPass->text(), regConsumerId->text().toInt()));
}

void MainWindow::onSetRates()
{
    appendOutput(controller->setRatesRecord(ratePeak->text().toFloat(), rateOffPeak->text().toFloat(), rateCommercial->text().toFloat(), rateSolar->text().toFloat()));
}

void MainWindow::onSearchConsumer()
{
    appendOutput(controller->searchConsumerText(consumerId->text().toInt()));
}

void MainWindow::onAllConsumers()
{
    appendOutput(controller->allConsumersText());
}

void MainWindow::onStatistics()
{
    appendOutput(controller->statisticsText());
}

void MainWindow::onViewHistory()
{
    appendOutput(controller->billHistoryText(historyId->text().toInt()));
}

void MainWindow::onPayBill()
{
    appendOutput(controller->payBillText(payId->text().toInt(), payMonth->text()));
}
