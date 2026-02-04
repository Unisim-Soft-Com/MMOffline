#include "FooterButton.h"
#include "Widgets/utils/ElementsStyles.h"
#include <QPainter>
#include <QPainterPath>

FooterButton::FooterButton(Style style, QWidget* parent)
    : QPushButton(parent), m_iconSize(28, 28), m_style(style), m_hovered(false), m_pressed(false)
{
    init();
}

FooterButton::FooterButton(const QIcon& icon, const QString& text, Style style, QWidget* parent)
    : QPushButton(text, parent), m_icon(icon), m_iconSize(28, 28), m_style(style), m_hovered(false), m_pressed(false)
{
    init();
}

void FooterButton::init()
{
    // Adaptive height for footer buttons
    setMinimumHeight(calculateAdaptiveButtonHeight(0.13));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setCursor(Qt::PointingHandCursor);
    setFlat(true);
}

void FooterButton::setIcon(const QIcon& icon)
{
    m_icon = icon;
    update();
}

void FooterButton::setIconSize(const QSize& size)
{
    m_iconSize = size;
    update();
}

void FooterButton::setButtonStyle(Style style)
{
    m_style = style;
    update();
}

void FooterButton::enterEvent(QEvent* event)
{
    m_hovered = true;
    update();
    QPushButton::enterEvent(event);
}

void FooterButton::leaveEvent(QEvent* event)
{
    m_hovered = false;
    update();
    QPushButton::leaveEvent(event);
}

// ==================== Color Methods ====================

QColor FooterButton::backgroundColor() const
{
    switch (m_style)
    {
    case Primary:
        return QColor("#2A7BE4");
    case Secondary:
        return QColor("#EBF4FF");
    case Danger:
        return QColor("#FFEAEC");
    case Success:
        return QColor("#38A169");
    default:
        return QColor("#2A7BE4");
    }
}

QColor FooterButton::backgroundColorHovered() const
{
    switch (m_style)
    {
    case Primary:
        return QColor("#1F5BB8");
    case Secondary:
        return QColor("#DBEAFE");
    case Danger:
        return QColor("#FFD8DB");
    case Success:
        return QColor("#2F855A");
    default:
        return QColor("#1F5BB8");
    }
}

QColor FooterButton::backgroundColorPressed() const
{
    switch (m_style)
    {
    case Primary:
        return QColor("#1a4f9e");
    case Secondary:
        return QColor("#BFDBFE");
    case Danger:
        return QColor("#FFC8CC");
    case Success:
        return QColor("#276749");
    default:
        return QColor("#1a4f9e");
    }
}

QColor FooterButton::borderColor() const
{
    switch (m_style)
    {
    case Primary:
        return QColor("#1c5399");
    case Secondary:
        return QColor("#BFDBFE");
    case Danger:
        return QColor("#FFC3C8");
    case Success:
        return QColor("#2F855A");
    default:
        return QColor("#1c5399");
    }
}

QColor FooterButton::textColor() const
{
    switch (m_style)
    {
    case Primary:
        return QColor("#FFFFFF");
    case Secondary:
        return QColor("#2A7BE4");
    case Danger:
        return QColor("#B3261E");
    case Success:
        return QColor("#FFFFFF");
    default:
        return QColor("#FFFFFF");
    }
}

// ==================== Paint ====================

void FooterButton::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int w = width();
    const int h = height();
    const int radius = 18;
    const int padding = 12;

    // === Determine current background color ===
    QColor bgColor;
    if (isDown() || m_pressed)
        bgColor = backgroundColorPressed();
    else if (m_hovered)
        bgColor = backgroundColorHovered();
    else
        bgColor = backgroundColor();

    // === Draw background ===
    QPainterPath path;
    path.addRoundedRect(QRectF(0, 0, w, h), radius, radius);

    painter.fillPath(path, bgColor);

    // Border
    QPen borderPen(borderColor());
    borderPen.setWidth(1);
    painter.setPen(borderPen);
    painter.drawPath(path);

    // === Calculate layout ===
    const int gap = 6;
    const int textHeight = 20;
    const int totalContentHeight = m_iconSize.height() + gap + textHeight;
    const int contentTop = (h - totalContentHeight) / 2;

    // === Draw icon ===
    if (!m_icon.isNull())
    {
        int iconX = (w - m_iconSize.width()) / 2;
        int iconY = contentTop;
        QRect iconRect(iconX, iconY, m_iconSize.width(), m_iconSize.height());
        QPixmap pixmap = m_icon.pixmap(m_iconSize, isEnabled() ? QIcon::Normal : QIcon::Disabled);
        painter.drawPixmap(iconRect, pixmap);
    }

    // === Draw text ===
    QString btnText = text();
    if (!btnText.isEmpty())
    {
        int textY = contentTop + m_iconSize.height() + gap;
        QRect textRect(padding, textY, w - padding * 2, textHeight);

        QFont font = painter.font();
        font.setPixelSize(18);
        font.setWeight(QFont::DemiBold);
        painter.setFont(font);

        painter.setPen(textColor());
        painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignTop, btnText);
    }
}
