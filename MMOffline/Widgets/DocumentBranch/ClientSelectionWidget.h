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
	//MegaIconButton* doSearchButton;
	QListView* clientView;
	MegaIconButton* backButton;
	//ClientDataModel* innerModel;
	DataEntityListModel* innerModel;
	ClientsDelegate* innerDelegate;
	QSortFilterProxyModel* searchProxy;

public:
	ClientSelectionWidget(QWidget* parent);
protected slots:
	void doSearch();
signals:
	void clientSelected(ClientEntity);
};