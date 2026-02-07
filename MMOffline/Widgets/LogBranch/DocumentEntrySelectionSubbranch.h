#pragma once
#include "Widgets/DocumentBranch/EntryCreationScreen.h"
#include "Widgets/MultibranchWidgets/ProductSelectionBranch.h"
#include "Widgets/DocumentBranch/EntryCreationScreen.h"
#include "Widgets/MultibranchWidgets/ProductSelectionBranch.h"
#include <QComboBox>
#include <QDoubleSpinBox>
#include "Widgets/ElementWidgets/MegaIconButton.h"

/*
	This widget is representing full subbranch containing everything for redacting document.
	It allows to change some document-related fields and to dive lower into redacting entries.
	This widget is based on abstract node and can react at backRequired signals.

	Affected tables (without subbranches):
	EV			Entries
	V			Doctypes
	signals:
		editingFinished(Document)
*/
class DocumentRedactingSubbranch : public inframedWidget, abstractNode
{
	Q_OBJECT
protected:
	// subwidgets
	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QLabel* docInfo;
	QHBoxLayout* headerPanel;
    QDoubleSpinBox* summPaidField;
	QComboBox* doctypeField;
	QHBoxLayout* searchPanel;
	QLabel* searchInfo;
	QLineEdit* searchField;
	QListView* entriesView;
	QHBoxLayout* buttonsLayout;
	MegaIconButton* backButton;
	MegaIconButton* editButton;
	MegaIconButton* deleteButton;
	MegaIconButton* addButton;

	// data models
	DataEntityListModel* innerModel;
	DataEntityFilterModel* filterModel;

	// subbranches
	EntryCreationScreen* entryCreation;
	ProductSelectionBranch* productSelection;

	// data 
	Document currentDocument;
	NamedIdList doctypes;

public:
	DocumentRedactingSubbranch(QWidget* parent = nullptr);
	// primes redacting
	void setDocument(Document doc);
	// clear all branch
	void clearContents();

protected slots:
	// deletes selected entry
	void handleDelete();
	// extracts currently selected entry and primes creation
	void handleEdit();
	// primes entry redacting
	void handleEditedEntry(DocumentEntry);
	// raises product selection branch
	void handleAdd();
	// primes creation instead of redacting
	void handleSelectedProduct(Product);
	// returns to previous step of the branch
	void hideCurrent();
	// collects all data and emits editingFinished
	void finishRedacting();
signals:
	void editingFinished(Document);
};
