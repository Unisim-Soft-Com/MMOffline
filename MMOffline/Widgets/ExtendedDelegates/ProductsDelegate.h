#pragma once
#include <QtWidgets/qstyleditemdelegate.h>

class ProductsDelegate : public QStyledItemDelegate
{
	Q_OBJECT
protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index)const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
public:
	ProductsDelegate(QObject* parent) : QStyledItemDelegate(parent) {};
};