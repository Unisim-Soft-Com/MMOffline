#include "GroupDelegate.h"
#include <QPainter>
#include <QtCore/QModelIndex>


void GroupDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QString temp = index.data(Qt::DisplayRole).toString();
	if (temp.isEmpty())
	{
		painter->save();
		painter->setBrush(option.palette.brightText());
		painter->setPen(Qt::NoPen);
		painter->drawRect(option.rect);
		painter->restore();
		return;
	}
	else
	{
		QLinearGradient gr;
		painter->save();
		gr.setColorAt(0, Qt::lightGray);
		gr.setColorAt(1, option.palette.highlight().color());
		gr.setStart(option.rect.topLeft());
		gr.setFinalStop(option.rect.bottomLeft());
		painter->setBrush(gr);
		painter->setPen(Qt::SolidLine);
		painter->drawRect(option.rect);
		painter->drawText(option.rect, Qt::TextWordWrap|Qt::AlignCenter, temp);
		if (option.state.testFlag(QStyle::State_Selected))
		{
			painter->setBrush(option.palette.highlight());
			painter->setOpacity(0.3);
			painter->drawRect(option.rect);
		}
		painter->restore();
		return;
	}
}

QSize GroupDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QSize(option.rect.width(), ( 4.1) * option.fontMetrics.height());
}

GroupDelegate::GroupDelegate(QObject* parent)
	:QStyledItemDelegate(parent)
{
}
