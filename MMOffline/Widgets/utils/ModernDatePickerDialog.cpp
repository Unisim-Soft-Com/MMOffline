#include "ModernDatePickerDialog.h"
#include "Widgets/utils/SvgHelper.h"
#include <QPainter>
#include <QLocale>

// Romanian day/month names
static const char* DAY_HEADERS[] = { "Lu", "Ma", "Mi", "Jo", "Vi", "Sâ", "Du" };
static const char* MONTH_NAMES[] = {
    "Ianuarie", "Februarie", "Martie", "Aprilie", "Mai", "Iunie",
    "Iulie", "August", "Septembrie", "Octombrie", "Noiembrie", "Decembrie"
};

ModernDatePickerDialog::ModernDatePickerDialog(const QString& title,
                                               const QDate& initialDate,
                                               QWidget* parent)
    : QDialog(parent),
    m_selectedDate(initialDate),
    m_minimumDate(QDate(2000, 1, 1)),
    m_viewYear(initialDate.year()),
    m_viewMonth(initialDate.month())
{
    setupUI(title);
    rebuildCalendar();
    updatePreview();
}

void ModernDatePickerDialog::setupUI(const QString& title)
{
    // Full-screen frameless dialog (same as ModernSelectionDialog)
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setModal(true);

    QWidget* topLevel = parentWidget();
    while (topLevel && topLevel->parentWidget())
        topLevel = topLevel->parentWidget();

    if (topLevel) {
        setFixedSize(topLevel->size());
        move(topLevel->mapToGlobal(QPoint(0, 0)));
    }

    // === White card ===
    cardFrame = new QFrame(this);
    cardFrame->setObjectName("datePickerCard");
    cardFrame->setStyleSheet(
        "#datePickerCard {"
        "  background-color: #FFFFFF;"
        "  border-radius: 16px;"
        "}"
        );

    cardLayout = new QVBoxLayout(cardFrame);
    cardLayout->setContentsMargins(0, 0, 0, 0);
    cardLayout->setSpacing(0);

    // === Header (gradient, same as ModernSelectionDialog) ===
    headerFrame = new QFrame(cardFrame);
    headerFrame->setMinimumHeight(54);
    headerFrame->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "  stop:0 #1976D2, stop:1 #1565C0);"
        "border-top-left-radius: 16px;"
        "border-top-right-radius: 16px;"
        );

    QHBoxLayout* headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(20, 14, 14, 14);
    headerLayout->setSpacing(10);

    titleLabel = new QLabel(title, headerFrame);
    titleLabel->setStyleSheet(
        "background: transparent;"
        "color: #FFFFFF;"
        "font-size: 17px;"
        "font-weight: 600;"
        );

    closeButton = new QPushButton(headerFrame);
    closeButton->setText(QString::fromUtf8("\xC3\x97")); // ×
    closeButton->setFixedSize(32, 32);
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setStyleSheet(
        "QPushButton {"
        "  background: rgba(255, 255, 255, 50);"
        "  border: none;"
        "  border-radius: 16px;"
        "  color: #FFFFFF;"
        "  font-size: 18px;"
        "  font-weight: 600;"
        "}"
        "QPushButton:pressed {"
        "  background: rgba(255, 255, 255, 90);"
        "}"
        );
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);

    headerLayout->addWidget(titleLabel, 1);
    headerLayout->addWidget(closeButton);
    cardLayout->addWidget(headerFrame);

    // === Month navigation row ===
    QFrame* navFrame = new QFrame(cardFrame);
    navFrame->setStyleSheet("background: #FFFFFF; border: none;");
    QHBoxLayout* navLayout = new QHBoxLayout(navFrame);
    navLayout->setContentsMargins(12, 12, 12, 8);
    navLayout->setSpacing(8);

    prevMonthBtn = new QPushButton(navFrame);
    prevMonthBtn->setIcon(SvgHelper::iconFromSvg(SvgHelper::SVG_ARROW_LEFT,
                                                 "#1976D2", QSize(20, 20)));
    prevMonthBtn->setFixedSize(36, 36);
    prevMonthBtn->setCursor(Qt::PointingHandCursor);
    prevMonthBtn->setStyleSheet(
        "QPushButton {"
        "  background: #F0F7FF;"
        "  border: none;"
        "  border-radius: 18px;"
        "}"
        "QPushButton:pressed {"
        "  background: #DBEAFE;"
        "}"
        );
    connect(prevMonthBtn, &QPushButton::clicked, this, &ModernDatePickerDialog::onPrevMonth);

    monthYearLabel = new QLabel(navFrame);
    monthYearLabel->setAlignment(Qt::AlignCenter);
    monthYearLabel->setStyleSheet(
        "background: transparent;"
        "color: #1a3a5c;"
        "font-size: 15px;"
        "font-weight: 600;"
        );

    nextMonthBtn = new QPushButton(navFrame);
    nextMonthBtn->setIcon(SvgHelper::iconFromSvg(SvgHelper::SVG_ARROW_RIGHT,
                                                 "#1976D2", QSize(20, 20)));
    nextMonthBtn->setFixedSize(36, 36);
    nextMonthBtn->setCursor(Qt::PointingHandCursor);
    nextMonthBtn->setStyleSheet(prevMonthBtn->styleSheet());
    connect(nextMonthBtn, &QPushButton::clicked, this, &ModernDatePickerDialog::onNextMonth);

    navLayout->addWidget(prevMonthBtn);
    navLayout->addWidget(monthYearLabel, 1);
    navLayout->addWidget(nextMonthBtn);
    cardLayout->addWidget(navFrame);

    // === Calendar grid ===
    calendarWidget = new QWidget(cardFrame);
    calendarWidget->setStyleSheet("background: #FFFFFF;");
    calendarGrid = new QGridLayout(calendarWidget);
    calendarGrid->setContentsMargins(12, 4, 12, 8);
    calendarGrid->setSpacing(4);

    // Day-of-week headers (Lu, Ma, Mi, ...)
    for (int col = 0; col < 7; ++col) {
        QLabel* dayHeader = new QLabel(QString::fromUtf8(DAY_HEADERS[col]), calendarWidget);
        dayHeader->setAlignment(Qt::AlignCenter);
        dayHeader->setFixedHeight(28);
        dayHeader->setStyleSheet(
            "background: transparent;"
            "color: #9CA3AF;"
            "font-size: 11px;"
            "font-weight: 600;"
            );
        calendarGrid->addWidget(dayHeader, 0, col);
    }

    cardLayout->addWidget(calendarWidget);

    // === Preview bar ===
    QFrame* previewFrame = new QFrame(cardFrame);
    previewFrame->setStyleSheet(
        "background: #F0F7FF;"
        "border: 1px solid #DBEAFE;"
        "border-left: none;"
        "border-right: none;"
        );
    QHBoxLayout* previewLayout = new QHBoxLayout(previewFrame);
    previewLayout->setContentsMargins(20, 10, 20, 10);

    QVBoxLayout* previewLeftCol = new QVBoxLayout();
    previewLeftCol->setSpacing(2);
    QLabel* previewLabel = new QLabel(tr("Selected date"), previewFrame);
    previewLabel->setStyleSheet(
        "background: transparent; color: #6B7280; font-size: 11px; border: none;"
        );
    previewDateLabel = new QLabel(previewFrame);
    previewDateLabel->setStyleSheet(
        "background: transparent; color: #1a3a5c; font-size: 16px; font-weight: 700; border: none;"
        );
    previewLeftCol->addWidget(previewLabel);
    previewLeftCol->addWidget(previewDateLabel);

    QVBoxLayout* previewRightCol = new QVBoxLayout();
    previewRightCol->setSpacing(2);
    previewRightCol->setAlignment(Qt::AlignRight);
    QLabel* deliveryLabel = new QLabel(tr("Delivery"), previewFrame);
    deliveryLabel->setAlignment(Qt::AlignRight);
    deliveryLabel->setStyleSheet(
        "background: transparent; color: #6B7280; font-size: 11px; border: none;"
        );
    previewDaysLabel = new QLabel(previewFrame);
    previewDaysLabel->setAlignment(Qt::AlignRight);
    previewDaysLabel->setStyleSheet(
        "background: transparent; color: #1976D2; font-size: 14px; font-weight: 600; border: none;"
        );
    previewRightCol->addWidget(deliveryLabel);
    previewRightCol->addWidget(previewDaysLabel);

    previewLayout->addLayout(previewLeftCol, 1);
    previewLayout->addLayout(previewRightCol);
    cardLayout->addWidget(previewFrame);

    // === Confirm button ===
    QFrame* buttonFrame = new QFrame(cardFrame);
    buttonFrame->setStyleSheet(
        "background: #FFFFFF;"
        "border-bottom-left-radius: 16px;"
        "border-bottom-right-radius: 16px;"
        );
    QHBoxLayout* buttonLayout = new QHBoxLayout(buttonFrame);
    buttonLayout->setContentsMargins(16, 12, 16, 16);

    confirmButton = new QPushButton(buttonFrame);
    confirmButton->setCursor(Qt::PointingHandCursor);
    confirmButton->setMinimumHeight(48);
    confirmButton->setStyleSheet(
        "QPushButton {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #1976D2, stop:1 #1565C0);"
        "  border: none;"
        "  border-radius: 12px;"
        "  color: #FFFFFF;"
        "  font-size: 15px;"
        "  font-weight: 600;"
        "  padding: 12px;"
        "}"
        "QPushButton:pressed {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #1565C0, stop:1 #0D47A1);"
        "}"
        );
    connect(confirmButton, &QPushButton::clicked, this, &ModernDatePickerDialog::onConfirm);

    buttonLayout->addWidget(confirmButton);
    cardLayout->addWidget(buttonFrame);

    positionCard();
}

