#pragma once
#include "Widgets/parents/inframedWidget.h"
#include "Widgets/parents/abstractNodeInterface.h"
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QLineEdit>
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include "Dataprovider/Entities/ClientEntity.h"
#include <QtWidgets/QListView>
#include "Widgets/ExtendedDelegates/ClientsDelegate.h"
#include <QtCore/QSortFilterProxyModel>
#include <Dataprovider/DataEntities.h>

class ClientSelectionWidget : public inframedWidget, abstractNode
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QLabel* userInfo;
	QHBoxLayout* searchPanel;
	QLineEdit* searchLine;
	QListView* clientView;
	MegaIconButton* backButton;
	DataCountingDataModel* innerModel;
	ClientsDelegate* innerDelegate;
	DataEntityFilterModel* searchProxy;

public:
	ClientSelectionWidget(QWidget* parent);
	void incrementDocCounter(int id, int q=1);
protected slots:
	void clientClicked(DataEntity);
signals:
	void clientSelected(ClientEntity);
};