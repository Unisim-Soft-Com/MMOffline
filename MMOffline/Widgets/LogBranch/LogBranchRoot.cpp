#include "LogBranchRoot.h"
#include "Dataprovider/SqliteDataProvider.h"



LogBranchRoot::LogBranchRoot(QWidget* parent)
	: inframedWidget(parent), abstractNode(), mainLayout(new QVBoxLayout(this)),
	docEditing(new DocumentSelectionWidget(this)), entryEditing(new DocumentRedactingSubbranch(this))
{
	
	// setting up abstract node interfaces. DocEditing widget is considered as inner-view of branch
	current = docEditing;
	main = this;
	untouchable = docEditing;

	// emplacing widgets
	this->setLayout(mainLayout);
	mainLayout->addWidget(untouchable);
	mainLayout->addWidget(entryEditing);

	// removing margins to save space
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	
	// hiding subbranch
	entryEditing->hide();

	// connecting slots
	QObject::connect(docEditing, &DocumentSelectionWidget::backRequired, this, &LogBranchRoot::hideCurrent);
	QObject::connect(entryEditing, &DocumentRedactingSubbranch::backRequired, this, &LogBranchRoot::hideCurrent);
	QObject::connect(docEditing, &DocumentSelectionWidget::DocumentSelected, this, &LogBranchRoot::onDocInteractions);
	QObject::connect(entryEditing, &DocumentRedactingSubbranch::editingFinished, this, &LogBranchRoot::onDocChange);
}

void LogBranchRoot::onDocInteractions(Document doc, int action)
{
	switch (action)
	{
	case deleteDocument:
	{
		// direct delete from database. No need to care about deleting item from DocumentSelection
		AppData->removeOneEntity(doc);
		AppData->removeEntitiesFiltered(DataEntity(new DocumentEntryEntity()), "parentDocId = " + QString::number(doc->documentId));
		return;
	}
	case editDocument:
	{
		// priming editing and showing subbranch
		entryEditing->setDocument(doc);
		_hideAny(entryEditing);
		return;
	}
	default:
		// here can be added more actions
		return;
	}
}

void LogBranchRoot::onDocChange(Document doc)
{
	// direct replacing of the document in the database and in the selection
	AppData->replaceData(doc);
	docEditing->replaceDocument(doc);
	_hideCurrent(untouchable);
}

void LogBranchRoot::hideCurrent()
{
	if (current == docEditing)
		emit backRequired();
	else if (current == entryEditing)
	{
		_hideCurrent(untouchable);
	}
}