void ModernDatePickerDialog::positionCard()
{
    int w = width();
    int h = height();

    // Card: 90% width, auto height, centered
    int cardW = qMin((int)(w * 0.90), 400);
    int cardH = qMin((int)(h * 0.75), 540);
    int cardX = (w - cardW) / 2;
    int cardY = (h - cardH) / 2;

    cardFrame->setGeometry(cardX, cardY, cardW, cardH);
}

void ModernDatePickerDialog::rebuildCalendar()
{
    // Clear old day buttons
    qDeleteAll(dayButtons);
    dayButtons.clear();

    // Update month/year label
    monthYearLabel->setText(
        QString::fromUtf8(MONTH_NAMES[m_viewMonth - 1]) + " " + QString::number(m_viewYear)
        );

    // First day of month (Monday = 1 in Qt)
    QDate firstOfMonth(m_viewYear, m_viewMonth, 1);
    int startDow = firstOfMonth.dayOfWeek(); // 1=Mon, 7=Sun
    int daysInMonth = firstOfMonth.daysInMonth();

    QDate today = QDate::currentDate();

    // Style templates
    QString baseBtnStyle =
        "QPushButton {"
        "  border: none;"
        "  border-radius: %1px;"
        "  font-size: 14px;"
        "  font-weight: %2;"
        "  background: %3;"
        "  color: %4;"
        "}"
        "QPushButton:pressed {"
        "  background: %5;"
        "}";

    int btnSize = 40;
    int radius = btnSize / 2;

    for (int day = 1; day <= daysInMonth; ++day) {
        QDate date(m_viewYear, m_viewMonth, day);
        int col = (startDow - 1 + day - 1) % 7;
        int row = (startDow - 1 + day - 1) / 7 + 1; // row 0 = headers

        QPushButton* btn = new QPushButton(QString::number(day), calendarWidget);
        btn->setFixedSize(btnSize, btnSize);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setProperty("date", date);

        bool isPast = date < m_minimumDate;
        bool isSelected = (date == m_selectedDate);
        bool isToday = (date == today);

        if (isSelected) {
            btn->setStyleSheet(baseBtnStyle
                                   .arg(radius).arg("700")
                                   .arg("qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #1976D2, stop:1 #1565C0)")
                                   .arg("#FFFFFF")
                                   .arg("#0D47A1"));
        } else if (isPast) {
            btn->setStyleSheet(baseBtnStyle
                                   .arg(radius).arg("400")
                                   .arg("transparent")
                                   .arg("#D1D5DB")
                                   .arg("transparent"));
            btn->setEnabled(false);
            btn->setCursor(Qt::ArrowCursor);
        } else if (isToday) {
            btn->setStyleSheet(baseBtnStyle
                                   .arg(radius).arg("600")
                                   .arg("#EFF6FF")
                                   .arg("#1976D2")
                                   .arg("#DBEAFE"));
        } else {
            btn->setStyleSheet(baseBtnStyle
                                   .arg(radius).arg("400")
                                   .arg("transparent")
                                   .arg("#374151")
                                   .arg("#F3F4F6"));
        }

        connect(btn, &QPushButton::clicked, this, &ModernDatePickerDialog::onDayClicked);

        calendarGrid->addWidget(btn, row, col, Qt::AlignCenter);
        dayButtons.append(btn);
    }
}

