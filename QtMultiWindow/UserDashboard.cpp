#include "UserDashboard.h"
#include "SystemContext.h"

#include <QFont>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScreen>
#include <QTextEdit>
#include <QVBoxLayout>

UserDashboard::UserDashboard(SystemContext* ctx, int id, QWidget* parent) : QDialog(parent), context(ctx), consumerID(id)
{
    buildUi();
}

void UserDashboard::buildUi()
{
    setWindowTitle("User Panel");

    QRect available = QGuiApplication::primaryScreen()->availableGeometry();
    resize(static_cast<int>(available.width() * 0.95), 720);

    QFont appFont("Poppins", 9);
    setFont(appFont);

    setStyleSheet(
        "QDialog { background-color: #272829; color: #D8D9DA; }"
        "QLabel { color: #D8D9DA; font-size: 9pt; font-weight: 500; }"
        "QLabel#headingLabel { color: #FFF6E0; font-size: 16pt; font-weight: 700; }"
        "QLineEdit, QTextEdit { background: #272829; color: #FFF6E0; border: 1px solid #61677A; border-radius: 6px; padding: 8px; font-size: 9pt; }"
        "QPushButton { background: #61677A; color: #FFF6E0; border: 1px solid #D8D9DA; border-radius: 8px; min-height: 40px; min-width: 180px; font-size: 9pt; font-weight: 600; padding: 8px 14px; }"
        "QPushButton:hover { background: #D8D9DA; color: #272829; }"
        "QPushButton:pressed { background: #61677A; color: #FFF6E0; }"
    );

    auto* layout = new QVBoxLayout(this);
    layout->setSpacing(12);
    layout->setContentsMargins(16, 16, 16, 16);

    auto* headingLabel = new QLabel("USER PANEL", this);
    headingLabel->setObjectName("headingLabel");
    layout->addWidget(headingLabel);

    auto* backBtn = new QPushButton("Back to Login", this);
    layout->addWidget(backBtn, 0, Qt::AlignLeft);

    auto* form = new QFormLayout();

    linkedConsumerId = new QLineEdit(this);
    linkedConsumerId->setText(QString::number(consumerID));
    linkedConsumerId->setReadOnly(true);

    month = new QLineEdit(this);
    month->setPlaceholderText("Example: May-2026");

    form->addRow("Linked Consumer ID", linkedConsumerId);
    form->addRow("Month to Pay", month);
    layout->addLayout(form);

    auto* viewBillBtn = new QPushButton("View Current Bill", this);
    auto* viewHistoryBtn = new QPushButton("View Bill History", this);
    auto* payBtn = new QPushButton("Pay Bill", this);

    layout->addWidget(viewBillBtn);
    layout->addWidget(viewHistoryBtn);
    layout->addWidget(payBtn);

    output = new QTextEdit(this);
    output->setReadOnly(true);
    layout->addWidget(output);

    connect(viewBillBtn, &QPushButton::clicked, this, &UserDashboard::onViewCurrentBill);
    connect(viewHistoryBtn, &QPushButton::clicked, this, &UserDashboard::onViewHistory);
    connect(payBtn, &QPushButton::clicked, this, &UserDashboard::onPayBill);
    connect(backBtn, &QPushButton::clicked, this, &UserDashboard::onBackToLogin);
}

void UserDashboard::appendOutput(const QString& text)
{
    output->append(text);
    output->append("--------------------------------");
}

void UserDashboard::onViewCurrentBill()
{
    appendOutput(context->viewCurrentBillText(consumerID));
}

void UserDashboard::onViewHistory()
{
    appendOutput(context->billHistoryText(consumerID));
}

void UserDashboard::onPayBill()
{
    appendOutput(context->payBillText(consumerID, month->text()));
}

void UserDashboard::onBackToLogin()
{
    if (parentWidget() != nullptr)
        parentWidget()->show();

    close();
}
