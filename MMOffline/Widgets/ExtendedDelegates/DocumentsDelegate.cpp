#include "DocumentsDelegate.h"
#include <QtGui/QPainter>
#include "DelegateUtility.h"
#include "Dataprovider/DataEntities.h"
#include <cmath>

// This literal is used for formatting datetime panel
const QString datetimeViewLiteral =
QStringLiteral("dd.MM/HH:mm");

void DocumentsDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Document d = upcastItem<DocumentEntity>(index);
	if (d == nullptr)
		return;
	//painting begins
	painter->save();
	// drawing rectangle for first date
	QRect textbox(
		QPoint(
			option.rect.bottomLeft().x(), option.rect.bottomLeft().y() - ((option.fontMetrics.height() + 6)))
		, QSize(
		(option.rect.width() * 0.66), option.fontMetrics.height() + 7));
	painter->setPen(Qt::PenStyle::SolidLine);
	drawRect(textbox, bright_delegate_color, painter);
	painter->drawText(textbox, Qt::AlignCenter, /*tr("Shipped:") +*/
		d->shippingDate.toString(QStringLiteral("dd.MM.yyyy")));

	// drawing rectangle for document id. It is truncated to 6 HEX symbols.
	textbox.setTopLeft(textbox.topRight());
	textbox.setBottomRight(option.rect.bottomRight());
	drawRect(textbox, bright_delegate_color, painter);
	painter->drawText(textbox, Qt::AlignCenter, QString::number(d->documentId, 16).right(6));

	// drawing rectangle for second date
	textbox.setTopLeft(QPoint(option.rect.bottomLeft().x(), option.rect.bottomLeft().y() - ((option.fontMetrics.height() + 6) * 2)
	));
	textbox.setBottomRight(QPoint(option.rect.topRight().x() - (option.rect.width() - (option.rect.width() * 0.658)),
		option.rect.bottomLeft().y() -
		((option.fontMetrics.height() + 6))
	));
	drawRect(textbox, bright_delegate_color, painter);
	painter->drawText(textbox, Qt::AlignCenter, /*tr("created:") + */
		d->dateWhenCreated.toString(datetimeViewLiteral));

	// drawing rectangle for summ paid
	textbox.setTopLeft(textbox.topRight());
	textbox.setBottomLeft(QPoint(option.rect.bottomRight().x(),
		option.rect.bottomLeft().y() - ((option.fontMetrics.height() + 6))));
	drawRect(textbox, bright_delegate_color, painter);
	painter->drawText(textbox, Qt::AlignCenter, tr("Paid:") + QString::number(d->alreadyPaid));
	
	// drawing flexible rectangle to wrap full client name
	textbox.setTopLeft(option.rect.topLeft());
	textbox.setBottomRight(QPoint(option.rect.bottomRight().x(), option.rect.bottomRight().y() - ((option.fontMetrics.height() + 6) * 2)));
	drawRect(textbox, dark_delegate_color, painter);
	painter->drawText(textbox, Qt::TextWordWrap | Qt::AlignCenter, d->clientName);

	//drawing selection rectangle
	if (option.state.testFlag(QStyle::State_Selected))
	{
		painter->setBrush(option.palette.highlight());
		painter->setOpacity(0.4);
		painter->drawRect(option.rect);
	}
	painter->restore();
}

QSize DocumentsDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	// final size : 2 * font height + enough space to wrap client name
	Document d = upcastItem<DocumentEntity>(index);
	if (d == nullptr)
		return QSize(100, 50);
	double wdth = option.fontMetrics.averageCharWidth() * (d->clientName.length());
	int tabs = 0;
	if (option.rect.width() == 0)
	{
		tabs = wdth;
	}
	else
		tabs = std::ceil(wdth / option.rect.width());
	tabs = (tabs > 1) ? tabs : 2;
	double t = (tabs + 2) * (option.fontMetrics.height() + 2);
	return QSize(option.rect.width(), t);
}
