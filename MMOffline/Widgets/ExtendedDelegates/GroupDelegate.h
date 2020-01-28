#pragma once
#include <QtWidgets/QStyledItemDelegate>

/*
	This delegate is simpliest from extended delegates. It draws large button-like panel.
*/

class GroupDelegate : public QStyledItemDelegate
{
	Q_OBJECT
protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index)const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
public:
	GroupDelegate(QObject* parent);
};