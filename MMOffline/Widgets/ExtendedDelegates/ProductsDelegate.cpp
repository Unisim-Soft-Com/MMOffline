#include "ProductsDelegate.h"
#include <QPainter>
#include "Dataprovider/DataEntities.h"
#include "DelegateUtility.h"

void ProductsDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Product temp = index.data().value<Product>();
	if (temp == nullptr)
	{
		temp = std::static_pointer_cast<ProductEntity>(index.data().value<DataEntity>());
		if (temp == nullptr)
			return;
	}
	// predrawing calculations
	QPoint diff(option.rect.bottomRight().x() * 0.333333, 0);
	QRect textbox(
		option.rect.topLeft(),
		QPoint(
			diff.x(),
			option.rect.topLeft().y() + option.fontMetrics.height() * 1.1
		));
	QLinearGradient gr(textbox.topLeft(), textbox.bottomLeft());
	gr.setColorAt(0, Qt::red);
	gr.setColorAt(1, option.palette.highlight().color());

	painter->save();
	//	Id box
	painter->setOpacity(0.6);
	painter->setBrush(gr);
	painter->drawRect(textbox);
	painter->setOpacity(1);
	painter->setPen(Qt::PenStyle::SolidLine);
	painter->drawText(
		textbox
		, Qt::AlignCenter, QString::number(temp->id));
	// Price box
	textbox.setBottomRight(textbox.bottomRight() + diff);
	textbox.setTopLeft(textbox.topLeft() + diff);
	gr.setStart(textbox.topLeft());
	gr.setFinalStop(textbox.bottomLeft());
	gr.setColorAt(0, option.palette.brightText().color());
	gr.setColorAt(1, option.palette.highlight().color());
	painter->setBrush(gr);
	painter->setOpacity(0.5);
	painter->drawRect(textbox);
	painter->setPen(Qt::SolidLine);
	painter->setOpacity(1);
	painter->drawText(textbox, Qt::AlignCenter, tr("Price: ") + temp->price);
	// Quantity box
	textbox.setTopLeft(
		textbox.topLeft() + diff
	);
	textbox.setBottomRight(
		textbox.bottomRight() + diff
	);
	gr.setStart(textbox.topLeft());
	gr.setFinalStop(textbox.bottomLeft());
	painter->setOpacity(0.5);
	painter->drawRect(textbox);
	painter->setPen(Qt::SolidLine);
	painter->setOpacity(1);
	painter->drawText(textbox, Qt::AlignCenter, tr("Quantity: ") + QString::number(temp->quantity));
	// name box, occupies all remaining space
	textbox.setTopLeft(
		option.rect.topLeft() + QPoint(0, option.fontMetrics.height() * 1.1)
	);
	textbox.setBottomRight(option.rect.bottomRight());
	gr.setStart(textbox.topLeft());
	gr.setFinalStop(textbox.bottomLeft());
	gr.setColorAt(1, option.palette.brightText().color());
	gr.setColorAt(0, option.palette.highlight().color());
	painter->setOpacity(0.8);
	painter->setBrush(gr);
	painter->drawRect(textbox);
	painter->setOpacity(1);
	painter->drawText(textbox, Qt::AlignCenter | Qt::TextWordWrap, temp->name);

	if (option.state.testFlag(QStyle::State_Selected))
	{
		painter->setBrush(option.palette.highlight());
		painter->setOpacity(0.3);
		painter->drawRect(option.rect);
	}

	painter->restore();
}

QSize ProductsDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Product p = index.data().value<Product>();
	if (p == nullptr)
	{
		p = std::static_pointer_cast<ProductEntity>(index.data().value<DataEntity>());
		if (p == nullptr)
			return QSize(100, 50);
	}
	double wdth = option.fontMetrics.averageCharWidth() * p->name.length();
	double tabs = 0;
	if (option.rect.width() == 0)
	{
		tabs = wdth;
	}
	else
		tabs = std::ceil(wdth / option.rect.width());
	tabs = (tabs > 0) ? tabs : 1;
	return QSize(option.rect.width(), (tabs + 1.1) * option.fontMetrics.height());
}
