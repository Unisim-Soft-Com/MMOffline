#pragma once
#include <QPushButton>
#include <QIcon>
#include <QVBoxLayout>
#include <QLabel>

/*
    Card-style menu button for StartingScreen.
    Displays an icon above text with card appearance (white background,
    rounded corners, subtle shadow).

    Usage:
        CardMenuButton* btn = new CardMenuButton(parent);
        btn->setIcon(SvgHelper::iconFromSvg(SvgHelper::SVG_DOCUMENT, "#2A7BE4"));
        btn->setText(tr("Document nou"));
*/

class CardMenuButton : public QPushButton
{
    Q_OBJECT

public:
    explicit CardMenuButton(QWidget* parent = nullptr);
    explicit CardMenuButton(const QIcon& icon, const QString& text, QWidget* parent = nullptr);

    void setIcon(const QIcon& icon);
    void setIconSize(const QSize& size);

protected:
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    QIcon m_icon;
    QSize m_iconSize;
    bool m_hovered;

    void init();
};
