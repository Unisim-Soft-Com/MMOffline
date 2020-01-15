#pragma once
#include "Widgets/parents/abstractNodeInterface.h"
#include "Widgets/MultibranchWidgets/DocumentSelectionWidget.h"
#include "Widgets/LogBranch/DocumentEntrySelectionSubbranch.h"

class LogBranchRoot : public inframedWidget, abstractNode
{
	Q_OBJECT
protected:
	enum actions { deleteDocument, editDocument };
	QVBoxLayout* mainLayout;
	DocumentSelectionWidget* docEditing;
	EntryRedactingSubbranch* entryEditing;

public:
	LogBranchRoot(QWidget* parent = nullptr);

protected slots:
	void onDocInteractions(Document doc, int action);
	void onDocChange(Document doc);
	void hideCurrent();
};