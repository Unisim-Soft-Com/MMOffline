#include "ModernAmountStepper.h"
#include "Widgets/utils/ElementsStyles.h"
#include <QLocale>
#include <QGuiApplication>
#include <QInputMethod>
#include <QLineEdit>

ModernAmountStepper::ModernAmountStepper(QWidget* parent)
    : QWidget(parent), m_step(1.0)
{
    init();
    applyStyles();
}

void ModernAmountStepper::init()
{
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);

    // Minus button
    m_minusBtn = new QPushButton(QStringLiteral("\u2212"), this);  // Unicode minus
    m_minusBtn->setObjectName("stepperMinus");
    m_minusBtn->setFixedWidth(56);
    m_minusBtn->setMinimumHeight(48);
    m_minusBtn->setCursor(Qt::PointingHandCursor);
    m_minusBtn->setFocusPolicy(Qt::NoFocus);

    // SpinBox (native buttons hidden via stylesheet)
    m_spinBox = new QDoubleSpinBox(this);
    spinBox()->setLocale(QLocale::c());
    m_spinBox->setObjectName("stepperValue");
    m_spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_spinBox->setAlignment(Qt::AlignCenter);
    m_spinBox->setMinimumHeight(48);
    m_spinBox->setKeyboardTracking(false);

#ifdef Q_OS_ANDROID
    // Inchide tastatura la Enter (fara alte actiuni / fara navigare)
    QObject::connect(m_spinBox, &QAbstractSpinBox::editingFinished, this, [this]() {
        m_spinBox->clearFocus();
        QGuiApplication::inputMethod()->hide();
    });

    // Extra-sigur: prindem direct Enter din QLineEdit-ul intern
    if (auto le = m_spinBox->findChild<QLineEdit*>()) {
        QObject::connect(le, &QLineEdit::returnPressed, this, [this]() {
            m_spinBox->clearFocus();
            QGuiApplication::inputMethod()->hide();
        });
    }
#endif

    // Plus button
    m_plusBtn = new QPushButton("+", this);
    m_plusBtn->setObjectName("stepperPlus");
    m_plusBtn->setFixedWidth(56);
    m_plusBtn->setMinimumHeight(48);
    m_plusBtn->setCursor(Qt::PointingHandCursor);
    m_plusBtn->setFocusPolicy(Qt::NoFocus);

    m_layout->addWidget(m_minusBtn);
    m_layout->addWidget(m_spinBox, 1);
    m_layout->addWidget(m_plusBtn);

    // Connections
    QObject::connect(m_minusBtn, &QPushButton::clicked, this, &ModernAmountStepper::decrement);
    QObject::connect(m_plusBtn, &QPushButton::clicked, this, &ModernAmountStepper::increment);
    QObject::connect(m_spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                     this, &ModernAmountStepper::valueChanged);
}

void ModernAmountStepper::applyStyles()
{
    setStyleSheet(R"(
        /* Container frame effect via border */
        ModernAmountStepper {
            background: #FAFAFA;
            border: 2px solid #E2E8F0;
            border-radius: 12px;
        }

        /* Minus button */
        #stepperMinus {
            background: transparent;
            border: none;
            border-right: 1px solid #E2E8F0;
            border-top-left-radius: 10px;
            border-bottom-left-radius: 10px;
            color: #EF4444;
            font-size: 22px;
            font-weight: bold;
        }
        #stepperMinus:pressed {
            background: #FEE2E2;
        }

        /* Plus button */
        #stepperPlus {
            background: transparent;
            border: none;
            border-left: 1px solid #E2E8F0;
            border-top-right-radius: 10px;
            border-bottom-right-radius: 10px;
            color: #10B981;
            font-size: 22px;
            font-weight: bold;
        }
        #stepperPlus:pressed {
            background: #D1FAE5;
        }

        /* Value display */
        #stepperValue {
            background: transparent;
            border: none;
            color: #1F2937;
            font-size: 18px;
            font-weight: 600;
        }
    )");
}

// ==================== Public API ====================

void ModernAmountStepper::setValue(double val)
{
    m_spinBox->setValue(val);
}

double ModernAmountStepper::value() const
{
    return m_spinBox->value();
}

void ModernAmountStepper::setMinimum(double min)
{
    m_spinBox->setMinimum(min);
}

void ModernAmountStepper::setMaximum(double max)
{
    m_spinBox->setMaximum(max);
}

void ModernAmountStepper::setPrecision(int decimals)
{
    m_spinBox->setDecimals(decimals);
}

void ModernAmountStepper::setStep(double step)
{
    m_step = step;
    m_spinBox->setSingleStep(step);
}

// ==================== Slots ====================

void ModernAmountStepper::increment()
{
    m_spinBox->setValue(m_spinBox->value() + m_step);
}

void ModernAmountStepper::decrement()
{
    double newVal = m_spinBox->value() - m_step;
    if (newVal >= m_spinBox->minimum())
        m_spinBox->setValue(newVal);
}
