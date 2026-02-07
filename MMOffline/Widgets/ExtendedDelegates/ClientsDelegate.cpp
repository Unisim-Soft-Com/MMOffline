#include "ClientsDelegate.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QApplication>
#include <QAbstractItemView>
#include "Dataprovider/DataEntities.h"
#include "DelegateUtility.h"

ClientsDelegate::ClientsDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
    , m_reportButtonHovered(false)
{
}

QRect ClientsDelegate::reportButtonRect(const QStyleOptionViewItem& option) const
{
    // Button on the right side of the card
    int btnHeight = option.rect.height() - CARD_MARGIN * 2 - CARD_PADDING * 2;
    int btnWidth = REPORT_BTN_WIDTH;
    int btnX = option.rect.right() - CARD_MARGIN - CARD_PADDING - btnWidth;
    int btnY = option.rect.top() + CARD_MARGIN + CARD_PADDING;

    // Limit button height
    if (btnHeight > 50) btnHeight = 50;

    // Center vertically
    btnY = option.rect.top() + (option.rect.height() - btnHeight) / 2;

    return QRect(btnX, btnY, btnWidth, btnHeight);
}

void ClientsDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                            const QModelIndex& index) const
{
    // Get Client entity
    Client client = upcastItem<ClientEntity>(index);
    if (client == nullptr) {
        return;
    }

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // === Card dimensions ===
    QRect cardRect = option.rect.adjusted(CARD_MARGIN, CARD_MARGIN/2, -CARD_MARGIN, -CARD_MARGIN/2);

    // === Draw card shadow (subtle) ===
    QPainterPath shadowPath;
    shadowPath.addRoundedRect(cardRect.adjusted(2, 2, 2, 2), CARD_RADIUS, CARD_RADIUS);
    painter->fillPath(shadowPath, QColor(0, 0, 0, 20));

    // === Draw card background ===
    QPainterPath cardPath;
    cardPath.addRoundedRect(cardRect, CARD_RADIUS, CARD_RADIUS);
    painter->fillPath(cardPath, QColor("#FFFFFF"));

    // Card border - blue if selected, light gray otherwise
    QPen borderPen(option.state & QStyle::State_Selected ? QColor("#1976D2") : QColor("#E0E0E0"));
    borderPen.setWidth(option.state & QStyle::State_Selected ? 2 : 1);
    painter->setPen(borderPen);
    painter->drawPath(cardPath);

    // === Content area (left side, leaving space for report button) ===
    int contentLeft = cardRect.left() + CARD_PADDING;
    int contentTop = cardRect.top() + CARD_PADDING;
    int contentWidth = cardRect.width() - CARD_PADDING * 3 - REPORT_BTN_WIDTH;

    // === Draw ID badge ===
    QString idText = QString::number(client->id);
    QFont badgeFont = painter->font();
    badgeFont.setPixelSize(12);
    badgeFont.setWeight(QFont::DemiBold);
    painter->setFont(badgeFont);

    QFontMetrics badgeFm(badgeFont);
    int badgeTextWidth = badgeFm.horizontalAdvance(idText);
    int badgeWidth = badgeTextWidth + 16;
    int badgeHeight = 22;

    QRect badgeRect(contentLeft, contentTop, badgeWidth, badgeHeight);

    // Badge background (blue gradient)
    QPainterPath badgePath;
    badgePath.addRoundedRect(badgeRect, badgeHeight/2, badgeHeight/2);
    QLinearGradient badgeGradient(badgeRect.topLeft(), badgeRect.bottomLeft());
    badgeGradient.setColorAt(0, QColor("#1976D2"));
    badgeGradient.setColorAt(1, QColor("#1565C0"));
    painter->fillPath(badgePath, badgeGradient);

    // Badge text
    painter->setPen(QColor("#FFFFFF"));
    painter->drawText(badgeRect, Qt::AlignCenter, idText);

    // === Check for quantity ===
    bool hasQuantity = false;
    int qty = index.data(DataEntityListModel::QuantityView).toInt(&hasQuantity);
    hasQuantity = hasQuantity && (qty != 0);

    if (hasQuantity) {
        // Draw quantity badge next to ID
        QString qtyText = tr("QTY: %1").arg(qty);
        int qtyBadgeWidth = badgeFm.horizontalAdvance(qtyText) + 16;
        QRect qtyBadgeRect(contentLeft + badgeWidth + 8, contentTop, qtyBadgeWidth, badgeHeight);

        QPainterPath qtyBadgePath;
        qtyBadgePath.addRoundedRect(qtyBadgeRect, badgeHeight/2, badgeHeight/2);
        painter->fillPath(qtyBadgePath, QColor("#E3F2FD"));

        painter->setPen(QColor("#1976D2"));
        painter->drawText(qtyBadgeRect, Qt::AlignCenter, qtyText);
    }

    // === Draw client name ===
    QFont nameFont = painter->font();
    nameFont.setPixelSize(15);
    nameFont.setWeight(QFont::DemiBold);
    painter->setFont(nameFont);
    painter->setPen(QColor("#333333"));

    int nameTop = contentTop + badgeHeight + 10;
    QRect nameRect(contentLeft, nameTop, contentWidth, 20);
    QString elidedName = QFontMetrics(nameFont).elidedText(client->name, Qt::ElideRight, contentWidth);
    painter->drawText(nameRect, Qt::AlignLeft | Qt::AlignVCenter, elidedName);

    // === Draw address (if present in name after |) ===
    // Parse name - usually format is "Name| Address| Phone"
    QStringList parts = client->name.split("|");
    if (parts.size() > 1) {
        // Redraw name (first part only)
        QString pureName = parts[0].trimmed();
        elidedName = QFontMetrics(nameFont).elidedText(pureName, Qt::ElideRight, contentWidth);
        painter->drawText(nameRect, Qt::AlignLeft | Qt::AlignVCenter, elidedName);

        // Draw address
        QFont addressFont = painter->font();
        addressFont.setPixelSize(13);
        addressFont.setWeight(QFont::Normal);
        painter->setFont(addressFont);
        painter->setPen(QColor("#666666"));

        QString address = parts.mid(1).join(" | ").trimmed();
        int addressTop = nameTop + 24;
        QRect addressRect(contentLeft, addressTop, contentWidth, 18);
        QString elidedAddress = QFontMetrics(addressFont).elidedText(address, Qt::ElideRight, contentWidth);
        painter->drawText(addressRect, Qt::AlignLeft | Qt::AlignVCenter, elidedAddress);
    }

    // === Draw Report button ===
    QRect btnRect = reportButtonRect(option);

    // Check if this button is hovered
    bool isHovered = (m_hoveredIndex == index && m_reportButtonHovered);

    // Button background
    QPainterPath btnPath;
    btnPath.addRoundedRect(btnRect, 8, 8);

    if (isHovered) {
        painter->fillPath(btnPath, QColor("#E8F5E9"));
    } else {
        painter->fillPath(btnPath, QColor("#FFFFFF"));
    }

    // Button border (green outline)
    QPen btnBorderPen(QColor("#43A047"));
    btnBorderPen.setWidth(2);
    painter->setPen(btnBorderPen);
    painter->drawPath(btnPath);

    // Button icon (simple document icon drawn manually)
    painter->setPen(QPen(QColor("#43A047"), 1.5));
    int iconSize = 18;
    int iconX = btnRect.center().x() - iconSize/2;
    int iconY = btnRect.top() + 8;

    // Draw document shape
    QRect iconRect(iconX, iconY, iconSize, iconSize);
    painter->drawRect(iconRect.adjusted(2, 0, -2, 0));
    painter->drawLine(iconRect.left() + 5, iconRect.top() + 5, iconRect.right() - 5, iconRect.top() + 5);
    painter->drawLine(iconRect.left() + 5, iconRect.top() + 9, iconRect.right() - 5, iconRect.top() + 9);
    painter->drawLine(iconRect.left() + 5, iconRect.top() + 13, iconRect.right() - 7, iconRect.top() + 13);

    // Button text
    QFont btnFont = painter->font();
    btnFont.setPixelSize(10);
    btnFont.setWeight(QFont::DemiBold);
    painter->setFont(btnFont);
    painter->setPen(QColor("#43A047"));

    QRect btnTextRect(btnRect.left(), iconY + iconSize + 2, btnRect.width(), 14);
    painter->drawText(btnTextRect, Qt::AlignCenter, tr("Report"));

    painter->restore();
}

QSize ClientsDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    // Fixed height for modern card design
    return QSize(100, 100);
}

bool ClientsDelegate::editorEvent(QEvent* event, QAbstractItemModel* model,
                                  const QStyleOptionViewItem& option,
                                  const QModelIndex& index)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QRect btnRect = reportButtonRect(option);

        if (btnRect.contains(mouseEvent->pos())) {
            // Report button was clicked
            emit reportClicked(index);
            return true;
        }
    }
    else if (event->type() == QEvent::MouseMove) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QRect btnRect = reportButtonRect(option);

        bool wasHovered = m_reportButtonHovered && (m_hoveredIndex == index);
        m_reportButtonHovered = btnRect.contains(mouseEvent->pos());
        m_hoveredIndex = index;

        if (wasHovered != m_reportButtonHovered) {
            // Request repaint for hover effect
            if (QAbstractItemView* view = qobject_cast<QAbstractItemView*>(
                    const_cast<QWidget*>(option.widget))) {
                view->update(index);
            }
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
