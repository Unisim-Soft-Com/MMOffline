#include "LogsWidget.h"
#include "Widgets/utils/ElementsStyles.h"
#include <QtWidgets/QMessageBox>
#include "Widgets/utils/ApplicationDataWorkset.h"
#include "Widgets/ExtendedDelegates/EntryDelegate.h"

LogsWidget::LogsWidget(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)), 
	logView(new QListView(this)),
	backButton(new MegaIconButton(this)),
	innerModel(new DataEntityListModel(this)),
	innerDelegate(new DocumentsDelegate(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(logView);
	mainLayout->addWidget(backButton);

	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	innerModel->setData(AppWorkset->dataprovider.loadDataAs<DocumentEntryEntity>());
	logView->setModel(innerModel);
	logView->setItemDelegate(new EntryDelegate(this));
	QObject::connect(backButton, &MegaIconButton::clicked, this, &LogsWidget::backRequired);
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

