#pragma once
#include <QtWidgets/qstyleditemdelegate.h>
#include <QSize>

/*
    Modern card-style delegate for DocumentEntity display.
    Features:
    - Card with rounded corners and subtle shadow
    - Client avatar (colored circle with initial letter)
    - Client name and address (parsed from clientName)
    - Creation datetime and paid amount
    - Shipping date and document code (hex)

    Replaces the old DocumentsDelegate with modern card-based design
    consistent with ClientsDelegate styling.
*/

class ModernDocumentsDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ModernDocumentsDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;

private:
    // Generate a consistent avatar color based on client name
    QColor avatarColorForName(const QString& name) const;

    // Layout constants
    static const int CARD_MARGIN = 8;
    static const int CARD_RADIUS = 14;
    static const int CARD_PADDING = 14;
    static const int AVATAR_SIZE = 40;
    static const int SECTION_GAP = 10;
};
