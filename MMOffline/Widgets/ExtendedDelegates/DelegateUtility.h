#pragma once
#include <QStyleOptionViewItem>

inline QSize countOptimalSizeOfItem(const QStyleOptionViewItem& option)
{
	return QSize(option.rect.width(), option.fontMetrics.height() * 4);
}
inline QSize countWrappingBorder(const QString& str, const QStyleOptionViewItem& option)
{
	return QSize(option.fontMetrics.width(str) + 6, option.fontMetrics.height() + 6);
}
template <class DataEntSub>
std::shared_ptr<DataEntSub> upcastItem(const QModelIndex& index)
{
	std::shared_ptr<DataEntSub> c = index.data().value<std::shared_ptr<DataEntSub> >();
	if (c == nullptr)
	{
		c = std::static_pointer_cast<DataEntSub>(index.data().value<DataEntity>());
	}
	if (c == nullptr)
	{
		c = (qobject_cast<const QSortFilterProxyModel*>(index.model()))->mapToSource(index).data().value<std::shared_ptr<DataEntSub> >();
	}
	if (c == nullptr)
	{
		c = std::static_pointer_cast<DataEntSub>(
			(qobject_cast<const QSortFilterProxyModel*>(
				index.model()))->mapToSource(index).data().value<std::shared_ptr<DataEntSub> >());
	}
	return c;
}