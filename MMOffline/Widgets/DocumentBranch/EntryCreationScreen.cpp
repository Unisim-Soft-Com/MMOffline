#include "EntryCreationScreen.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
#include "Widgets/utils/ElementsStyles.h"



EntryCreationScreen::EntryCreationScreen(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	productInfo(new QLabel(this)), formLayout(new QFormLayout(this)),
	priceInfo(new QLabel(this)), measureField(new QComboBox(this)),
	quantitySpinbox(new BigButtonsSpinbox(BigButtonsSpinbox::floatspin, this)),
	foptionField(new QComboBox(this)), soptionField(new QComboBox(this)),
	toptionField(new QComboBox(this)), commentField(new QLineEdit(this)),
	buttonLayout(new QHBoxLayout(this)), backButton(new MegaIconButton(this)),
	okButton(new MegaIconButton(this)), measures(), options(), operatedProduct()
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(productInfo);
	mainLayout->addLayout(formLayout);
	mainLayout->addLayout(buttonLayout);
	formLayout->addRow(tr("Price: "), priceInfo);
	formLayout->addRow(tr("Measure: "), measureField);
	formLayout->addRow(tr("Quantity: "), quantitySpinbox);
	formLayout->addRow(tr("Option: "), foptionField);
	formLayout->addRow(tr("Option: "), soptionField);
	formLayout->addRow(tr("Option: "), toptionField);
	formLayout->addRow(tr("Comment: "), commentField);
	buttonLayout->addWidget(backButton);
	buttonLayout->addWidget(okButton);

	quantitySpinbox->setMinimum(0);
	quantitySpinbox->setMaximum(1000000);
	backButton->setText(tr("back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	backButton->setIcon(QIcon(":/res/back.png"));
	okButton->setText(tr("Ok"));
	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	okButton->setIcon(QIcon(":/res/submit.png"));

	measures = AppWorkset->dataprovider.loadEntitiesFromTable<NamedIdEntity>("Measures");
	options = AppWorkset->dataprovider.loadEntitiesFromTable<NamedIdEntity>("Options");
	for (NamedId& id : measures)
	{
		measureField->addItem(id->name);
	}
	for (NamedId& id : options)
	{
		foptionField->addItem(id->name);
		soptionField->addItem(id->name);
		toptionField->addItem(id->name);
	}

	QObject::connect(okButton, &MegaIconButton::clicked, this, &EntryCreationScreen::confirmed);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &EntryCreationScreen::backRequired);

}

void EntryCreationScreen::primeEntryCreation(Product p, Document doc)
{
	if (p == nullptr || doc == nullptr)
		return;
	operatedProduct.reset(new ProductEntity(*p));
	priceInfo->setText(QString::number(operatedProduct->price));
	productInfo->setText(operatedProduct->name);
	currentEntry.reset(new DocumentEntryEntity());
	currentEntry->productName = p->name;
	currentEntry->entryId = IdGenerator::generateId();
	currentEntry->parentDocId = doc->documentId;
	currentEntry->price = p->price;
	currentEntry->productId = p->id;
	
}

void EntryCreationScreen::primeEntryCreation(DocumentEntry e)
{
	if (e == nullptr)
		return;
	operatedProduct = AppWorkset->dataprovider.loadEntityById<ProductEntity>(e->productId);
	Document doc = AppWorkset->dataprovider.loadEntityById<DocumentEntity>(e->parentDocId);
	if (operatedProduct == nullptr || doc == nullptr)
	{
		emit backRequired();
		return;
	}
	priceInfo->setText(QString::number(operatedProduct->price));
	productInfo->setText(operatedProduct->name);
	currentEntry = e;
	int index = findNamedId(e->measure, measures);
	if (index != -1)
		measureField->setCurrentIndex(index);
	index = findNamedId(e->option1, options);
	if (index != -1)
		foptionField->setCurrentIndex(index);
	index = findNamedId(e->option2, options);
	if (index != -1)
		soptionField->setCurrentIndex(index);
	index = findNamedId(e->option3, options);
	if (index != -1)
		toptionField->setCurrentIndex(index);
	quantitySpinbox->setDValue(e->quantity);
	commentField->setText(e->comment);

}

void EntryCreationScreen::show()
{
	quantitySpinbox->setFocus();
	inframedWidget::show();
}

void EntryCreationScreen::confirmed()
{
	currentEntry->comment = commentField->text();
	currentEntry->quantity = quantitySpinbox->dvalue();
	int index = findNamedId(measureField->currentText(), measures);
	if (index != -1)
		currentEntry->measure = measures.at(index)->id;
	index = findNamedId(foptionField->currentText(), options);
	if (index != -1)
		currentEntry->option1 = options.at(index)->id;
	index = findNamedId(soptionField->currentText(), options);
	if (index != -1)
		currentEntry->option2 = options.at(index)->id;
	index = findNamedId(toptionField->currentText(), options);
	if (index != -1)
		currentEntry->option3 = options.at(index)->id;
	quantitySpinbox->clear();
	commentField->clear();
	emit entryCreated(currentEntry);
}
