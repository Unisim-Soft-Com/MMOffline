#include "ModernDocumentsDelegate.h"
#include <QPainter>
#include <QPainterPath>
#include "DelegateUtility.h"
#include "Dataprovider/DataEntities.h"

ModernDocumentsDelegate::ModernDocumentsDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

QColor ModernDocumentsDelegate::avatarColorForName(const QString& name) const
{
    static const QColor colors[] = {
        QColor("#1565C0"), QColor("#7B1FA2"), QColor("#C62828"),
        QColor("#00838F"), QColor("#2E7D32"), QColor("#E65100"),
        QColor("#4527A0"), QColor("#AD1457")
    };
    uint hash = 0;
    for (const QChar& c : name)
        hash = hash * 31 + c.unicode();
    return colors[hash % 8];
}

void ModernDocumentsDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                                    const QModelIndex& index) const
{
    Document d = upcastItem<DocumentEntity>(index);
    if (d == nullptr)
        return;

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // ==================== Parse client name ====================
    // Format: "Name| Address| Phone" or just "Name"
    QStringList parts = d->clientName.split("|");
    QString clientDisplayName = parts.isEmpty() ? d->clientName : parts[0].trimmed();
    QString clientAddress;
    if (parts.size() > 1)
        clientAddress = parts.mid(1).join(" | ").trimmed();

    // First letter for avatar
    QChar avatarLetter = clientDisplayName.isEmpty() ? QChar('?') : clientDisplayName.at(0).toUpper();
    QColor avatarColor = avatarColorForName(clientDisplayName);

    // ==================== Card background ====================
    QRect cardRect = option.rect.adjusted(CARD_MARGIN, CARD_MARGIN / 2, -CARD_MARGIN, -CARD_MARGIN / 2);

    // Shadow
    QPainterPath shadowPath;
    shadowPath.addRoundedRect(cardRect.adjusted(1, 2, 1, 2), CARD_RADIUS, CARD_RADIUS);
    painter->fillPath(shadowPath, QColor(0, 0, 0, 20));

    // Card fill
    QPainterPath cardPath;
    cardPath.addRoundedRect(cardRect, CARD_RADIUS, CARD_RADIUS);
    painter->fillPath(cardPath, QColor("#FFFFFF"));

    // Card border
    QPen borderPen(option.state & QStyle::State_Selected ? QColor("#1976D2") : QColor("#E8EDF2"));
    borderPen.setWidth(option.state & QStyle::State_Selected ? 2 : 1);
    painter->setPen(borderPen);
    painter->drawPath(cardPath);

    // ==================== Header section: avatar + name + address ====================
    int contentLeft = cardRect.left() + CARD_PADDING;
    int contentRight = cardRect.right() - CARD_PADDING;
    int contentWidth = contentRight - contentLeft;
    int yPos = cardRect.top() + CARD_PADDING;

    // Draw avatar circle
    QRect avatarRect(contentLeft, yPos, AVATAR_SIZE, AVATAR_SIZE);
    QPainterPath avatarPath;
    avatarPath.addEllipse(avatarRect);

    QLinearGradient avatarGrad(avatarRect.topLeft(), avatarRect.bottomRight());
    avatarGrad.setColorAt(0, avatarColor);
    avatarGrad.setColorAt(1, avatarColor.darker(120));
    painter->fillPath(avatarPath, avatarGrad);

    // Avatar letter
    QFont avatarFont = painter->font();
    avatarFont.setPixelSize(17);
    avatarFont.setWeight(QFont::Bold);
    painter->setFont(avatarFont);
    painter->setPen(QColor("#FFFFFF"));
    painter->drawText(avatarRect, Qt::AlignCenter, QString(avatarLetter));

    // Client name
    int textLeft = contentLeft + AVATAR_SIZE + 12;
    int textWidth = contentRight - textLeft;

    QFont nameFont = painter->font();
    nameFont.setPixelSize(15);
    nameFont.setWeight(QFont::DemiBold);
    painter->setFont(nameFont);
    painter->setPen(QColor("#1A2332"));

    QRect nameRect(textLeft, yPos + 2, textWidth, 20);
    QString elidedName = QFontMetrics(nameFont).elidedText(clientDisplayName, Qt::ElideRight, textWidth);
    painter->drawText(nameRect, Qt::AlignLeft | Qt::AlignVCenter, elidedName);

    // Client address
    if (!clientAddress.isEmpty())
    {
        QFont addrFont = painter->font();
        addrFont.setPixelSize(12);
        addrFont.setWeight(QFont::Normal);
        painter->setFont(addrFont);
        painter->setPen(QColor("#64748B"));

        QRect addrRect(textLeft, yPos + 22, textWidth, 16);
        QString elidedAddr = QFontMetrics(addrFont).elidedText(clientAddress, Qt::ElideRight, textWidth);
        painter->drawText(addrRect, Qt::AlignLeft | Qt::AlignVCenter, elidedAddr);
    }

    // ==================== Separator line ====================
    int separatorY = yPos + AVATAR_SIZE + SECTION_GAP;
    painter->setPen(QPen(QColor("#F1F5F9"), 1));
    painter->drawLine(contentLeft, separatorY, contentRight, separatorY);

    // ==================== Details section ====================
    int detailsY = separatorY + SECTION_GAP;
    int halfWidth = contentWidth / 2;

    // --- Label font ---
    QFont labelFont = painter->font();
    labelFont.setPixelSize(10);
    labelFont.setWeight(QFont::DemiBold);
    labelFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);

    // --- Value font ---
    QFont valueFont = painter->font();
    valueFont.setPixelSize(13);
    valueFont.setWeight(QFont::Medium);

    // --- Large value font (for ACH amount) ---
    QFont achFont = painter->font();
    achFont.setPixelSize(15);
    achFont.setWeight(QFont::Bold);

    // ===== Row 1: Created + Paid =====

    // Label "CREAT"
    painter->setFont(labelFont);
    painter->setPen(QColor("#94A3B8"));
    QRect createdLabelRect(contentLeft, detailsY, halfWidth, 14);
    painter->drawText(createdLabelRect, Qt::AlignLeft | Qt::AlignBottom, tr("CREATED"));

    // Label "ACHITAT" (right-aligned)
    QRect achLabelRect(contentLeft + halfWidth, detailsY, halfWidth, 14);
    painter->drawText(achLabelRect, Qt::AlignRight | Qt::AlignBottom, tr("PAID"));

    // Value: creation datetime
    int row1ValueY = detailsY + 16;
    painter->setFont(valueFont);
    painter->setPen(QColor("#334155"));
    QRect createdValRect(contentLeft, row1ValueY, halfWidth, 18);
    painter->drawText(createdValRect, Qt::AlignLeft | Qt::AlignVCenter,
                      d->dateWhenCreated.toString(QStringLiteral("dd.MM / HH:mm")));

    // Value: already paid (bold blue)
    painter->setFont(achFont);
    painter->setPen(QColor("#1565C0"));
    QRect achValRect(contentLeft + halfWidth, row1ValueY, halfWidth, 18);
    QString achText = QString::number(d->alreadyPaid, 'f',
                                      (d->alreadyPaid == (int)d->alreadyPaid) ? 0 : 2);
    painter->drawText(achValRect, Qt::AlignRight | Qt::AlignVCenter, achText + " MDL");

    // ===== Row 2: Shipping date + Document code =====
    int row2Y = row1ValueY + 24;

    // Label "DATA LIVRĂRII"
    painter->setFont(labelFont);
    painter->setPen(QColor("#94A3B8"));
    QRect shipLabelRect(contentLeft, row2Y, halfWidth, 14);
    painter->drawText(shipLabelRect, Qt::AlignLeft | Qt::AlignBottom, tr("DELIVERY DATE"));

    // Label "COD"
    QRect codLabelRect(contentLeft + halfWidth, row2Y, halfWidth, 14);
    painter->drawText(codLabelRect, Qt::AlignRight | Qt::AlignBottom, tr("CODE"));

    // Value: shipping date (with blue badge)
    int row2ValueY = row2Y + 16;

    // Draw date badge background
    QString shipDateStr = d->shippingDate.toString(QStringLiteral("dd.MM.yyyy"));
    QFont dateFont = valueFont;
    dateFont.setWeight(QFont::DemiBold);
    painter->setFont(dateFont);
    int dateTextWidth = QFontMetrics(dateFont).horizontalAdvance(shipDateStr) + 16;
    int badgeHeight = 22;
    QRect dateBadgeRect(contentLeft, row2ValueY, dateTextWidth, badgeHeight);

    QPainterPath badgePath;
    badgePath.addRoundedRect(dateBadgeRect, badgeHeight / 2, badgeHeight / 2);
    painter->fillPath(badgePath, QColor("#E8F0FE"));

    painter->setPen(QColor("#1565C0"));
    painter->drawText(dateBadgeRect, Qt::AlignCenter, shipDateStr);

    // Value: document code (hex, 6 chars)
    QFont codeFont = painter->font();
    codeFont.setPixelSize(12);
    codeFont.setWeight(QFont::Normal);
    codeFont.setFamily("monospace");
    painter->setFont(codeFont);
    painter->setPen(QColor("#94A3B8"));

    QString codeStr = QString::number(d->documentId, 16).right(6);

    // Code background
    int codeTextWidth = QFontMetrics(codeFont).horizontalAdvance(codeStr) + 14;
    QRect codeBgRect(contentRight - codeTextWidth, row2ValueY + 1, codeTextWidth, 20);
    QPainterPath codeBgPath;
    codeBgPath.addRoundedRect(codeBgRect, 6, 6);
    painter->fillPath(codeBgPath, QColor("#F8FAFC"));

    painter->drawText(codeBgRect, Qt::AlignCenter, codeStr);

    // ==================== Selection overlay ====================
    if (option.state & QStyle::State_Selected)
    {
        painter->setOpacity(0.12);
        painter->fillPath(cardPath, QColor("#1976D2"));
    }

    painter->restore();
}

QSize ModernDocumentsDelegate::sizeHint(const QStyleOptionViewItem& option,
                                        const QModelIndex& index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    // Fixed height: padding(14) + avatar(40) + gap(10) + row1(34) + row2(38) + padding(14) + margins(8)
    return QSize(100, 166);
}
