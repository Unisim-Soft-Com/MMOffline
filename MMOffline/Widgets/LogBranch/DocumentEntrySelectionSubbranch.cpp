#include "DocumentEntrySelectionSubbranch.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Widgets/ExtendedDelegates/EntryDelegate.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
#include <qmessagebox.h>

EntryRedactingSubbranch::EntryRedactingSubbranch(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	docInfo(new QLabel(innerWidget)), headerPanel(new QHBoxLayout(innerWidget)),
	summPaidField(new QDoubleSpinBox(innerWidget)),doctypeField(new QComboBox(innerWidget)),
	searchField(new QLineEdit(innerWidget)), entriesView(new QListView(innerWidget)),
	buttonsLayout(new QHBoxLayout(innerWidget)), backButton(new MegaIconButton(innerWidget)),
	editButton(new MegaIconButton(innerWidget)), deleteButton(new MegaIconButton(innerWidget)),
	addButton(new MegaIconButton(innerWidget)),
	innerModel(new DataEntityListModel(this)), filterModel(new DataEntityFilterModel(this)),
	entryCreation(new EntryCreationScreen(this)), productSelection(new ProductSelectionBranch(this)),
	currentDocument(nullptr)
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	innerLayout->addWidget(docInfo);
	innerLayout->addLayout(headerPanel);
	headerPanel->addWidget(summPaidField);
	headerPanel->addWidget(doctypeField);
	innerLayout->addWidget(searchField);
	innerLayout->addWidget(entriesView);
	innerLayout->addLayout(buttonsLayout);
	buttonsLayout->addWidget(backButton);
	buttonsLayout->addWidget(editButton);
	buttonsLayout->addWidget(deleteButton);
	buttonsLayout->addWidget(addButton);
	mainLayout->addWidget(entryCreation);
	mainLayout->addWidget(productSelection);

	mainLayout->setSpacing(0);
	innerLayout->setSpacing(0);
	buttonsLayout->setSpacing(0);
	headerPanel->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	buttonsLayout->setContentsMargins(0, 0, 0, 0);
	headerPanel->setContentsMargins(0, 0, 0, 0);

	current = innerWidget;
	untouchable = innerWidget;
	main = this;
	entryCreation->hide();
	productSelection->hide();

	docInfo->setFont(makeFont(0.02));
	
	summPaidField->setDecimals(2);
	summPaidField->setMinimum(0);
	summPaidField->setMaximum(1000000);

	doctypes = AppWorkset->dataprovider.loadEntitiesFromTable<NamedIdEntity>("Tips");
	for (NamedId dtype : doctypes)
	{
		doctypeField->addItem(dtype->name);
	}
	
	filterModel->setSourceModel(innerModel);
	filterModel->setFilterRole(DataEntityListModel::SearchRole);
	entriesView->setModel(filterModel);
	entriesView->setItemDelegate(new EntryDelegate(this));

	backButton->setText(tr("back"));
	editButton->setText(tr("edit"));
	deleteButton->setText(tr("delete"));
	addButton->setText(tr("add"));

	backButton->setIcon(QIcon(":/res/back.png"));
	editButton->setIcon(QIcon(":/res/pencil.png"));
	deleteButton->setIcon(QIcon(":/res/data.png"));
	addButton->setIcon(QIcon(":/res/plus.png"));

	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	editButton->setStyleSheet(CHANGE_BUTTONS_STYLESHEET);
	deleteButton->setStyleSheet(DELETE_BUTTONS_STYLESHEET);
	addButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);

	QObject::connect(searchField, &QLineEdit::textChanged, filterModel, &DataEntityFilterModel::setFilterFixedString);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &EntryRedactingSubbranch::finishRedacting);
	QObject::connect(editButton, &MegaIconButton::clicked, this, &EntryRedactingSubbranch::handleEdit);
	QObject::connect(deleteButton, &MegaIconButton::clicked, this, &EntryRedactingSubbranch::handleDelete);
	QObject::connect(addButton, &MegaIconButton::clicked, this, &EntryRedactingSubbranch::handleAdd);
	QObject::connect(entryCreation, &EntryCreationScreen::entryCreated, this, &EntryRedactingSubbranch::handleEditedEntry);
	QObject::connect(entryCreation, &EntryCreationScreen::backRequired, this, &EntryRedactingSubbranch::hideCurrent);
	QObject::connect(productSelection, &ProductSelectionBranch::backRequired, this, &EntryRedactingSubbranch::hideCurrent);
	QObject::connect(productSelection, &ProductSelectionBranch::selectionDone, this, &EntryRedactingSubbranch::handleSelectedProduct);

}

