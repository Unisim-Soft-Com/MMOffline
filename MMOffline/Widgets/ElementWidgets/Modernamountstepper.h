#pragma once
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>

/*
    Modern amount stepper widget with clean +/- buttons.
    Replaces old BigButtonsSpinbox for numeric input.

    Features:
    - Styled minus/plus buttons with rounded corners
    - QDoubleSpinBox core (hidden native buttons)
    - Configurable min, max, precision, step

    Usage:
        ModernAmountStepper* stepper = new ModernAmountStepper(this);
        stepper->setMinimum(0);
        stepper->setMaximum(50000);
        stepper->setPrecision(2);
        stepper->setStep(1.0);
*/

class ModernAmountStepper : public QWidget
{
    Q_OBJECT

public:
    explicit ModernAmountStepper(QWidget* parent = nullptr);

    void setValue(double val);
    double value() const;
    void setMinimum(double min);
    void setMaximum(double max);
    void setPrecision(int decimals);
    void setStep(double step);

    // Expose the inner spinbox for focus management
    QDoubleSpinBox* spinBox() const { return m_spinBox; }

signals:
    void valueChanged(double value);

private slots:
    void increment();
    void decrement();

private:
    QHBoxLayout* m_layout;
    QPushButton* m_minusBtn;
    QPushButton* m_plusBtn;
    QDoubleSpinBox* m_spinBox;
    double m_step;

    void init();
    void applyStyles();
};