void ModernDatePickerDialog::updatePreview()
{
    previewDateLabel->setText(m_selectedDate.toString("dd.MM.yyyy"));

    int daysUntil = QDate::currentDate().daysTo(m_selectedDate);
    if (daysUntil == 0)
        previewDaysLabel->setText(tr("Today"));
    else if (daysUntil == 1)
        previewDaysLabel->setText(tr("Tomorrow"));
    else if (daysUntil > 0)
        previewDaysLabel->setText(tr("In %1 days").arg(daysUntil));
    else
        previewDaysLabel->setText(tr("%1 days ago").arg(-daysUntil));

    confirmButton->setText(
        QString::fromUtf8("\xe2\x9c\x93") + "  " + tr("Confirm") +
        QString::fromUtf8(" \xe2\x80\x94 ") + m_selectedDate.toString("dd.MM.yyyy")
        );
}

// ==================== Slots ====================

void ModernDatePickerDialog::onPrevMonth()
{
    m_viewMonth--;
    if (m_viewMonth < 1) {
        m_viewMonth = 12;
        m_viewYear--;
    }
    rebuildCalendar();
}

void ModernDatePickerDialog::onNextMonth()
{
    m_viewMonth++;
    if (m_viewMonth > 12) {
        m_viewMonth = 1;
        m_viewYear++;
    }
    rebuildCalendar();
}

void ModernDatePickerDialog::onDayClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QDate date = btn->property("date").toDate();
    if (date.isValid() && date >= m_minimumDate) {
        m_selectedDate = date;
        rebuildCalendar();
        updatePreview();
    }
}

void ModernDatePickerDialog::onConfirm()
{
    accept();
}

// ==================== Public API ====================

QDate ModernDatePickerDialog::selectedDate() const
{
    return m_selectedDate;
}

void ModernDatePickerDialog::setMinimumDate(const QDate& date)
{
    m_minimumDate = date;
}

// ==================== Events (same pattern as ModernSelectionDialog) ====================

void ModernDatePickerDialog::resizeEvent(QResizeEvent* event)
{
    QDialog::resizeEvent(event);
    positionCard();
}

void ModernDatePickerDialog::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0, 0, 0, 140));
}

void ModernDatePickerDialog::mousePressEvent(QMouseEvent* event)
{
    if (!cardFrame->geometry().contains(event->pos())) {
        reject();
        return;
    }
    QDialog::mousePressEvent(event);
}