void EntryRedactingSubbranch::setDocument(Document doc)
{
	currentDocument = doc;
	summPaidField->setValue(doc->alreadyPaid);
	doctypeField->setCurrentIndex(findNamedId(doc->documentTypeName, doctypes));
	docInfo->setText(doc->clientName);
	innerModel->setData(AppWorkset->dataprovider.loadDataFilteredAs<DocumentEntryEntity>(
		QStringLiteral(" parentDocId = ") + QString::number(doc->documentId)
		));
	searchField->clear();
}

void EntryRedactingSubbranch::clearContents()
{
	currentDocument = nullptr;
	summPaidField->setValue(0);
	doctypeField->setCurrentIndex(0);
	docInfo->setText(QString());
	innerModel->reset();
}

void EntryRedactingSubbranch::handleDelete()
{
	if (entriesView->currentIndex().isValid())
	{
		int userResponse = QMessageBox::information(this,
			tr("Delete entry"), tr("Sure you want to delete?"),
			QMessageBox::Ok, QMessageBox::Cancel);
		if (userResponse != QMessageBox::Ok)
			return;
		DataEntity temp =
				entriesView->currentIndex().data(DataEntityListModel::DataCopyRole).value<DataEntity>();
		if (temp != nullptr)
		{
			AppWorkset->dataprovider.removeOneEntity(temp);
			innerModel->removeDataEntity(filterModel->mapToSource(entriesView->currentIndex()));
		}
	}
	entriesView->clearSelection();
}

void EntryRedactingSubbranch::handleEdit()
{
	if (entriesView->currentIndex().isValid())
	{
		DocumentEntry temp = std::dynamic_pointer_cast<DocumentEntryEntity>(
			entriesView->currentIndex().data(DataEntityListModel::DataCopyRole).value<DataEntity>());
		if (temp == nullptr)
			return;
		entryCreation->primeEntryCreation(temp);
		_hideAny(entryCreation);
	}
	entriesView->clearSelection();
}

void EntryRedactingSubbranch::handleEditedEntry(DocumentEntry e)
{
	AppWorkset->dataprovider.replaceData(e);
	innerModel->setData(AppWorkset->dataprovider.loadDataFilteredAs<DocumentEntryEntity>(
		QStringLiteral(" parentDocId = ") + QString::number(currentDocument->documentId)
		));
	searchField->clear();
	_hideCurrent(untouchable);
}

void EntryRedactingSubbranch::handleAdd()
{
	if (currentDocument == nullptr)
		return;
	productSelection->primeSelection(AppWorkset->dataprovider.loadEntityById<ClientEntity>(currentDocument->clientId));
	_hideAny(productSelection);
}

void EntryRedactingSubbranch::handleSelectedProduct(Product p)
{
	if (currentDocument == nullptr || p == nullptr)
		return;
	entryCreation->primeEntryCreation(p, currentDocument);
	_hideAny(entryCreation);
}

void EntryRedactingSubbranch::hideCurrent()
{
	_hideCurrent(untouchable);
}

void EntryRedactingSubbranch::finishRedacting()
{
	bool isReplaceRequired = false;
	if (currentDocument->alreadyPaid != summPaidField->value())
	{
		isReplaceRequired = true;
		currentDocument->alreadyPaid = summPaidField->value();
	}
		int index = findNamedId(doctypeField->currentText(), doctypes);
		if (index != -1)
		{
			if (currentDocument->documentType != doctypes.at(index)->id)
			{
				isReplaceRequired = true;
				currentDocument->documentType = doctypes.at(index)->id;
				currentDocument->documentTypeName = doctypes.at(index)->name;
			}
		}

		if (isReplaceRequired)
			emit editingFinished(currentDocument);
		else
			emit backRequired();
}

