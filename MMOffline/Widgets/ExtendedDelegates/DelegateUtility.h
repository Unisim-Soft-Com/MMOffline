#pragma once
#include <QStyleOptionViewItem>
#include <QtGui/QPainter>
#include <Dataprovider/DataEntities.h>

inline void setGradient(const QRect* r, QLinearGradient* g, const QColor& a, const QColor& b)
{
	g->setColorAt(0, a);
	g->setColorAt(1, b);
	g->setStart(r->topLeft());
	g->setFinalStop(r->bottomLeft());
}
inline void drawRect(const QRect& r, const QLinearGradient& gr, QPainter* p)
{
	p->setBrush(gr);
	p->setOpacity(0.6);
	p->drawRect(r);
	p->setOpacity(1);
}
inline void drawRect(const QRect& r, const QColor& c, QPainter* p)
{
	p->setBrush(c);
	p->drawRect(r);
}
inline void drawRect(const QRect& re, const int r, const int g, const int b, QPainter* p)
{
	p->setBrush(QColor(r, g, b));
	p->drawRect(re);
}

extern const QColor bright_delegate_color;
extern const QColor dark_delegate_color;

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
