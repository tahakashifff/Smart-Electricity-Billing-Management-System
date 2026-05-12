#include "UserDashboard.h"
#include "SystemContext.h"

#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

UserDashboard::UserDashboard(SystemContext* ctx, int id, QWidget* parent) : QDialog(parent), context(ctx), consumerID(id)
{
    buildUi();
}

void UserDashboard::buildUi()
{
    setWindowTitle("User Dashboard");
    resize(900, 650);

    auto* layout = new QVBoxLayout(this);
    auto* form = new QFormLayout();

    consumerIdInput = new QLineEdit(this);
    consumerIdInput->setText(QString::number(consumerID));
    month = new QLineEdit(this);

    form->addRow("Consumer ID", consumerIdInput);
    form->addRow("Month", month);
    layout->addLayout(form);

    auto* viewBillBtn = new QPushButton("View Bill", this);
    auto* viewHistoryBtn = new QPushButton("View History", this);
    auto* payBtn = new QPushButton("Pay Bill", this);

    layout->addWidget(viewBillBtn);
    layout->addWidget(viewHistoryBtn);
    layout->addWidget(payBtn);

    output = new QTextEdit(this);
    output->setReadOnly(true);
    layout->addWidget(output);

    connect(viewBillBtn, &QPushButton::clicked, this, &UserDashboard::onViewBill);
    connect(viewHistoryBtn, &QPushButton::clicked, this, &UserDashboard::onViewHistory);
    connect(payBtn, &QPushButton::clicked, this, &UserDashboard::onPayBill);
}

void UserDashboard::appendOutput(const QString& text)
{
    output->append(text);
}

void UserDashboard::onViewBill()
{
    int id = consumerIdInput->text().toInt();
    appendOutput(context->searchConsumerText(id));
}

void UserDashboard::onViewHistory()
{
    int id = consumerIdInput->text().toInt();
    appendOutput(context->billHistoryText(id));
}

void UserDashboard::onPayBill()
{
    int id = consumerIdInput->text().toInt();
    appendOutput(context->payBillText(id, month->text()));
}
