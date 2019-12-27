#include "DocumentsDelegate.h"
#include <QtGui/QPainter>
#include "DelegateUtility.h"
#include "Dataprovider/DataEntities.h"

const QString datetimeViewLiteral =
QStringLiteral("dd.MM/HH:mm");
void DocumentsDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Document d = upcastItem<DocumentEntity>(index);
	if (d == nullptr)
		return;
	QLinearGradient gr;
	painter->save();
	QRect textbox(
		QPoint(
			option.rect.bottomLeft().x(), option.rect.bottomLeft().y() - ((option.fontMetrics.height() + 6)))
		, QSize(
		(option.rect.width() / 2), option.fontMetrics.height() + 7));
	setGradient(&textbox, &gr, option.palette.dark().color(), option.palette.highlight().color());
	painter->setPen(Qt::PenStyle::SolidLine);
	drawRect(textbox, gr, painter);
	painter->drawText(textbox, Qt::AlignCenter, tr("Shipped: ") + d->shippingDate.toString(QStringLiteral("dd.MM.yyyy")));

	textbox.setTopLeft(textbox.topRight());
	textbox.setBottomRight(option.rect.bottomRight());
	setGradient(&textbox, &gr, option.palette.dark().color(), option.palette.highlight().color());
	drawRect(textbox, gr, painter);
	painter->drawText(textbox, Qt::AlignCenter, tr("created: ") + d->dateWhenCreated.toString(datetimeViewLiteral));

	textbox.setTopLeft(QPoint(option.rect.bottomLeft().x(), option.rect.bottomLeft().y() - ((option.fontMetrics.height() + 6) * 2)
	));
	textbox.setBottomRight(QPoint(option.rect.topRight().x() - (option.rect.width()/2),
		option.rect.bottomLeft().y()-
		((option.fontMetrics.height() + 6))
	));
	drawRect(textbox, gr, painter);
	painter->drawText(textbox, Qt::AlignCenter, QString::number(d->documentId, 16).right(6));

	textbox.setTopLeft(textbox.topRight());
	textbox.setBottomLeft(QPoint(option.rect.bottomRight().x(),
		option.rect.bottomLeft().y() - ( (option.fontMetrics.height() + 6))));
	setGradient(&textbox, &gr, option.palette.dark().color(), option.palette.highlight().color());
	drawRect(textbox, gr, painter);
	painter->drawText(textbox, Qt::AlignCenter, tr("Paid: ") + QString::number(d->alreadyPaid));
	textbox.setTopLeft(option.rect.topLeft());
	textbox.setBottomRight(QPoint(option.rect.bottomRight().x(), option.rect.bottomRight().y() -((option.fontMetrics.height() + 6) * 2)));

	setGradient(&textbox, &gr, QColor(Qt::red), option.palette.highlight().color());
	drawRect(textbox, gr, painter);
	painter->drawText(textbox, Qt::AlignCenter, d->clientName);
	painter->restore();
}

QSize DocumentsDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Document d = upcastItem<DocumentEntity>(index);
	if (d == nullptr)
		return QSize(100, 50);
	double wdth = option.fontMetrics.averageCharWidth() *( d->clientName.length());
	int tabs = 0;
	if (option.rect.width() == 0)
	{
		tabs = wdth;
	}
	else
		tabs = std::ceil(wdth / option.rect.width());
	tabs = (tabs > 0) ? tabs : 1;
	double t = (tabs + 2.8) * (option.fontMetrics.height() + 2);
	return QSize(option.rect.width(), t);
}

// Backup
//QRect textbox(option.rect.topLeft(), QSize(
//(option.rect.width() / 2), option.fontMetrics.height() + 6));
//setGradient(&textbox, &gr, QColor(Qt::red), option.palette.highlight().color());
//painter->setPen(Qt::PenStyle::SolidLine);
//drawRect(textbox, gr, painter);
//painter->drawText(textbox, Qt::AlignCenter, QString::number(d->documentId, 16).right(6));
//textbox.setTopLeft(textbox.topRight());
//textbox.setBottomRight(QPoint(option.rect.topRight().x(), option.rect.topLeft().y() + option.fontMetrics.height() + 6));
//setGradient(&textbox, &gr, option.palette.dark().color(), option.palette.highlight().color());
//drawRect(textbox, gr, painter);
//painter->drawText(textbox, Qt::AlignCenter, tr("Paid: ") + QString::number(d->alreadyPaid));
//textbox.setTopLeft(textbox.bottomLeft());
//textbox.setBottomRight(QPoint(option.rect.topRight().x(),
//	option.rect.topLeft().y() +
//	(option.fontMetrics.height() + 6) * 2
//));
//setGradient(&textbox, &gr, option.palette.highlight().color(), option.palette.dark().color());
//drawRect(textbox, gr, painter);
//painter->drawText(textbox, Qt::AlignCenter, tr("Shipped: ") + d->shippingDate.toString(QStringLiteral("dd.MM.yyyy")));
//textbox.setTopRight(textbox.topLeft());
//textbox.setBottomLeft(QPoint(0, option.rect.topLeft().y() + (option.fontMetrics.height() + 6) * 2));
//drawRect(textbox, gr, painter);
//painter->drawText(textbox, Qt::AlignCenter, tr("created: ") + d->dateWhenCreated.toString(datetimeViewLiteral));
//textbox.setTopLeft(QPoint(0, option.rect.topLeft().y() + (option.fontMetrics.height() + 6) * 2));
//textbox.setBottomRight(option.rect.bottomRight());
//setGradient(&textbox, &gr, option.palette.dark().color(), option.palette.highlight().color());
//drawRect(textbox, gr, painter);
//painter->drawText(textbox, Qt::AlignCenter, d->clientName);