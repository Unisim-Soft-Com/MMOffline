#pragma once
#include "Widgets/parents/abstractNodeInterface.h"
#include "Widgets/DocumentBranch/ClientSelectionWidget.h"
#include "Widgets/MultibranchWidgets/GroupSelectionWidget.h"
#include "Widgets/DocumentBranch/DocumentCreationScreen.h"
#include "Widgets/DocumentBranch/EntryCreationScreen.h"
#include "Widgets/MultibranchWidgets/ProductSelectionWidget.h"

class DocumentRootWidget : public inframedWidget, abstractNode
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	ClientSelectionWidget* clientSelection;
	DocumentCreationScreen* documentCreation;
	GroupSelectionWidget* groupSelection;
	ProductSelectionWidget* productSelection;
	EntryCreationScreen* entryCreation;

	Client currentClient;
	Group currentGroup;
	Document currentDocument;
	bool isDocumentSaved;
public:
	DocumentRootWidget(QWidget* parent = nullptr);

protected slots:
	void clientConfirmed(ClientEntity);
	void hideCurrent();
	void documentCreated(Document);
	void groupSelected(GroupEntity);
	void productSelected(Product);
	void entryCreated(DocumentEntry);
};