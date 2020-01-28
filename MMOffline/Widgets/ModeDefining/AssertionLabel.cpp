#include "AssertionLabel.h"
#include "Widgets/utils/ElementsStyles.h"
void AssertionLabel::_makeErrorLabel(const QString& errortext)
{
	QLabel* ilabel = new QLabel(this);
	ilabel->setPixmap(errorIcon.pixmap(calculateAdaptiveSize(0.1, 0.333333)));
	ilabel->setFixedHeight(calculateAdaptiveButtonHeight(0.1));
	mainLayout->addWidget(ilabel, topLvl, 0);
	ilabel = new QLabel(errortext, this);
	ilabel->setFixedHeight(calculateAdaptiveButtonHeight(0.1));
	mainLayout->addWidget(ilabel, topLvl, 1);
	++topLvl;
}

void AssertionLabel::_makeOkLabel()
{
	QLabel* ilabel = new QLabel(this);
	ilabel->setPixmap(okIcon.pixmap(calculateAdaptiveSize(0.1, 0.333333)));
	ilabel->setFixedHeight(calculateAdaptiveButtonHeight(0.1));
	mainLayout->addWidget(ilabel, topLvl, 0);
	ilabel = new QLabel(tr("No errors"), this);
	ilabel->setFixedHeight(calculateAdaptiveButtonHeight(0.1));
	mainLayout->addWidget(ilabel, topLvl, 1);
	++topLvl;
}

AssertionLabel::AssertionLabel(QWidget* parent)
	: QWidget(parent), mainLayout(new QGridLayout(this)), errorIcon(":/res/errorstate.png"),
	okIcon(":/res/okstate.png"), topLvl(0)
{
	this->setLayout(mainLayout);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(1, 1, 1, 1);
}

void AssertionLabel::setErrorlist(const QStringList& errors)
// will never create more than 4 rows to avoid off-screen labels
{
	switch (errors.count())
	{
	case 0:
		_makeOkLabel();
		break;
	case 1:
	case 2:
	case 3:
	case 4:
		for (const QString& error : errors)
		{
			_makeErrorLabel(error);
		}
		break;
	default:
		for (int i = 0; i < 4; ++i)
		{
			_makeErrorLabel(errors.at(i));
		}
		break;
	}
}

void AssertionLabel::clear()
{
	for (QLabel* label : labels)
	{
		mainLayout->removeWidget(label);
		label->deleteLater();
	}
	labels.clear();
}