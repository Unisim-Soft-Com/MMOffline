#include "BigButtonsSpinbox.h"
#include "widgets/utils/ElementsStyles.h"
#include <QtWidgets/QDoubleSpinBox>
#include <cmath>
BigButtonsSpinbox::BigButtonsSpinbox(spintype type, QWidget* parent, double adaptH)
	: QWidget(parent), mainLayout(new QGridLayout(this)),
	buttonUp(new QPushButton(this)), buttonDown(new QPushButton(this)),
	infoLabel(new QLabel(this)),
	keyFilter(new filters::CaptureBackFilter(this)),
	coreSpinbox()
{
	switch (type)
	{
	case intspin:
		coreSpinbox = new QSpinBox(this);
		break;
	case timespin:
		coreSpinbox = new QTimeEdit(this);
	case floatspin:
		coreSpinbox = new QDoubleSpinBox(this);
	}
	sptype = type;
	this->setLayout(mainLayout);
	mainLayout->addWidget(buttonUp, 0, 0, 3, 1);
	mainLayout->addWidget(infoLabel, 0, 1);
	mainLayout->addWidget(coreSpinbox, 1, 1, 2, 1);
	mainLayout->addWidget(buttonDown, 0, 2, 3, 1);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	buttonUp->setIcon(QIcon(":/res/uparrow.png"));
	buttonDown->setIcon(QIcon(":/res/downarrow.png"));
	coreSpinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
	buttonUp->setMinimumHeight(calculateAdaptiveButtonHeight(adaptH));
	buttonDown->setMinimumHeight(calculateAdaptiveButtonHeight(adaptH));
	infoLabel->setMinimumHeight(calculateAdaptiveButtonHeight(adaptH / 3));
	infoLabel->setFont(makeFont(0.027));
	infoLabel->setAlignment(Qt::AlignCenter);
	coreSpinbox->setMinimumHeight(calculateAdaptiveButtonHeight(adaptH * 0.6666));
	coreSpinbox->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum));

	coreSpinbox->installEventFilter(keyFilter);

	infoLabel->setStyleSheet("QLabel{border: 1px solid black;}");
	buttonUp->setStyleSheet(UP_SPINBOX_STYLESHEET);
	buttonDown->setStyleSheet(DOWN_SPINBOX_STYLESHEET);
	QObject::connect(buttonUp, &QPushButton::pressed, coreSpinbox, &QSpinBox::stepUp);
	QObject::connect(buttonDown, &QPushButton::pressed, coreSpinbox, &QSpinBox::stepDown);
	QObject::connect(coreSpinbox, &QAbstractSpinBox::editingFinished, this, &BigButtonsSpinbox::editingDone);
	QObject::connect(keyFilter, &filters::CaptureBackFilter::backRequired, this, &BigButtonsSpinbox::backRequire);

	switch (sptype)
		//RTTI used to connect right signals
	{
	case intspin:
	{
		QSpinBox* isp;
		isp = qobject_cast<QSpinBox*>(coreSpinbox);
		if (isp != Q_NULLPTR) {
			isp->setSpecialValueText("");
			QObject::connect(isp, QOverload<int>::of(&QSpinBox::valueChanged), this, &BigButtonsSpinbox::intValueChanged);
		}
		break;
	}
	case timespin:
	{QTimeEdit* tsp;
	tsp = qobject_cast<QTimeEdit*>(coreSpinbox);
	if (tsp != Q_NULLPTR)
	{
		tsp->setDisplayFormat("HH:mm:ss");
		QObject::connect(tsp, &QTimeEdit::timeChanged, this, &BigButtonsSpinbox::timeValueChanged);
	}break;
	}
	case floatspin:
	{
		QDoubleSpinBox* dsp;
		dsp = qobject_cast<QDoubleSpinBox*>(coreSpinbox);
		if (dsp != Q_NULLPTR)
		{
			dsp->setSpecialValueText("");
            dsp->setDecimals(3);
			QObject::connect(dsp, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &BigButtonsSpinbox::doubleValueChanged);
		}
		break;
	}
	default:
		break;
	}
}

void BigButtonsSpinbox::setMinimum(int min)
{
	switch (sptype)
	{		// }
	case intspin:
	{
		QSpinBox* isp = qobject_cast<QSpinBox*>(coreSpinbox);	// <-This is normal RTTI check:
		if (isp != Q_NULLPTR)							//	to see if this pointer can be casted
		{								//}
			isp->setMinimum(min);
		}
		break;
	}
	case floatspin:
	{
		QDoubleSpinBox* dsp = qobject_cast<QDoubleSpinBox*>(coreSpinbox);
		if (dsp != Q_NULLPTR)
		{
			dsp->setMinimum(min);
		}
		break;
	}
	}
}

