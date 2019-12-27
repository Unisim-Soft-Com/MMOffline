#pragma once
#include "Widgets/parents/inframedWidget.h"
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/qlistview.h>
#include "Dataprovider/DataEntities.h"
#include "Widgets/ExtendedDelegates/DocumentsDelegate.h"


class LogsWidget : public inframedWidget
{
protected:
	QVBoxLayout* mainLayout;
	QListView* logView;
	MegaIconButton* backButton;
	DataEntityListModel* innerModel;
	DocumentsDelegate* innerDelegate;
public:
	LogsWidget(QWidget* parent);

private slots:
	void deleteAllActions();

};