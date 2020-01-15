#include "DocumentEntrySelectionSubbranch.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Widgets/ExtendedDelegates/EntryDelegate.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
#include <qmessagebox.h>
#include <QScroller>

EntryRedactingSubbranch::EntryRedactingSubbranch(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	docInfo(new QLabel(innerWidget)), headerPanel(new QHBoxLayout(innerWidget)),
	summPaidField(new QDoubleSpinBox(innerWidget)), doctypeField(new QComboBox(innerWidget)),
	searchPanel(new QHBoxLayout(innerWidget)), searchInfo(new QLabel(innerWidget)),
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
	innerLayout->addLayout(searchPanel);
	searchPanel->addWidget(searchInfo);
	searchPanel->addWidget(searchField);
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
	headerPanel->setMargin(2);
	current = innerWidget;
	untouchable = innerWidget;
	main = this;
	entryCreation->hide();
	productSelection->hide();

	entriesView->setFont(QFont("Times new Roman", 20, 20));
	docInfo->setFont(makeFont(1.5));
	docInfo->setAlignment(Qt::AlignCenter);
	docInfo->setWordWrap(true);

	summPaidField->setDecimals(2);
	summPaidField->setMinimum(0);
	summPaidField->setMaximum(1000000);
	summPaidField->setButtonSymbols(QAbstractSpinBox::NoButtons);

	doctypes = AppWorkset->dataprovider.loadEntities<NamedIdEntity>(QString::null, "Tips");
	for (NamedId dtype : doctypes)
	{
		doctypeField->addItem(dtype->name);
	}

	searchInfo->setText(tr("Search:"));
	searchInfo->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

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

	QScroller::grabGesture(entriesView, QScroller::LeftMouseButtonGesture);
	entriesView->setVerticalScrollMode(QListView::ScrollPerPixel);
	QObject::connect(searchField, &QLineEdit::textChanged, filterModel, &DataEntityFilterModel::setFilterFixedString);
#ifdef Q_OS_ANDROID
	searchField->setInputMethodHints(Qt::InputMethodHint::ImhNoPredictiveText);
#endif
	QObject::connect(backButton, &MegaIconButton::clicked, this, &EntryRedactingSubbranch::finishRedacting);
	QObject::connect(editButton, &MegaIconButton::clicked, this, &EntryRedactingSubbranch::handleEdit);
	QObject::connect(deleteButton, &MegaIconButton::clicked, this, &EntryRedactingSubbranch::handleDelete);
	QObject::connect(addButton, &MegaIconButton::clicked, this, &EntryRedactingSubbranch::handleAdd);
	QObject::connect(entryCreation, &EntryCreationScreen::entryCreated, this, &EntryRedactingSubbranch::handleEditedEntry);
	QObject::connect(entryCreation, &EntryCreationScreen::backRequired, this, &EntryRedactingSubbranch::hideCurrent);
	QObject::connect(productSelection, &ProductSelectionBranch::backRequired, this, &EntryRedactingSubbranch::hideCurrent);
	QObject::connect(productSelection, &ProductSelectionBranch::selectionDone, this, &EntryRedactingSubbranch::handleSelectedProduct);
	QObject::connect(entriesView, &QListView::doubleClicked, this, &EntryRedactingSubbranch::handleEdit);
	QObject::connect(searchField, &QLineEdit::returnPressed, qApp->inputMethod(), &QInputMethod::hide);
}

void EntryRedactingSubbranch::setDocument(Document doc)
{
	currentDocument = doc;
	summPaidField->setValue(doc->alreadyPaid);
	doctypeField->setCurrentIndex(findNamedId(doc->documentTypeName, doctypes));
	docInfo->setText(doc->clientName);
	innerModel->setData(AppWorkset->dataprovider.loadDataAs<DocumentEntryEntity>(
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
	innerModel->setData(AppWorkset->dataprovider.loadDataAs<DocumentEntryEntity>(
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