#include "ClientsDelegate.h"
#include <QPainter>
#include "Dataprovider/DataEntities.h"
#include "debugtrace.h"
#include "qsortfilterproxymodel.h"
#include "DelegateUtility.h"

void ClientsDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Client temp = upcastItem<ClientEntity>(index);
	if (temp == nullptr)
	{
		return;
	}
	QLinearGradient gr;
	painter->save();
	gr.setColorAt(0, Qt::red);
	gr.setColorAt(1, option.palette.highlight().color());
	QRect textbox(option.rect.topLeft(), QSize(option.rect.width(), option.fontMetrics.height() + 6));
	gr.setStart(textbox.topLeft());
	gr.setFinalStop(textbox.bottomLeft());
	painter->setPen(Qt::PenStyle::SolidLine);
	painter->setBrush(gr);
	painter->setOpacity(0.6);
	painter->drawRect(textbox);
	painter->setOpacity(1);
	painter->drawText(textbox, Qt::AlignCenter, QString::number(temp->id));
	textbox.setTopLeft(textbox.bottomLeft());
	textbox.setBottomRight(option.rect.bottomRight());
	gr.setColorAt(0, option.palette.highlight().color());
	gr.setColorAt(1, option.palette.brightText().color());
	gr.setStart(textbox.topLeft());
	gr.setFinalStop(textbox.bottomLeft());
	painter->setBrush(gr);
	painter->setOpacity(0.6);
	painter->drawRect(textbox);
	painter->setOpacity(1);
	painter->drawText(textbox, Qt::TextWordWrap | Qt::AlignCenter, temp->name);

	if (option.state.testFlag(QStyle::State_Selected))
	{
		painter->setBrush(option.palette.highlight());
		painter->setOpacity(0.3);
		painter->drawRect(option.rect);
	}

	painter->restore();
}


QSize ClientsDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Client c = upcastItem<ClientEntity>(index);
	if (c == nullptr)
		return QSize(100, 50);
	double wdth = option.fontMetrics.averageCharWidth() * c->name.length();
	int tabs = 0;
	if (option.rect.width() == 0)
	{
		tabs = wdth;
	}
	else
		tabs = std::ceil(wdth / option.rect.width());
	tabs = (tabs > 0) ? tabs : 1;
	return QSize(option.rect.width(), (tabs + 1.4) * (option.fontMetrics.height()+2) );
}
