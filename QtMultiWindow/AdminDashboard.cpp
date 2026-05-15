#include "AdminDashboard.h"
#include "SystemContext.h"

#include <QComboBox>
#include <QFont>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScreen>
#include <QTextEdit>
#include <QVBoxLayout>

AdminDashboard::AdminDashboard(SystemContext* ctx, QWidget* parent) : QDialog(parent), context(ctx)
{
    buildUi();
}

void AdminDashboard::buildUi()
{
    setWindowTitle("Admin Panel");

    QRect available = QGuiApplication::primaryScreen()->availableGeometry();
    resize(static_cast<int>(available.width() * 0.95), static_cast<int>(available.height() * 0.92));

    QFont appFont("Poppins", 9);
    setFont(appFont);

    setStyleSheet(
        "QDialog { background-color: #272829; color: #D8D9DA; }"
        "QGroupBox { border: 1px solid #61677A; border-radius: 8px; margin-top: 14px; padding-top: 8px; color: #FFF6E0; font-size: 10pt; font-weight: 700; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 6px; color: #D8D9DA; }"
        "QLabel { color: #D8D9DA; font-size: 8pt; font-weight: 500; }"
        "QLineEdit, QComboBox, QTextEdit { background: #272829; color: #FFF6E0; border: 1px solid #61677A; border-radius: 5px; padding: 4px; font-size: 8pt; }"
        "QPushButton { background: #61677A; color: #FFF6E0; border: 1px solid #D8D9DA; border-radius: 7px; min-height: 26px; min-width: 100px; max-height: 30px; font-size: 8pt; font-weight: 600; padding: 2px 8px; }"
        "QPushButton:hover { background: #D8D9DA; color: #272829; }"
        "QPushButton:pressed { background: #61677A; color: #FFF6E0; }"
    );

    auto* layout = new QVBoxLayout(this);
    layout->setSpacing(6);
    layout->setContentsMargins(10, 10, 10, 10);

    auto* backBtn = new QPushButton("Back to Login", this);
    layout->addWidget(backBtn);

    auto* addBox = new QGroupBox("Add Consumer", this);
    auto* addLayout = new QGridLayout(addBox);
    addId = new QLineEdit(addBox);
    addName = new QLineEdit(addBox);
    addMonth = new QLineEdit(addBox);
    addType = new QComboBox(addBox);
    addType->addItems({"Residential", "Commercial", "Solar"});
    addUnits = new QLineEdit(addBox);
    addPeakUnits = new QLineEdit(addBox);
    addExportedUnits = new QLineEdit(addBox);
    auto* addBtn = new QPushButton("Add Consumer", addBox);

    addLayout->addWidget(new QLabel("ID"), 0, 0);
    addLayout->addWidget(addId, 0, 1);
    addLayout->addWidget(new QLabel("Name"), 0, 2);
    addLayout->addWidget(addName, 0, 3);
    addLayout->addWidget(new QLabel("Month"), 0, 4);
    addLayout->addWidget(addMonth, 0, 5);
    addLayout->addWidget(new QLabel("Type"), 1, 0);
    addLayout->addWidget(addType, 1, 1);
    addLayout->addWidget(new QLabel("Units Consumed"), 1, 2);
    addLayout->addWidget(addUnits, 1, 3);
    addLayout->addWidget(new QLabel("Peak Units"), 1, 4);
    addLayout->addWidget(addPeakUnits, 1, 5);
    addLayout->addWidget(new QLabel("Units Exported"), 2, 0);
    addLayout->addWidget(addExportedUnits, 2, 1);
    addLayout->addWidget(addBtn, 2, 5);

    addBox->setMaximumHeight(130);

    auto* updateBox = new QGroupBox("Update Consumer Units", this);
    auto* updateLayout = new QGridLayout(updateBox);
    updateId = new QLineEdit(updateBox);
    updateMonth = new QLineEdit(updateBox);
    updateUnits = new QLineEdit(updateBox);
    updatePeakUnits = new QLineEdit(updateBox);
    updateExportedUnits = new QLineEdit(updateBox);
    auto* updateBtn = new QPushButton("Update Consumer", updateBox);

    updateLayout->addWidget(new QLabel("Consumer ID"), 0, 0);
    updateLayout->addWidget(updateId, 0, 1);
    updateLayout->addWidget(new QLabel("Month"), 0, 2);
    updateLayout->addWidget(updateMonth, 0, 3);
    updateLayout->addWidget(new QLabel("Units Consumed"), 1, 0);
    updateLayout->addWidget(updateUnits, 1, 1);
    updateLayout->addWidget(new QLabel("Peak Units"), 1, 2);
    updateLayout->addWidget(updatePeakUnits, 1, 3);
    updateLayout->addWidget(new QLabel("Units Exported"), 1, 4);
    updateLayout->addWidget(updateExportedUnits, 1, 5);
    updateLayout->addWidget(updateBtn, 1, 6);

    updateBox->setMaximumHeight(105);

    auto* searchBox = new QGroupBox("Display/Search Consumers", this);
    auto* searchLayout = new QHBoxLayout(searchBox);
    searchConsumerId = new QLineEdit(searchBox);
    auto* searchConsumerBtn = new QPushButton("Search Consumer by ID", searchBox);
    auto* displayAllBtn = new QPushButton("Display All Consumers", searchBox);
    searchLayout->addWidget(new QLabel("Consumer ID"));
    searchLayout->addWidget(searchConsumerId);
    searchLayout->addWidget(searchConsumerBtn);
    searchLayout->addWidget(displayAllBtn);

    searchBox->setMaximumHeight(78);

    auto* ratesBox = new QGroupBox("Set Rates", this);
    auto* ratesLayout = new QGridLayout(ratesBox);
    peakRate = new QLineEdit(ratesBox);
    offPeakRate = new QLineEdit(ratesBox);
    commercialRate = new QLineEdit(ratesBox);
    solarRate = new QLineEdit(ratesBox);
    auto* setRatesBtn = new QPushButton("Save Rates", ratesBox);

    ratesLayout->addWidget(new QLabel("Peak Rate"), 0, 0);
    ratesLayout->addWidget(peakRate, 0, 1);
    ratesLayout->addWidget(new QLabel("Off-Peak Rate"), 0, 2);
    ratesLayout->addWidget(offPeakRate, 0, 3);
    ratesLayout->addWidget(new QLabel("Commercial Rate"), 1, 0);
    ratesLayout->addWidget(commercialRate, 1, 1);
    ratesLayout->addWidget(new QLabel("Solar Rate"), 1, 2);
    ratesLayout->addWidget(solarRate, 1, 3);
    ratesLayout->addWidget(setRatesBtn, 1, 5);

    ratesBox->setMaximumHeight(88);

    auto* userBox = new QGroupBox("User Management", this);
    auto* userLayout = new QGridLayout(userBox);
    regUser = new QLineEdit(userBox);
    regPass = new QLineEdit(userBox);
    regConsumerId = new QLineEdit(userBox);
    searchUserName = new QLineEdit(userBox);
    auto* registerBtn = new QPushButton("Register User", userBox);
    auto* searchUserBtn = new QPushButton("Search User", userBox);
    auto* statsBtn = new QPushButton("Show Statistics", userBox);

    userLayout->addWidget(new QLabel("Username"), 0, 0);
    userLayout->addWidget(regUser, 0, 1);
    userLayout->addWidget(new QLabel("Password"), 0, 2);
    userLayout->addWidget(regPass, 0, 3);
    userLayout->addWidget(new QLabel("Consumer ID"), 0, 4);
    userLayout->addWidget(regConsumerId, 0, 5);
    userLayout->addWidget(registerBtn, 0, 6);

    userLayout->addWidget(new QLabel("Search Username"), 1, 0);
    userLayout->addWidget(searchUserName, 1, 1);
    userLayout->addWidget(searchUserBtn, 1, 2);
    userLayout->addWidget(statsBtn, 1, 6);

    userBox->setMaximumHeight(118);

    output = new QTextEdit(this);
    output->setReadOnly(true);
    output->setMinimumHeight(static_cast<int>(available.height() * 0.35));

    layout->addWidget(addBox);
    layout->addWidget(updateBox);
    layout->addWidget(searchBox);
    layout->addWidget(ratesBox);
    layout->addWidget(userBox);
    layout->addWidget(output, 1);

    connect(addBtn, &QPushButton::clicked, this, &AdminDashboard::onAddConsumer);
    connect(updateBtn, &QPushButton::clicked, this, &AdminDashboard::onUpdateConsumer);
    connect(displayAllBtn, &QPushButton::clicked, this, &AdminDashboard::onDisplayAll);
    connect(searchConsumerBtn, &QPushButton::clicked, this, &AdminDashboard::onSearchConsumer);
    connect(setRatesBtn, &QPushButton::clicked, this, &AdminDashboard::onSetRates);
    connect(registerBtn, &QPushButton::clicked, this, &AdminDashboard::onRegisterUser);
    connect(searchUserBtn, &QPushButton::clicked, this, &AdminDashboard::onSearchUser);
    connect(statsBtn, &QPushButton::clicked, this, &AdminDashboard::onShowStatistics);
    connect(backBtn, &QPushButton::clicked, this, &AdminDashboard::onBackToLogin);
}

