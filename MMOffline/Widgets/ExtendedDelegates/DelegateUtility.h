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