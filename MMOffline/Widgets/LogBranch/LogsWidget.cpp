#include "LogsWidget.h"
#include "Widgets/utils/ElementsStyles.h"
#include <QtWidgets/QMessageBox>



LogsWidget::LogsWidget(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)), totalInfo(new QLabel(this)),
	deleteAllButton(new MegaIconButton(this)), logView(new QListWidget(this)),
	backButton(new MegaIconButton(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(totalInfo);
	mainLayout->addWidget(deleteAllButton);
	mainLayout->addWidget(logView);
	mainLayout->addWidget(backButton);

	backButton->setIcon(QIcon(":/res/back.png"));
	deleteAllButton->setIcon(QIcon(":/res/data.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	deleteAllButton->setStyleSheet(DELETE_BUTTONS_STYLESHEET);
	totalInfo->setText(tr("All made actions"));
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &LogsWidget::backRequired);
	QObject::connect(deleteAllButton, &MegaIconButton::clicked, this, &LogsWidget::deleteAllActions);
}

void LogsWidget::deleteAllActions()
{
	auto result = QMessageBox::question(this, tr("delete all?"), tr("delete all?"), QMessageBox::Ok, QMessageBox::Cancel);
	if (result == QMessageBox::Ok)
	{
		/*
			delete actions
		
		*/
	}
}

