#pragma once
#include <QtWidgets/qstyleditemdelegate.h>

/*
	This delegate is used for creating document views.It does not expects any role except
	DisplayRole. This delegate is creating 3 panels with document description.
*/

class DocumentsDelegate : public QStyledItemDelegate
{
	Q_OBJECT
protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index)const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
public:
	DocumentsDelegate(QObject* parent) : QStyledItemDelegate(parent) {};
};