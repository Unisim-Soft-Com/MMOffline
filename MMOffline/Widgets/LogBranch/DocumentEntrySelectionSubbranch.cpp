#include "DocumentEntrySelectionSubbranch.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Widgets/ExtendedDelegates/EntryDelegate.h"
#include "Dataprovider/SqliteDataProvider.h"
#include <qmessagebox.h>
#include <QScroller>

DocumentRedactingSubbranch::DocumentRedactingSubbranch(QWidget* parent)
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
	// emplacing widgets
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

	// saving space by removing margins
	mainLayout->setSpacing(0);
	innerLayout->setSpacing(0);
	buttonsLayout->setSpacing(0);
	headerPanel->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	buttonsLayout->setContentsMargins(0, 0, 0, 0);
	headerPanel->setContentsMargins(0, 0, 0, 0);
	headerPanel->setMargin(2);

	// activating abstract node interfaces. innerWidget now considered inner-view of this widget
	current = innerWidget;
	untouchable = innerWidget;
	main = this;
	// hiding not required widgets
	entryCreation->hide();
	productSelection->hide();
	
	//scaling fonts
	entriesView->setFont(QFont("Times new Roman", 20, 20));
	docInfo->setFont(makeFont(1.5));

	// setting labels and buttons appearance
	docInfo->setAlignment(Qt::AlignCenter);
	docInfo->setWordWrap(true);
	searchInfo->setText(tr("Search:"));
	searchInfo->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
	
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

	// setting up spinbox options
	summPaidField->setDecimals(2);
	summPaidField->setMinimum(0);
	summPaidField->setMaximum(1000000);
	summPaidField->setButtonSymbols(QAbstractSpinBox::NoButtons);

	// loading combo box data
	doctypes = AppData->loadEntities<NamedIdEntity>(QString::null, "Tips");
	for (NamedId dtype : doctypes)
	{
		doctypeField->addItem(dtype->name);
	}

	// setting up models and view
	filterModel->setSourceModel(innerModel);
	filterModel->setFilterRole(DataEntityListModel::SearchRole);
	entriesView->setModel(filterModel);
	
	// changing view appearance and adding scroll	
	QScroller::grabGesture(entriesView, QScroller::LeftMouseButtonGesture);
	entriesView->setVerticalScrollMode(QListView::ScrollPerPixel);
	entriesView->setItemDelegate(new EntryDelegate(this));

	// connecting slots
	QObject::connect(searchField, &QLineEdit::textChanged, filterModel, &DataEntityFilterModel::setFilterFixedString);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &DocumentRedactingSubbranch::finishRedacting);
	QObject::connect(editButton, &MegaIconButton::clicked, this, &DocumentRedactingSubbranch::handleEdit);
	QObject::connect(deleteButton, &MegaIconButton::clicked, this, &DocumentRedactingSubbranch::handleDelete);
	QObject::connect(addButton, &MegaIconButton::clicked, this, &DocumentRedactingSubbranch::handleAdd);
	QObject::connect(entryCreation, &EntryCreationScreen::entryCreated, this, &DocumentRedactingSubbranch::handleEditedEntry);
	QObject::connect(entryCreation, &EntryCreationScreen::backRequired, this, &DocumentRedactingSubbranch::hideCurrent);
	QObject::connect(productSelection, &ProductSelectionBranch::backRequired, this, &DocumentRedactingSubbranch::hideCurrent);
	QObject::connect(productSelection, &ProductSelectionBranch::selectionDone, this, &DocumentRedactingSubbranch::handleSelectedProduct);
	QObject::connect(entriesView, &QListView::doubleClicked, this, &DocumentRedactingSubbranch::handleEdit);
	QObject::connect(searchField, &QLineEdit::returnPressed, qApp->inputMethod(), &QInputMethod::hide);
	// on android prediction is disabled for search string to allow instant search
#ifdef Q_OS_ANDROID
	searchField->setInputMethodHints(Qt::InputMethodHint::ImhNoPredictiveText);
#endif
}

void DocumentRedactingSubbranch::setDocument(Document doc)
{
	currentDocument = doc;
	summPaidField->setValue(doc->alreadyPaid);
	doctypeField->setCurrentIndex(findNamedId(doc->documentTypeName, doctypes));
	docInfo->setText(doc->clientName);
	innerModel->setData(AppData->loadDataAs(DataEntity(new DocumentEntryEntity()),
		QStringLiteral(" parentDocId = ") + QString::number(doc->documentId)
		));
	searchField->clear();
}

void DocumentRedactingSubbranch::clearContents()
{
	currentDocument = nullptr;
	summPaidField->setValue(0);
	doctypeField->setCurrentIndex(0);
	docInfo->setText(QString());
	innerModel->reset();
}

void DocumentRedactingSubbranch::handleDelete()
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
			AppData->removeOneEntity(temp);
			innerModel->removeDataEntity(filterModel->mapToSource(entriesView->currentIndex()));
		}
	}
	entriesView->clearSelection();
}

void DocumentRedactingSubbranch::handleEdit()
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

void DocumentRedactingSubbranch::handleEditedEntry(DocumentEntry e)
{
	AppData->replaceData(e);
	innerModel->setData(AppData->loadDataAs(DataEntity(new DocumentEntryEntity()),
		QStringLiteral(" parentDocId = ") + QString::number(currentDocument->documentId)
		));
	searchField->clear();
	_hideCurrent(untouchable);
}

void DocumentRedactingSubbranch::handleAdd()
{
	if (currentDocument == nullptr)
		return;
	productSelection->primeSelection(AppData->loadEntityById<ClientEntity>(currentDocument->clientId));
	_hideAny(productSelection);
}

void DocumentRedactingSubbranch::handleSelectedProduct(Product p)
{
	if (currentDocument == nullptr || p == nullptr)
		return;
	entryCreation->primeEntryCreation(p, currentDocument);
	_hideAny(entryCreation);
}

void DocumentRedactingSubbranch::hideCurrent()
{
	_hideCurrent(untouchable);
}

void DocumentRedactingSubbranch::finishRedacting()
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