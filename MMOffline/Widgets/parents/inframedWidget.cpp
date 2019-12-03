#include "inframedWidget.h"
//#define DEBUG
#ifdef DEBUG
#include <debugtrace.h>
#endif

using namespace filters;
bool inframedWidget::isExpectingControl(int val)
{
#ifdef DEBUG
	detrace_METHCALL(":isExpectingControl(" << val << " wadress: " << (long long int) this);
#endif

	return false;
}
inframedWidget::inframedWidget(QWidget* parent)
	: QWidget(parent), keyfilter(new filters::GeneralPurposeFilter(filters::GeneralPurposeFilter::infPack, this))
{
#ifndef Q_OS_ANDROID
	QObject::installEventFilter(keyfilter);
	QObject::connect(keyfilter, &GeneralPurposeFilter::backPressed, this, &inframedWidget::backReaction);
	QObject::connect(keyfilter, &GeneralPurposeFilter::returnPressed, this, &inframedWidget::returnReaction);
	QObject::connect(keyfilter, &GeneralPurposeFilter::numberPressed, this, &inframedWidget::controlReaction);
#endif
}
bool inframedWidget::back()
{
	return false;
}

bool inframedWidget::giveSettings()
{
	return false;
}

void inframedWidget::show()
{
	QWidget::show();
}

void inframedWidget::installEventFilter(QObject* obj)
{
	if (obj != keyfilter)
	{
		if (keyfilter != nullptr) {
			removeEventFilter(keyfilter);
#ifndef Q_OS_ANDROID
			keyfilter->deleteLater();
#endif
		}
		QWidget::installEventFilter(obj);
		return;
	}
}

void inframedWidget::returnReaction()
{
}

void inframedWidget::backReaction()
{
	emit backRequired();
}

void inframedWidget::controlReaction(int val)
{
	isExpectingControl(val - 1);
}