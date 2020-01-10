#include "LogBranchRoot.h"

LogBranchRoot::LogBranchRoot(QWidget* parent)
	: inframedWidget(parent), abstractNode(), mainLayout(new QVBoxLayout(this)),
	docEditing(new DocumentSelectionWidget(this)), entryEditing(new EntryRedactingSubbranch(this))
{
	current = docEditing;
	main = this;
	untouchable = docEditing;
	this->setLayout(mainLayout);
	mainLayout->addWidget(untouchable);
	mainLayout->addWidget(entryEditing);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	entryEditing->hide();

	QObject::connect(docEditing, &DocumentSelectionWidget::backRequired, this, &LogBranchRoot::hideCurrent);
	QObject::connect(entryEditing, &EntryRedactingSubbranch::backRequired, this, &LogBranchRoot::hideCurrent);
	QObject::connect(docEditing, &DocumentSelectionWidget::DocumentSelected, this, &LogBranchRoot::onDocInteractions);
	QObject::connect(entryEditing, &EntryRedactingSubbranch::editingFinished, this, &LogBranchRoot::onDocChange);

}

void LogBranchRoot::onDocInteractions(Document doc, int action)
{
	switch (action)
	{
	case deleteDocument:
	{
		AppWorkset->dataprovider.removeOneEntity(doc);
		AppWorkset->dataprovider.removeEntitiesFiltered<DocumentEntryEntity>("parentDocId = " + QString::number(doc->documentId));
		return;
	}
	case editDocument:
	{
		entryEditing->setDocument(doc);
		_hideAny(entryEditing);
		return;
	}
	default:
		return;
	}
}

void LogBranchRoot::onDocChange(Document doc)
{
	AppWorkset->dataprovider.replaceData(doc);
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
