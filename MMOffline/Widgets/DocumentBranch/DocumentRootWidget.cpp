#include "DocumentRootWidget.h"
#include "Widgets/utils/ApplicationDataWorkset.h"

DocumentRootWidget::DocumentRootWidget(QWidget* parent)
	: inframedWidget(parent), abstractNode(),
	mainLayout(new QVBoxLayout(this)),
	clientSelection(new ClientSelectionWidget(this)),
	documentCreation(new DocumentCreationScreen(this)),
	groupSelection(new GroupSelectionWidget(this)),
	productSelection(new ProductSelectionWidget(this)),
	entryCreation(new EntryCreationScreen(this)),
	currentClient(), currentGroup(), isDocumentSaved(false)
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(clientSelection);
	mainLayout->addWidget(documentCreation);
	mainLayout->addWidget(groupSelection);
	mainLayout->addWidget(productSelection);
	mainLayout->addWidget(entryCreation);

	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	documentCreation->hide();
	groupSelection->hide();
	productSelection->hide();
	entryCreation->hide();
	untouchable = clientSelection;
	main = this;
	current = clientSelection;
	QObject::connect(clientSelection, &ClientSelectionWidget::clientSelected, this,
		&DocumentRootWidget::clientConfirmed);
	QObject::connect(documentCreation, &DocumentCreationScreen::documentCreated, this,
		&DocumentRootWidget::documentCreated);
	QObject::connect(groupSelection, &GroupSelectionWidget::groupSelected, this,
		&DocumentRootWidget::groupSelected);
	QObject::connect(productSelection, &ProductSelectionWidget::productObtained, this,
		&DocumentRootWidget::productSelected);
	QObject::connect(entryCreation, &EntryCreationScreen::entryCreated, this,
		&DocumentRootWidget::entryCreated);
	QObject::connect(clientSelection, &ClientSelectionWidget::backRequired, this,
		&DocumentRootWidget::hideCurrent);
	QObject::connect(documentCreation, &DocumentCreationScreen::backRequired, this,
		&DocumentRootWidget::hideCurrent);
	QObject::connect(groupSelection, &GroupSelectionWidget::backRequired, this,
		&DocumentRootWidget::hideCurrent);
	QObject::connect(productSelection, &ProductSelectionWidget::backRequired, this,
		&DocumentRootWidget::hideCurrent);
	QObject::connect(entryCreation, &EntryCreationScreen::backRequired, this,
		&DocumentRootWidget::hideCurrent);
}

void DocumentRootWidget::clientConfirmed(ClientEntity ce)
{
	currentClient = Client(new ClientEntity(ce));
	documentCreation->primeCreation(currentClient);
	_hideAny(documentCreation);
}

void DocumentRootWidget::hideCurrent()
{
	if (current == clientSelection)
		emit backRequired();
	else if (current == documentCreation || current == groupSelection)
	{
		_hideCurrent(clientSelection);
	}
	else if (current == productSelection)
		_hideCurrent(groupSelection);
	else if (current == entryCreation)
		_hideCurrent(productSelection);
}

void DocumentRootWidget::documentCreated(Document doc)
{
	currentDocument = doc;
	isDocumentSaved = false;
	_hideCurrent(groupSelection);
}

void DocumentRootWidget::groupSelected(GroupEntity g)
{
	currentGroup = Group(new GroupEntity(g));
	productSelection->primeSelection(currentGroup, currentClient);

	_hideCurrent(productSelection);
}

void DocumentRootWidget::productSelected(Product p)
{
	entryCreation->primeEntryCreation(p, currentDocument);
	_hideCurrent(entryCreation);
}

void DocumentRootWidget::entryCreated(DocumentEntry entry)
{
	if (!isDocumentSaved)
	{
		AppWorkset->dataprovider.replaceData(std::static_pointer_cast<abs_entity>(currentDocument));
		isDocumentSaved = true;
		clientSelection->incrementDocCounter(currentDocument->clientId);
	}
	AppWorkset->dataprovider.replaceData(entry);
	productSelection->setQuantityCounter(entry->productId, entry->quantity);
	hideCurrent();
}