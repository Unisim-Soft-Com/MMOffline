#pragma once
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QDate>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QPaintEvent>

/*
    ModernDatePickerDialog - Full-screen overlay dialog with calendar.
    Follows the same pattern as ModernSelectionDialog:
    - Dark overlay background
    - Centered white card with gradient header
    - Custom calendar grid with month navigation
    - Date preview + confirm button

    Usage:
        ModernDatePickerDialog dlg(tr("Select delivery date"), currentDate, this);
        dlg.setMinimumDate(QDate::currentDate());  // optional: block past dates
        if (dlg.exec() == QDialog::Accepted) {
            QDate chosen = dlg.selectedDate();
        }
*/

class ModernDatePickerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ModernDatePickerDialog(const QString& title,
                                    const QDate& initialDate = QDate::currentDate(),
                                    QWidget* parent = nullptr);

    QDate selectedDate() const;
    void setMinimumDate(const QDate& date);

protected:
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void onPrevMonth();
    void onNextMonth();
    void onDayClicked();
    void onConfirm();

private:
    void setupUI(const QString& title);
    void positionCard();
    void rebuildCalendar();
    void updatePreview();

    QDate m_selectedDate;
    QDate m_minimumDate;
    int m_viewYear;
    int m_viewMonth;

    // UI elements
    QFrame* cardFrame;
    QVBoxLayout* cardLayout;

    // Header
    QFrame* headerFrame;
    QLabel* titleLabel;
    QPushButton* closeButton;

    // Month navigation
    QPushButton* prevMonthBtn;
    QPushButton* nextMonthBtn;
    QLabel* monthYearLabel;

    // Calendar grid
    QGridLayout* calendarGrid;
    QWidget* calendarWidget;
    QList<QPushButton*> dayButtons;

    // Preview + confirm
    QLabel* previewDateLabel;
    QLabel* previewDaysLabel;
    QPushButton* confirmButton;
};
