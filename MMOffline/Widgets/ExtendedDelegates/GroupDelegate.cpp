#include "GroupDelegate.h"
#include <QPainter>
#include <QtCore/QModelIndex>
#include "DelegateUtility.h"

void GroupDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	// trying to obtain group name
	QString temp = index.data(Qt::DisplayRole).toString();
	if (temp.isEmpty())
	{
		// draw empty rectangle
		painter->save();
		painter->setBrush(option.palette.brightText());
		painter->setPen(Qt::NoPen);
		painter->drawRect(option.rect);
		painter->restore();
		return;
	}
	else
	{
		// draw bright rectangle with text inside
		painter->save();
		painter->setBrush(bright_delegate_color);
		painter->setPen(Qt::SolidLine);
		painter->drawRect(option.rect);
		painter->drawText(option.rect, Qt::TextWordWrap | Qt::AlignCenter, temp);
		// draw selection
		if (option.state.testFlag(QStyle::State_Selected))
		{
			painter->setBrush(option.palette.highlight());
			painter->setOpacity(0.4);
			painter->drawRect(option.rect);
		}
		painter->restore();
		return;
	}
}

QSize GroupDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	// final size 4.1 * font height. It does not change, to imitate real buttons
	return QSize(option.rect.width(), (4.1) * option.fontMetrics.height());
}

GroupDelegate::GroupDelegate(QObject* parent)
	:QStyledItemDelegate(parent)
{
}