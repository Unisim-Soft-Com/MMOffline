#pragma once
#include "Widgets/parents/abstractNodeInterface.h"
#include "Widgets/DocumentBranch/ClientSelectionWidget.h"
#include "Widgets/MultibranchWidgets/GroupSelectionWidget.h"
#include "Widgets/DocumentBranch/DocumentCreationScreen.h"
#include "Widgets/DocumentBranch/EntryCreationScreen.h"
#include "Widgets/MultibranchWidgets/ProductSelectionWidget.h"

/*
	This widget is "manager" of other widgets. His main goal is to create
	branch using provided subdocuments. Here is concentrated pure data flow
	without UI interactions. This widget only manages WHAT to show, not HOW.
	It is constructed on abstractNode interfaces, so it supports _hide() 
	methods. It has own layout and frame, but all interaction happens with 
	it's subbranches.
	Affected tables:
	E:		Documents, Entries
*/


class DocumentRootWidget : public inframedWidget, abstractNode
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	// subwidgets
	ClientSelectionWidget* clientSelection;
	DocumentCreationScreen* documentCreation;
	GroupSelectionWidget* groupSelection;
	ProductSelectionWidget* productSelection;
	EntryCreationScreen* entryCreation;

	// data collected
	Client currentClient;
	Group currentGroup;
	Document currentDocument;
	// documents are saved only if they have more than 0 entries
	bool isDocumentSaved;
public:
	DocumentRootWidget(QWidget* parent = nullptr);

protected slots:
	// primes creation of new document
	void clientConfirmed(ClientEntity);
	// defines which subbranch must be shown after hiding current
	void hideCurrent();
	// prepares document for adding entries
	void documentCreated(Document);
	// primes product selection
	void groupSelected(GroupEntity);
	// primes entry creation
	void productSelected(Product);
	// saves accumulated data
	void entryCreated(DocumentEntry);
};