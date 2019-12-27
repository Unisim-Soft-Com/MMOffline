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
	bool QuantityGiven = false;
	int qty = index.data(DataEntityListModel::QuantityView).toInt(&QuantityGiven);
	QuantityGiven = (QuantityGiven) ? qty != 0 : QuantityGiven;
	QLinearGradient gr;
	painter->save();
	gr.setColorAt(0, Qt::red);
	gr.setColorAt(1, option.palette.highlight().color());
	QRect textbox(option.rect.topLeft(), QSize(
		((QuantityGiven) ? 
		option.rect.width()/2 : option.rect.width()), option.fontMetrics.height() + 6));
	gr.setStart(textbox.topLeft());
	gr.setFinalStop(textbox.bottomLeft());
	painter->setPen(Qt::PenStyle::SolidLine);
	painter->setBrush(gr);
	painter->setOpacity(0.6);
	painter->drawRect(textbox);
	painter->setOpacity(1);
	painter->drawText(textbox, Qt::AlignCenter, QString::number(temp->id));
	if (QuantityGiven)
	{
		textbox.setTopLeft(textbox.topRight());
		textbox.setBottomRight(QPoint(option.rect.bottomRight().x(), option.rect.topRight().y() + option.fontMetrics.height()+6));
		gr.setColorAt(0, option.palette.dark().color());
		gr.setColorAt(1, option.palette.highlight().color());
		gr.setStart(textbox.topLeft());
		gr.setFinalStop(textbox.bottomLeft());
		painter->setBrush(gr);
		painter->setOpacity(0.6);
		painter->drawRect(textbox);
		painter->setOpacity(1);
		painter->drawText(textbox, Qt::AlignCenter, tr("Quantity: ") + QString::number(qty));
	}
	textbox.setTopRight(textbox.bottomRight());
	textbox.setBottomLeft(option.rect.bottomLeft());
	gr.setColorAt(0, option.palette.highlight().color());
	gr.setColorAt(1, option.palette.brightText().color());
	gr.setStart(textbox.topLeft());
	gr.setFinalStop(textbox.bottomLeft());
	painter->setBrush(gr);
	painter->setOpacity(0.6);
	painter->drawRect(textbox);
	painter->setOpacity(1);
	painter->drawText(textbox, Qt::TextWordWrap | Qt::AlignCenter, temp->name);

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
	return QSize(option.rect.width(), (tabs + 1.8) * (option.fontMetrics.height()+2) );
}
