#pragma once
#include <QPushButton>
#include <QIcon>

/*
    Footer action button for StartingScreen.
    Supports two styles: Primary (blue/settings) and Danger (red/exit).
    Displays icon above text with rounded corners.

    Usage:
        FooterButton* settingsBtn = new FooterButton(FooterButton::Primary, parent);
        settingsBtn->setIcon(SvgHelper::iconFromSvg(SvgHelper::SVG_SETTINGS, "#FFFFFF"));
        settingsBtn->setText(tr("Setări"));

        FooterButton* exitBtn = new FooterButton(FooterButton::Danger, parent);
        exitBtn->setIcon(SvgHelper::iconFromSvg(SvgHelper::SVG_EXIT, "#B3261E"));
        exitBtn->setText(tr("Ieșire"));
*/

class FooterButton : public QPushButton
{
    Q_OBJECT

public:
    enum Style {
        Primary,    // Blue filled (Settings)
        Danger      // Red outline (Exit/Quit)
    };

    explicit FooterButton(Style style = Primary, QWidget* parent = nullptr);
    explicit FooterButton(const QIcon& icon, const QString& text, Style style = Primary, QWidget* parent = nullptr);

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
    bool m_pressed;

    void init();

    // Color getters based on style
    QColor backgroundColor() const;
    QColor backgroundColorHovered() const;
    QColor backgroundColorPressed() const;
    QColor borderColor() const;
    QColor textColor() const;
};