void BigButtonsSpinbox::setMaximum(int max)
{
	switch (sptype)
	{		// }
	case intspin:
	{
		QSpinBox* isp = qobject_cast<QSpinBox*>(coreSpinbox);	// <-This is normal RTTI check:
		if (isp != Q_NULLPTR)							//	to see if this pointer can be casted
		{								//}
			isp->setMaximum(max);
		}
		break;
	}
	case floatspin:
	{
		QDoubleSpinBox* dsp = qobject_cast<QDoubleSpinBox*>(coreSpinbox);
		if (dsp != Q_NULLPTR)
		{
			dsp->setMaximum(max);
		}
		break;
	}
	}
}

void BigButtonsSpinbox::setValue(int val)
{
	switch (sptype)
	{		// }
	case intspin:
	{
		QSpinBox* isp = qobject_cast<QSpinBox*>(coreSpinbox);	// <-This is normal RTTI check:
		if (isp != Q_NULLPTR)							//	to see if this pointer can be casted
		{								//}
			isp->setValue(val);
		}
		break;
	}
	case floatspin:
	{
		QDoubleSpinBox* dsp = qobject_cast<QDoubleSpinBox*>(coreSpinbox);
		if (dsp != Q_NULLPTR)
		{
			dsp->setValue(val);
		}
		break;
	}
	}
}

void BigButtonsSpinbox::setDValue(double val)
{
	switch (sptype) {
	case floatspin:
	{
		QDoubleSpinBox* dsp = qobject_cast<QDoubleSpinBox*>(coreSpinbox);
		if (dsp != Q_NULLPTR)
		{
			dsp->setValue(val);
		}
		break;
	}
	case intspin:
	{
		QSpinBox* isp = qobject_cast<QSpinBox*> (coreSpinbox);
		if (isp != Q_NULLPTR)
		{
			int floored = (int)std::ceilf(val);
			isp->setValue(floored);
		}
		break;
	}
	}
}

void BigButtonsSpinbox::setTime(const QTime& tm)
{
	if (sptype == timespin)
	{
		QTimeEdit* tsp = qobject_cast<QTimeEdit*> (coreSpinbox);
		if (tsp != 0)
		{
			tsp->setTime(tm);
		}
	}
}

int BigButtonsSpinbox::value() const
{
	switch (sptype)
	{
	case intspin:
	{	QSpinBox* isp = qobject_cast<QSpinBox*>(coreSpinbox);
	if (isp != Q_NULLPTR)
	{
		return isp->value();
	}
	break;
	}
	case floatspin:
	{	QDoubleSpinBox* dsp = qobject_cast<QDoubleSpinBox*> (coreSpinbox);
	if (dsp != Q_NULLPTR)
	{
		return (int)dsp->value();
	}
	break;
	}
	default:
		break;
	}
	return 0;
}

double BigButtonsSpinbox::dvalue() const
{
	switch (sptype)
	{
	case floatspin:
	{	QDoubleSpinBox* dsp = qobject_cast<QDoubleSpinBox*>(coreSpinbox);
	if (dsp != Q_NULLPTR)
	{
		return dsp->value();
	}
	break; }
	case intspin:
	{
		QSpinBox* isp = qobject_cast<QSpinBox*>(coreSpinbox);
		if (isp != Q_NULLPTR)
		{
			return (double)isp->value();
		}
		break;
	}
	}
	return 0.0;
}

QTime BigButtonsSpinbox::time()
{
	if (sptype == timespin)
	{
		QTimeEdit* tsp = qobject_cast<QTimeEdit*>(coreSpinbox);
		if (tsp != Q_NULLPTR)
		{
			return tsp->time();
		}
	}
	return QTime();
}

void BigButtonsSpinbox::setDisplayFormat(const QString& tf)
{
	if (sptype == timespin)
	{
		QTimeEdit* tsp = qobject_cast<QTimeEdit*>(coreSpinbox);
		if (tsp != Q_NULLPTR)
		{
			tsp->setDisplayFormat(tf);
		}
	}
}

bool BigButtonsSpinbox::hasFocus() const
{
	return coreSpinbox->hasFocus();
}

void BigButtonsSpinbox::setInfo(QString& str)
{
	infoLabel->setText(str);
}

void BigButtonsSpinbox::timeValueChanged(const QTime& t)
{
	emit timeChanged(t);
	emit valueChanged(coreSpinbox->text());
}

void BigButtonsSpinbox::doubleValueChanged(double v)
{
	emit dvalueChanged(v);
	emit valueChanged(coreSpinbox->text());
}

void BigButtonsSpinbox::editingDone()
{
	emit returnPressed();
}

void BigButtonsSpinbox::backRequire()
{
	emit backRequired();
}

void BigButtonsSpinbox::setFocus() const
{
	coreSpinbox->setFocus();
}

void BigButtonsSpinbox::intValueChanged(int t)
{
	emit ivalueChanged(t);
	emit valueChanged(coreSpinbox->text());
}
