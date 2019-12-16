#include "ClientsDelegate.h"
#include <QPainter>
#include "Dataprovider/DataEntities.h"
#include "debugtrace.h"


void ClientsDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Client temp = index.data().value<Client>();
	if (temp == nullptr)
	{
		temp = std::static_pointer_cast<ClientEntity>(index.data().value<DataEntity>());
		if (temp == nullptr)
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
	Client c = index.data().value<Client>();
	if (c == nullptr)
	{
		c = std::static_pointer_cast<ClientEntity>(index.data().value<DataEntity>());
		if (c == nullptr)
			return QSize(100, 50);
	}
	double wdth = option.fontMetrics.averageCharWidth() * c->name.length();
	int tabs = 0;
	if (option.rect.width() == 0)
	{
		tabs = wdth;
	}
	else
		tabs = std::ceil(wdth / option.rect.width());
	tabs = (tabs > 0) ? tabs : 1;
	detrace_METHEXPL("for width " << wdth << " decided to make " << tabs << " tabs comparing with width " << option.rect.width());
	return QSize(option.rect.width(), (tabs + 1.4) * (option.fontMetrics.height()+2) );
}
