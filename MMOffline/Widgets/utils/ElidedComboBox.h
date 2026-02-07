#pragma once
#include <QComboBox>
#include <QStylePainter>
#include <QStyleOptionComboBox>

class ElidedComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ElidedComboBox(QWidget* parent = nullptr) : QComboBox(parent) {}

protected:
    void paintEvent(QPaintEvent*) override
    {
        QStylePainter painter(this);
        QStyleOptionComboBox opt;
        initStyleOption(&opt);

        // Get the area where text is drawn
        QRect textRect = style()->subControlRect(
            QStyle::CC_ComboBox, &opt, QStyle::SC_ComboBoxEditField, this);

        // Elide text with "..." if too long
        QString elided = fontMetrics().elidedText(
            currentText(), Qt::ElideRight, textRect.width());
        opt.currentText = elided;

        // Draw the combo box and its label
        painter.drawComplexControl(QStyle::CC_ComboBox, opt);
        painter.drawControl(QStyle::CE_ComboBoxLabel, opt);
    }
};
