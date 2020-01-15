#include "BigButtonsSpinbox.h"
#include "widgets/utils/ElementsStyles.h"
#include <QtWidgets/QDoubleSpinBox>
#include <cmath>
#include <QtWidgets/qcalendarwidget.h>
#include <QKeyEvent>
#include <QLineEdit>
#include "debugtrace.h"

#include "Widgets/utils/EventsAndFilters.h"

class extDSP : public QDoubleSpinBox
{
public:
	extDSP(QWidget* parent = nullptr)
		: QDoubleSpinBox(parent)
	{
		lineEdit()->installEventFilter(new filters::SpinboxHelper(this));
	}
};

void BigButtonsSpinbox::showEvent(QShowEvent* ev)
{
	QWidget::showEvent(ev);
	if (sptype == datespin)
	{
		QDateEdit* tsp = qobject_cast<QDateEdit*>(coreSpinbox);
		if (tsp != Q_NULLPTR)
		{
			QString tmp = "QDateEdit{"
				"border-radius: 1px;"
				"padding: 1px 1px 1px 1px;"
				"border: 1px solid black;"
				"}"
				"QDateEdit::drop-down {"
				"subcontrol-origin: padding;"
				"subcontrol-position: center right;"
				"width: " + QString::number((int)(coreSpinbox->width() / 3)) + " px;"
				"}"
				;
			tsp->setStyleSheet(tmp);
		}
	}
}
BigButtonsSpinbox::BigButtonsSpinbox(spintype type, QWidget* parent, double adaptH)
	: QWidget(parent), mainLayout(new QGridLayout(this)),
	buttonUp(new QPushButton(this)), buttonDown(new QPushButton(this)),

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
		break;
	case floatspin:
		coreSpinbox = new extDSP(this);
		break;
	case datespin:
		coreSpinbox = new QDateEdit(this);
		break;
	}
	sptype = type;
	this->setLayout(mainLayout);
	mainLayout->addWidget(buttonUp, 0, 0, 3, 1);
	mainLayout->addWidget(coreSpinbox, 0, 1, 3, 1);
	mainLayout->addWidget(buttonDown, 0, 2, 3, 1);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	buttonUp->setIcon(QIcon(":/res/uparrow.png"));
	buttonDown->setIcon(QIcon(":/res/downarrow.png"));
	coreSpinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
	buttonUp->setMinimumHeight(calculateAdaptiveButtonHeight(adaptH));
	buttonDown->setMinimumHeight(calculateAdaptiveButtonHeight(adaptH));
	coreSpinbox->setMinimumHeight(calculateAdaptiveButtonHeight(adaptH));
#ifdef Q_OS_ANDROID
	buttonUp->setMinimumWidth(calculateAdaptiveWidth(0.2));
	buttonDown->setMinimumWidth(calculateAdaptiveWidth(0.2));
#endif
#ifdef Q_OS_WIN
	buttonUp->setMinimumWidth(calculateAdaptiveButtonHeight(0.2) * 0.4);
	buttonDown->setMinimumWidth(calculateAdaptiveButtonHeight(0.2) * 0.4);
#endif
	coreSpinbox->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum));
	buttonUp->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
	buttonDown->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
	coreSpinbox->installEventFilter(keyFilter);
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
#ifdef Q_OS_ANDROID
			QObject::connect(dsp, &QDoubleSpinBox::editingFinished, QGuiApplication::inputMethod(), &QInputMethod::hide);
#endif
			QObject::connect(dsp, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &BigButtonsSpinbox::doubleValueChanged);
		}
		break;
	}
	case datespin:
	{
		QDateEdit* dsp;
		dsp = qobject_cast<QDateEdit*>(coreSpinbox);
		if (dsp != Q_NULLPTR)
		{
			buttonUp->hide();
			buttonDown->hide();
			dsp->setSpecialValueText("");
			dsp->setDate(QDate::currentDate());
			dsp->setCalendarPopup(true);
			dsp->setDisplayFormat(QStringLiteral("dd:MM:yyyy"));
			QObject::connect(dsp, &QDateEdit::dateChanged, this, &BigButtonsSpinbox::dateChanged);
			QObject::connect(dsp, &QDateEdit::dateChanged, qApp->inputMethod(), &QInputMethod::hide);
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

void BigButtonsSpinbox::setPrecision(int prec)
{
	switch (sptype)
	{
	case floatspin:
	{
		QDoubleSpinBox* dsp = qobject_cast<QDoubleSpinBox*>(coreSpinbox);
		if (dsp != Q_NULLPTR)
		{
			dsp->setDecimals(prec);
		}
		break;
	}
	}
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
}

QDate BigButtonsSpinbox::date()
{
	if (sptype == datespin)
	{
		QDateEdit* tsp = qobject_cast<QDateEdit*>(coreSpinbox);
		if (tsp != Q_NULLPTR)
		{
			return tsp->date();
		}
	}
	return QDate();
}

void BigButtonsSpinbox::clear()
{
	switch (sptype)
	{
	case intspin:
	case floatspin:
		setValue(0);
		break;
	case datespin:
		setDate(QDate::currentDate());
		break;
	case timespin:
		setTime(QTime::currentTime());
		break;
	}
}

void BigButtonsSpinbox::setDate(QDate time)
{
	if (sptype == datespin)
	{
		QDateEdit* tsp = qobject_cast<QDateEdit*>(coreSpinbox);
		if (tsp != Q_NULLPTR)
		{
			tsp->setDate(time);
		}
	}
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

void BigButtonsSpinbox::dateChanged(const QDate& d)
{
	emit dateValueChanged(d);
	emit valueChanged(coreSpinbox->text());
}

void BigButtonsSpinbox::setFocus() const
{
	coreSpinbox->setFocus();
	coreSpinbox->selectAll();
}

void BigButtonsSpinbox::intValueChanged(int t)
{
	emit ivalueChanged(t);
	emit valueChanged(coreSpinbox->text());
}