void AdminDashboard::appendOutput(const QString& text)
{
    output->append(text);
    output->append("--------------------------------");
}

void AdminDashboard::onAddConsumer()
{
    appendOutput(context->addConsumerText(
        addId->text().toInt(),
        addName->text(),
        addMonth->text(),
        addType->currentIndex() + 1,
        addUnits->text().toFloat(),
        addPeakUnits->text().toFloat(),
        addExportedUnits->text().toFloat()));
}

void AdminDashboard::onUpdateConsumer()
{
    appendOutput(context->updateConsumerText(
        updateId->text().toInt(),
        updateMonth->text(),
        updateUnits->text().toFloat(),
        updatePeakUnits->text().toFloat(),
        updateExportedUnits->text().toFloat()));
}

void AdminDashboard::onDisplayAll()
{
    appendOutput(context->displayAllConsumersText());
}

void AdminDashboard::onSearchConsumer()
{
    appendOutput(context->searchConsumerText(searchConsumerId->text().toInt()));
}

void AdminDashboard::onSetRates()
{
    appendOutput(context->setRatesText(
        peakRate->text().toFloat(),
        offPeakRate->text().toFloat(),
        commercialRate->text().toFloat(),
        solarRate->text().toFloat()));
}

void AdminDashboard::onRegisterUser()
{
    appendOutput(context->registerUserText(
        regUser->text(),
        regPass->text(),
        regConsumerId->text().toInt()));
}

void AdminDashboard::onSearchUser()
{
    appendOutput(context->searchUserText(searchUserName->text()));
}

void AdminDashboard::onShowStatistics()
{
    appendOutput(context->statisticsText());
}

void AdminDashboard::onBackToLogin()
{
    if (parentWidget() != nullptr)
        parentWidget()->show();

    close();
}
