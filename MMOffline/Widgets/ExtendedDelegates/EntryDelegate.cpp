#include "EntryDelegate.h"
#include "Dataprovider/DataEntities.h"
#include "DelegateUtility.h"
#include <QtGui/QPainter>
void EntryDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	DocumentEntry d = upcastItem<DocumentEntryEntity>(index);
	if (d == nullptr)
		return;
	QLinearGradient gr;
	painter->save();
	QRect textbox(
		QPoint(
			option.rect.bottomLeft().x(), option.rect.bottomLeft().y() - ((option.fontMetrics.height() + 6)))
		, QSize(
		(option.rect.width() * 0.333333333), option.fontMetrics.height() + 7));
	setGradient(&textbox, &gr, option.palette.dark().color(), option.palette.highlight().color());
	painter->setPen(Qt::PenStyle::SolidLine);
	drawRect(textbox, gr, painter);
	painter->drawText(textbox, Qt::AlignCenter, tr("Price: ") + QString::number(d->price));

	textbox.setTopLeft(textbox.topRight());
	textbox.setBottomRight(QPoint(
			option.rect.topLeft().x() + (option.rect.width() * 0.66666666),
		option.rect.bottomLeft().y()
		));
	drawRect(textbox, gr, painter);
	painter->drawText(textbox, Qt::AlignCenter, tr("Quantity: ") + QString::number(d->quantity));

	textbox.setTopLeft(textbox.topRight());
	textbox.setBottomRight(option.rect.bottomRight());
	drawRect(textbox, gr, painter);
	painter->drawText(textbox, Qt::AlignCenter, tr("Summ: ") + QString::number(d->quantity * d->price));


	textbox.setTopLeft(option.rect.topLeft());
	textbox.setBottomRight(QPoint(option.rect.bottomRight().x(), option.rect.bottomRight().y() - ((option.fontMetrics.height() + 6))));

	setGradient(&textbox, &gr, option.palette.brightText().color(), option.palette.highlight().color());
	drawRect(textbox, gr, painter);
	painter->drawText(textbox, Qt::AlignCenter, d->productName);
	if (option.state.testFlag(QStyle::State_Selected))
	{
		painter->setBrush(option.palette.highlight());
		painter->setOpacity(0.3);
		painter->drawRect(option.rect);
	}
	painter->restore();
}

QSize EntryDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	DocumentEntry d = upcastItem<DocumentEntryEntity>(index);
	if (d == nullptr)
		return QSize(100, 50);
	double wdth = option.fontMetrics.averageCharWidth() * (d->productName.length() + 8);
	int tabs = 0;
	if (option.rect.width() == 0)
	{
		tabs = wdth;
	}
	else
		tabs = std::ceil(wdth / option.rect.width());
	tabs = (tabs > 0) ? tabs : 1;
	double t = (tabs + 1.8) * (option.fontMetrics.height() + 2);
	return QSize(option.rect.width(), t);
}
