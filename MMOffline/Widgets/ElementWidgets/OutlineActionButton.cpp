#include "OutlineActionButton.h"
#include "Widgets/utils/ElementsStyles.h"
#include <QPainter>
#include <QPainterPath>

OutlineActionButton::OutlineActionButton(Style style, QWidget* parent)
    : QPushButton(parent)
    , m_iconSize(24, 24)
    , m_style(style)
    , m_hovered(false)
{
    init();
}

OutlineActionButton::OutlineActionButton(const QIcon& icon, const QString& text, Style style, QWidget* parent)
    : QPushButton(text, parent)
    , m_icon(icon)
    , m_iconSize(24, 24)
    , m_style(style)
    , m_hovered(false)
{
    init();
}

void OutlineActionButton::init()
{
    setMinimumHeight(60);
    setMaximumHeight(70);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setCursor(Qt::PointingHandCursor);
    setFlat(true);
}

void OutlineActionButton::setIcon(const QIcon& icon)
{
    m_icon = icon;
    update();
}

void OutlineActionButton::setIconSize(const QSize& size)
{
    m_iconSize = size;
    update();
}

void OutlineActionButton::setButtonStyle(Style style)
{
    m_style = style;
    update();
}

void OutlineActionButton::enterEvent(QEvent* event)
{
    m_hovered = true;
    update();
    QPushButton::enterEvent(event);
}

void OutlineActionButton::leaveEvent(QEvent* event)
{
    m_hovered = false;
    update();
    QPushButton::leaveEvent(event);
}

// ==================== Color Methods ====================

QColor OutlineActionButton::borderColor() const
{
    if (m_style == Red) return QColor("#D32F2F");
    return (m_style == Blue) ? QColor("#1976D2") : QColor("#43A047");
}

QColor OutlineActionButton::borderColorHovered() const
{
    if (m_style == Red) return QColor("#C62828");
    return (m_style == Blue) ? QColor("#1565C0") : QColor("#388E3C");
}

QColor OutlineActionButton::backgroundColor() const
{
    return QColor("#FFFFFF");
}

QColor OutlineActionButton::backgroundColorHovered() const
{
    if (m_style == Red) return QColor("#FFEBEE");
    return (m_style == Blue) ? QColor("#E3F2FD") : QColor("#E8F5E9");
}

QColor OutlineActionButton::textColor() const
{
    if (m_style == Red) return QColor("#D32F2F");
    return (m_style == Blue) ? QColor("#1976D2") : QColor("#43A047");
}

// ==================== Paint ====================

void OutlineActionButton::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int w = width();
    const int h = height();
    const int radius = 12;
    const int borderWidth = 2;

    // === Determine colors based on state ===
    QColor bgColor = m_hovered ? backgroundColorHovered() : backgroundColor();
    QColor brdColor = m_hovered ? borderColorHovered() : borderColor();

    // === Draw background ===
    QPainterPath path;
    QRectF bgRect(borderWidth / 2.0, borderWidth / 2.0,
                  w - borderWidth, h - borderWidth);
    path.addRoundedRect(bgRect, radius, radius);

    painter.fillPath(path, bgColor);

    // === Draw border ===
    QPen borderPen(brdColor);
    borderPen.setWidth(borderWidth);
    painter.setPen(borderPen);
    painter.drawPath(path);

    // === Calculate layout - icon and text closer together ===
    const int gap = 2;  // Small gap between icon and text
    const int textHeight = 22;
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
        QRect textRect(0, textY, w, textHeight);

        QFont font = painter.font();
        font.setPixelSize(18);
        font.setWeight(QFont::DemiBold);
        painter.setFont(font);

        painter.setPen(textColor());
        painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignTop, btnText);
    }
}
