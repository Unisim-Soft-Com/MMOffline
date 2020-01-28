#include "ProductsDelegate.h"
#include <QPainter>
#include "Dataprovider/DataEntities.h"
#include "DelegateUtility.h"
#include <cmath>
void ProductsDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Product temp = upcastItem<ProductEntity>(index);
	if (temp == nullptr)
		return;
	// predrawing calculations
	QPoint diff(option.rect.bottomRight().x() * 0.333333, 0);
	QRect textbox(
		option.rect.topLeft(),
		QPoint(
			diff.x(),
			option.rect.topLeft().y() + option.fontMetrics.height() * 1.1
		));

	painter->save();
	//	Id box
	painter->setBrush(dark_delegate_color);
	painter->drawRect(textbox);
	painter->setPen(Qt::PenStyle::SolidLine);
	painter->drawText(
		textbox
		, Qt::AlignCenter, QString::number(temp->id));
	// Price box
	textbox.setBottomRight(textbox.bottomRight() + diff);
	textbox.setTopLeft(textbox.topLeft() + diff);
	painter->drawRect(textbox);
	painter->setPen(Qt::SolidLine);
	painter->drawText(textbox, Qt::AlignCenter, tr("Price:") + QString::number(temp->price, 'g', 4));
	// Quantity box
	textbox.setTopLeft(
		textbox.topLeft() + diff
	);
	textbox.setBottomRight(
		textbox.bottomRight() + diff
	);
	painter->drawRect(textbox);
	painter->setPen(Qt::SolidLine);
	painter->drawText(textbox, Qt::AlignCenter, tr("Quantity:") + QString::number(index.data(DataEntityListModel::QuantityView).toInt()));
	// name box, occupies all remaining space
	textbox.setTopLeft(
		option.rect.topLeft() + QPoint(0, option.fontMetrics.height() * 1.1)
	);
	textbox.setBottomRight(option.rect.bottomRight());
	painter->setBrush(bright_delegate_color);
	painter->drawRect(textbox);
	painter->drawText(textbox, Qt::AlignCenter | Qt::TextWordWrap, temp->name);
	painter->setBrush(Qt::NoBrush);
	painter->setPen(QPen(Qt::black, 3));
	textbox.setTopLeft(option.rect.topLeft() + QPoint(1, 1));
	textbox.setBottomRight(option.rect.bottomRight() - QPoint(1, 1));
	painter->drawRect(textbox);
	if (option.state.testFlag(QStyle::State_Selected))
	{
		painter->setBrush(option.palette.highlight());
		painter->setPen(Qt::NoPen);
		painter->setOpacity(0.4);
		painter->drawRect(option.rect);
	}

	painter->restore();
}

QSize ProductsDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
//	final size: 1.1 * font height + flexible box for wrapping text
	Product temp = upcastItem<ProductEntity>(index);
	if (temp == nullptr)
		return QSize(100, 50);

	double wdth = option.fontMetrics.averageCharWidth() * temp->name.length();
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