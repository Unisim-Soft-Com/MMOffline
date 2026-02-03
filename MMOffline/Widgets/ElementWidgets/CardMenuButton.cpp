#include "CardMenuButton.h"
#include "Widgets/utils/ElementsStyles.h"
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsDropShadowEffect>

CardMenuButton::CardMenuButton(QWidget* parent)
    : QPushButton(parent), m_iconSize(40, 40), m_hovered(false)
{
    init();
}

CardMenuButton::CardMenuButton(const QIcon& icon, const QString& text, QWidget* parent)
    : QPushButton(text, parent), m_icon(icon), m_iconSize(40, 40), m_hovered(false)
{
    init();
}

void CardMenuButton::init()
{
    // Înălțime fixă mai mare pentru a încăpea icon + text
    setMinimumHeight(100);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setCursor(Qt::PointingHandCursor);

    // Remove default button styling
    setFlat(true);

    // Add subtle shadow effect
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(8);
    shadow->setColor(QColor(0, 0, 0, 10));
    shadow->setOffset(0, 2);
    setGraphicsEffect(shadow);
}

void CardMenuButton::setIcon(const QIcon& icon)
{
    m_icon = icon;
    update();
}

void CardMenuButton::setIconSize(const QSize& size)
{
    m_iconSize = size;
    update();
}

void CardMenuButton::enterEvent(QEvent* event)
{
    m_hovered = true;

    if (QGraphicsDropShadowEffect* shadow = qobject_cast<QGraphicsDropShadowEffect*>(graphicsEffect()))
    {
        shadow->setBlurRadius(12);
        shadow->setColor(QColor(42, 123, 228, 38));
        shadow->setOffset(0, 4);
    }

    update();
    QPushButton::enterEvent(event);
}

void CardMenuButton::leaveEvent(QEvent* event)
{
    m_hovered = false;

    if (QGraphicsDropShadowEffect* shadow = qobject_cast<QGraphicsDropShadowEffect*>(graphicsEffect()))
    {
        shadow->setBlurRadius(8);
        shadow->setColor(QColor(0, 0, 0, 10));
        shadow->setOffset(0, 2);
    }

    update();
    QPushButton::leaveEvent(event);
}

void CardMenuButton::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int w = width();
    const int h = height();
    const int radius = 12;

    // === Draw card background ===
    QPainterPath path;
    path.addRoundedRect(QRectF(0, 0, w, h), radius, radius);

    painter.fillPath(path, QColor("#FFFFFF"));

    QPen borderPen(m_hovered ? QColor("#AFC8FF") : QColor("#DDE5F2"));
    borderPen.setWidth(1);
    painter.setPen(borderPen);
    painter.drawPath(path);

    // === Layout simplu: icon centrat sus, text centrat jos ===
    const int gap = 8;
    const int topPadding = 16;
    const int bottomPadding = 14;

    // Icon position - centrat orizontal, sus cu padding
    int iconX = (w - m_iconSize.width()) / 2;
    int iconY = topPadding;

    if (!m_icon.isNull())
    {
        QRect iconRect(iconX, iconY, m_iconSize.width(), m_iconSize.height());
        QPixmap pixmap = m_icon.pixmap(m_iconSize, isEnabled() ? QIcon::Normal : QIcon::Disabled);
        painter.drawPixmap(iconRect, pixmap);
    }

    // Text position - sub icon
    QString btnText = text();
    if (!btnText.isEmpty())
    {
        int textY = iconY + m_iconSize.height() + gap;
        int textHeight = h - textY - bottomPadding;

        if (textHeight > 0)
        {
            QRect textRect(8, textY, w - 16, textHeight);

            QFont font = painter.font();
            font.setPixelSize(16);
            font.setWeight(QFont::Medium);
            painter.setFont(font);

            painter.setPen(QColor("#1F2A44"));
            painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignTop, btnText);
        }
    }
}
