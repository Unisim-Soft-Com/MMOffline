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
	GeneralPurposeFilter* temp = qobject_cast<GeneralPurposeFilter*>(keyfilter);
	QObject::installEventFilter(keyfilter);
	QObject::connect(temp, &GeneralPurposeFilter::backPressed, this, &inframedWidget::backReaction);
	QObject::connect(temp, &GeneralPurposeFilter::returnPressed, this, &inframedWidget::returnReaction);
	QObject::connect(temp, &GeneralPurposeFilter::numberPressed, this, &inframedWidget::controlReaction);
}
inframedWidget::inframedWidget(bool listen, QWidget* parent)
	: QWidget(parent), keyfilter(nullptr)
{
	if (listen)
	{
		GeneralPurposeFilter * temp = new filters::GeneralPurposeFilter(GeneralPurposeFilter::infPack, this);
		QObject::installEventFilter(keyfilter);
		QObject::connect(temp, &GeneralPurposeFilter::backPressed, this, &inframedWidget::backReaction);
		QObject::connect(temp, &GeneralPurposeFilter::returnPressed, this, &inframedWidget::returnReaction);
		QObject::connect(temp, &GeneralPurposeFilter::numberPressed, this, &inframedWidget::controlReaction);
		keyfilter = temp;
	}
}
bool inframedWidget::back()
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
			keyfilter->deleteLater();
		}
		QWidget::installEventFilter(obj);
		return;
	}
}

void inframedWidget::disableListening()
{
	if (keyfilter != nullptr)
	{
		removeEventFilter(keyfilter);
		keyfilter->deleteLater();
		keyfilter = nullptr;
	}
}

bool inframedWidget::isListening()
{
	return keyfilter == nullptr;
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