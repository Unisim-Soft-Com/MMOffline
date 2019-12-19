#pragma once
#include <QtWidgets/QVBoxLayout>
#include "Widgets/parents/inframedWidget.h"
#include "Widgets/parents/abstractNodeInterface.h"
#include "Widgets/DocumentBranch/ClientSelectionWidget.h"
#include "Widgets/MultibranchWidgets/GroupSelectionWidget.h"
class DocumentCreationScreen;
class EntryCreationScreen;

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


public:
	DocumentRootWidget(QWidget* parent = nullptr);
};