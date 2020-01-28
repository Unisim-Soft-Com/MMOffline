#pragma once
#include "Widgets/parents/inframedWidget.h"
#include "Dataprovider/DataEntities.h"
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QListView>
#include <QtWidgets/QLineEdit>

/*
	This widget is used for selecting document. It is branch independent. 
	It allows to change document representation in it without affecting database.
	It allows user to select action with document, but does not performs it.
	It does not needed to be primed. 

	Affected tables:
	V		Documents
*/

class DocumentSelectionWidget : public inframedWidget
	// Allows user to select document and action
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

	// replaces document in the model, but not in database
	void replaceDocument(Document doc);
protected slots:
	// emits current document and action code
	void handleEdit();
	void handleDelete();
signals:
	void DocumentSelected(Document doc, int action);
};
