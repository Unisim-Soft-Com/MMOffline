#pragma once
#include "Widgets/parents/inframedWidget.h"
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QLineEdit>
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include <QtWidgets/QListView>
#include <Dataprovider/DataEntities.h>

/*
	This widget is created for client selection screen. It implements list widget with search string
	and proxy model, which allows to filter results. Also, this widget automatically counts documents
	created for this client. This widget loads data on creation and never again asks for refresh, so 
	updating quantity of documents is only manual.

	Affected tables:
	V:	Clients, Documents

	emitted on pick:
		clientSelected(client entity)
*/



class ClientSelectionWidget : public inframedWidget
	// Allows user to pick client from Clients table
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	QLabel* userInfo;
	QHBoxLayout* searchPanel;
	QLabel* searchInfo;
	QLineEdit* searchLine;
	QListView* clientView;
	QHBoxLayout* buttonPanel;
	MegaIconButton* backButton;
	MegaIconButton* okButton;
	DataCountingDataModel* innerModel;
	DataEntityFilterModel* searchProxy;

public:
	ClientSelectionWidget(QWidget* parent);
	// Increments counter for the counter with provided id
	void incrementDocCounter(IdInt id, int q = 1);
protected slots:
	// triggered on doubleclick
	void clientClicked(DataEntity);
	// chooses currently selected client and emits it as it was clicked
	void okClicked();
signals:
	void clientSelected(ClientEntity);
};
