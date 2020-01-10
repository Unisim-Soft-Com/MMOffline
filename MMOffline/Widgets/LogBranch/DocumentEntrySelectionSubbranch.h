#pragma once
#include "Widgets/parents/inframedWidget.h"
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include "Dataprovider/DataEntities.h"
#include <QtWidgets/QListView>
#include <QtWidgets/QBoxLayout>
#include "Widgets/parents/abstractNodeInterface.h"
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDoubleSpinBox>
#include "Widgets/DocumentBranch/EntryCreationScreen.h"
#include "Widgets/MultibranchWidgets/ProductSelectionBranch.h"

class EntryRedactingSubbranch : public inframedWidget, abstractNode
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QLabel* docInfo;
	QHBoxLayout* headerPanel;
	QDoubleSpinBox* summPaidField;
	QComboBox* doctypeField;
	QLineEdit* searchField;
	QListView* entriesView;
	QHBoxLayout* buttonsLayout;
	MegaIconButton* backButton;
	MegaIconButton* editButton;
	MegaIconButton* deleteButton;
	MegaIconButton* addButton;

	DataEntityListModel* innerModel;
	DataEntityFilterModel* filterModel;

	EntryCreationScreen* entryCreation;
	ProductSelectionBranch* productSelection;

	Document currentDocument;

	NamedIdList doctypes;
public:
	EntryRedactingSubbranch(QWidget* parent = nullptr);
	void setDocument(Document doc);
	void clearContents();

protected slots:
	void handleDelete();
	void handleEdit();
	void handleEditedEntry(DocumentEntry);
	void handleAdd();
	void handleSelectedProduct(Product);
	void hideCurrent();
	void finishRedacting();
signals:
	void editingFinished(Document);
};
