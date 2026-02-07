#pragma once
#include <QtWidgets/qstyleditemdelegate.h>
#include <QSize>

/*
    Modern card-style delegate for ClientEntity display.
    Features:
    - Card with rounded corners and shadow effect
    - Client ID badge (blue)
    - Client name and address
    - Report button (green outline) on the right side

    Emits reportClicked signal through the model when Report button is pressed.
*/

class ClientsDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ClientsDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;

    // Handle mouse events for the Report button
    bool editorEvent(QEvent* event, QAbstractItemModel* model,
                     const QStyleOptionViewItem& option,
                     const QModelIndex& index) override;

signals:
    void reportClicked(const QModelIndex& index);

private:
    // Calculate the Report button rectangle
    QRect reportButtonRect(const QStyleOptionViewItem& option) const;

    // Track hover state for report button
    mutable QModelIndex m_hoveredIndex;
    mutable bool m_reportButtonHovered;

    // Constants for layout
    static const int CARD_MARGIN = 8;
    static const int CARD_RADIUS = 12;
    static const int CARD_PADDING = 16;
    static const int REPORT_BTN_WIDTH = 65;
    static const int REPORT_BTN_MARGIN = 10;
};
