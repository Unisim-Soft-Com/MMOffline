#include "AbstractListSelectionWidget.h"
// Qt 5 only imports
#include "widgets/utils/ElementsStyles.h"
#include <QtWidgets/qscroller.h>
#include "widgets/ElementWidgets/ZebraListItemDelegate.h"
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif

void  AbstractListSelectionWidget::init()
{
	for (int i = 0; i < countElems(); ++i)
	{
        addItem(elemToString(i));

#ifdef DEBUG
		detrace_CYCLEEXPL("adding elem " << elemToString(i) << "by index" << i);
#endif
	}
}

void AbstractListSelectionWidget::reload()
{
	clear();
	init();
}
AbstractListSelectionWidget::AbstractListSelectionWidget(QWidget* parent)
	: QListWidget(parent)
{
	this->setSelectionMode(QAbstractItemView::SingleSelection);
	QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);
    this->setFont(makeFont(0.05));
	this->setWordWrap(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ZebraItemDelegate* zd = new ZebraItemDelegate(this);
	setItemDelegate(zd);
	QObject::connect(this, &QListWidget::itemClicked, this, &AbstractListSelectionWidget::itemSelectedFromList);
}
