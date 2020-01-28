#pragma once
#include <QtWidgets/qstyleditemdelegate.h>

/*
	This delegate is suited for entry displaying. It draws two panels, one of which is splitted into 3 
	columns. This delegate still expects DataEntity or DocumentEntryEntity
*/

class EntryDelegate : public QStyledItemDelegate
{
	Q_OBJECT
protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index)const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
public:
	EntryDelegate(QObject* parent) : QStyledItemDelegate(parent) {};
};