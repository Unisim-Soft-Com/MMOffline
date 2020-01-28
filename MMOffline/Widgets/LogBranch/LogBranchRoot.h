#pragma once
#include "Widgets/parents/abstractNodeInterface.h"
#include "Widgets/MultibranchWidgets/DocumentSelectionWidget.h"
#include "Widgets/LogBranch/DocumentEntrySelectionSubbranch.h"

/*
	This widget is managing redacting of collected data. But it does not controls 
	his entry redacting subbranch, it only awaits redacting start and end. It is based
	on abstract node, so it is allowing backtracking.  It loads all his data during creation.
	It is using another widget as his main view. It has own layout, but it is made frameless to
	avoid space loss. Enumeration actions allows to extend list of interactions without 
	additional slots. It has no signals except backRequired.

	Affected tables:
	EV		Documents
*/

class LogBranchRoot : public inframedWidget, abstractNode
	// This branch is responsible for redacting and deleting documents
{
	Q_OBJECT
protected:
	// actions understanded by slot onDocInteractions
	enum actions { deleteDocument, editDocument };
	QVBoxLayout* mainLayout;
	// subwidget allowing to select doc and action to do with it
	DocumentSelectionWidget* docEditing;
	// subbranch for redacting entries
	DocumentRedactingSubbranch* entryEditing;

public:
	explicit LogBranchRoot(QWidget* parent = nullptr);

protected slots:
	// performs action on doc, then saves result in database
	void onDocInteractions(Document doc, int action);
	// replaces document in database
	void onDocChange(Document doc);
	// hides current branch
	void hideCurrent();
};