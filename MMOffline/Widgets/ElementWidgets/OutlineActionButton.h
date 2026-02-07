#pragma once
#include <QPushButton>
#include <QIcon>

/*
    Modern outline-style action button for footers.
    Features a clean outline design with icon above text.

    Two styles available:
    - Blue: For Back/Cancel actions
    - Green: For OK/Confirm actions

    Usage:
        OutlineActionButton* backBtn = new OutlineActionButton(OutlineActionButton::Blue, this);
        backBtn->setIcon(SvgHelper::iconFromSvg(SvgHelper::SVG_ARROW_LEFT, "#1976D2"));
        backBtn->setText(tr("Back"));

        OutlineActionButton* okBtn = new OutlineActionButton(OutlineActionButton::Green, this);
        okBtn->setIcon(SvgHelper::iconFromSvg(SvgHelper::SVG_CHECK, "#43A047"));
        okBtn->setText(tr("OK"));
*/

class OutlineActionButton : public QPushButton
{
    Q_OBJECT

public:
    enum Style {
        Blue,   // For Back, Cancel
        Green,  // For OK, Confirm, Save
        Red     // For Delete, Remove
    };

    explicit OutlineActionButton(Style style = Blue, QWidget* parent = nullptr);
    explicit OutlineActionButton(const QIcon& icon, const QString& text, Style style = Blue, QWidget* parent = nullptr);

    void setIcon(const QIcon& icon);
    void setIconSize(const QSize& size);
    void setButtonStyle(Style style);

protected:
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    QIcon m_icon;
    QSize m_iconSize;
    Style m_style;
    bool m_hovered;

    void init();

    QColor borderColor() const;
    QColor borderColorHovered() const;
    QColor backgroundColor() const;
    QColor backgroundColorHovered() const;
    QColor textColor() const;
};
