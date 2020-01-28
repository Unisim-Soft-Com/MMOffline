#pragma once
#include <QtWidgets/qstyleditemdelegate.h>

/*
	This delegate is oriented for work with ClientEntity. It expects two roles from source model:
		QuantityView role - must return number to show as quantity. Can return nothing.
		Display role   - must return Client pointer to display it, or at least castable DataEntity.
		Normal delegate view provides two colorized panels with id and name, and quantity if provided.
*/

class ClientsDelegate : public QStyledItemDelegate
{
	Q_OBJECT
protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index)const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
public:
	ClientsDelegate(QObject* parent) : QStyledItemDelegate(parent) {};
};