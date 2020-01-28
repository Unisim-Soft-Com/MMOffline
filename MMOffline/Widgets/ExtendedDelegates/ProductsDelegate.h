#pragma once
#include <QtWidgets/qstyleditemdelegate.h>


/*
	This delegate is suited for drawing Product's. It creates similar with entryDelegate layout,
	but outlines borders and requires QuantityRole to count how much products were included.
*/
class ProductsDelegate : public QStyledItemDelegate
{
	Q_OBJECT
protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index)const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
public:
	ProductsDelegate(QObject* parent) : QStyledItemDelegate(parent) {};
};