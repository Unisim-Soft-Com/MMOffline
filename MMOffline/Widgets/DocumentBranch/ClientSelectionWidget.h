#pragma once
#include "Widgets/parents/inframedWidget.h"
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QLineEdit>
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include <QtWidgets/QListView>
#include <Dataprovider/DataEntities.h>

class ClientSelectionWidget : public inframedWidget
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
	void incrementDocCounter(int id, int q = 1);
protected slots:
	void clientClicked(DataEntity);
	void okClicked();
signals:
	void clientSelected(ClientEntity);
};
