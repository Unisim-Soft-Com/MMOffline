#include "ClientsDelegate.h"
#include <QPainter>
#include "Dataprovider/DataEntities.h"
#include "debugtrace.h"
#include "qsortfilterproxymodel.h"
#include "DelegateUtility.h"
#include <cmath>

void ClientsDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	// get Client
	Client temp = upcastItem<ClientEntity>(index);
	if (temp == nullptr)
	{
		return;
	}
	// try to get quantity
	bool QuantityGiven = false;
	int qty = index.data(DataEntityListModel::QuantityView).toInt(&QuantityGiven);
	QuantityGiven = (QuantityGiven) ? qty != 0 : QuantityGiven;
	
	// painting begins
	painter->save();
	// drawing first textbox
	QRect textbox(option.rect.topLeft(), QSize(
		((QuantityGiven) ?
			option.rect.width() / 2 : option.rect.width()), option.fontMetrics.height() + 6));
	painter->setPen(Qt::PenStyle::SolidLine);
	painter->setBrush(dark_delegate_color);
	painter->drawRect(textbox);
	painter->drawText(textbox, Qt::AlignCenter, QString::number(temp->id));
	if (QuantityGiven)
		// if quantity given - draw second textbox
	{
		textbox.setTopLeft(textbox.topRight());
		textbox.setBottomRight(QPoint(option.rect.bottomRight().x(), option.rect.topRight().y() + option.fontMetrics.height() + 6));
		painter->setBrush(dark_delegate_color);
		painter->drawRect(textbox);
		painter->drawText(textbox, Qt::AlignCenter, tr("Quantity:") + QString::number(qty));
	}
	// draw bottom textbox
	textbox.setTopRight(textbox.bottomRight());
	textbox.setBottomLeft(option.rect.bottomLeft());
	painter->setBrush(bright_delegate_color);
	painter->drawRect(textbox);
	painter->drawText(textbox, Qt::TextWordWrap | Qt::AlignCenter, temp->name);

	// draw selection
	if (option.state.testFlag(QStyle::State_Selected))
	{
		painter->setBrush(option.palette.highlight());
		painter->setOpacity(0.4);
		painter->drawRect(option.rect);
	}
	painter->restore();
}

QSize ClientsDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	// final size - 1.8 * font height for top panel + enough space to fully wrap text
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
	return QSize(option.rect.width(), (tabs + 1.8) * (option.fontMetrics.height() + 2));
}