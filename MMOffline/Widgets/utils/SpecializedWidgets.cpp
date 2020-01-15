#include "SpecializedWidgets.h"
#include <exception> // Qt 5 only imports
#include <QtWidgets/qstyle.h>
#include <qstyleoption.h>
#include <qstylepainter.h>
specwidgets::indexedButton::indexedButton(int Index, QWidget* parent) : QPushButton(parent), index(Index)
{
    QObject::connect(this, &QPushButton::clicked, this, &indexedButton::clickCapt);
}
void specwidgets::indexedButton::setIndex(int Index)
{
	index = Index;
}
void specwidgets::indexedButton::clickCapt()
{
	emit indexClicked(index);
}
;

void specwidgets::toggledIndexedButton::paintEvent(QPaintEvent*)
{
	QStylePainter spainter(this);
	QStyleOptionButton opt;
	this->initStyleOption(&opt);
	spainter.drawPrimitive(QStyle::PE_Widget, opt);
	opt.text = "";
	spainter.drawControl(QStyle::CE_PushButton, opt);
	curricon = &((isChecked()) ? stateToggled : stateNon);
	spainter.drawItemPixmap(this->rect(), Qt::AlignLeft, curricon->pixmap(this->rect().height()));
	spainter.drawItemText(this->rect(), Qt::AlignCenter, QPalette(), true, text(), QPalette::NoRole);
}

void specwidgets::toggledIndexedButton::clickCapt()
{
	curricon = &((isChecked()) ? stateToggled : stateNon);
	indexedButton::clickCapt();
}

specwidgets::toggledIndexedButton::toggledIndexedButton(QIcon& non, QIcon& is, int Index, QWidget* parent)
	: indexedButton(Index, parent), stateNon(non), stateToggled(is)
{
}
