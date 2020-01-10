#pragma once
#include "Widgets/parents/inframedWidget.h"
#include "Dataprovider/DataEntities.h"
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QListView>
#include <QtWidgets/QLineEdit>

class DocumentSelectionWidget : public inframedWidget
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	QListView* mainView;
	DataEntityListModel* coreModel;
	DataEntityFilterModel* filterModel;
	QLineEdit* searchField;
	QHBoxLayout* buttonsLayout;
	MegaIconButton* backButton;
	MegaIconButton* editButton;
	MegaIconButton* deleteButton;

public:
	DocumentSelectionWidget(QWidget* parent = nullptr);

	void replaceDocument(Document doc);
protected slots:
	void handleEdit();
	void handleDelete();
signals:
	void DocumentSelected(Document doc, int action);
};
