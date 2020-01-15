#include "EntryCreationScreen.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
#include "Widgets/utils/ElementsStyles.h"

const QString entryLoadQuery
= QStringLiteral(" parentDocId = %1 and productId = %2");

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
	this->setFont(makeFont(1));
	mainLayout->addWidget(productInfo);
	mainLayout->addStretch(0);
	mainLayout->addLayout(formLayout);
	mainLayout->addStretch(0);
	mainLayout->addLayout(buttonLayout);
	formLayout->setLabelAlignment(Qt::AlignHCenter);
	formLayout->addRow(tr("Price: "), priceInfo);
	formLayout->addRow(tr("Measure: "), measureField);
	formLayout->addRow(tr("Quantity: "), quantitySpinbox);
	formLayout->addRow(tr("Option: "), foptionField);
	//formLayout->addRow(tr("Option: "), soptionField);
	//formLayout->addRow(tr("Option: "), toptionField);
	formLayout->addRow(tr("Comment: "), commentField);
	buttonLayout->addWidget(backButton);
	buttonLayout->addWidget(okButton);
	formLayout->setContentsMargins(0, 6, 0, 6);
	formLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	buttonLayout->setSpacing(0);
	buttonLayout->setContentsMargins(0, 0, 0, 0);
#ifdef Q_OS_ANDROID
	productInfo->setFont(makeFont(2));
#else
	productInfo->setFont(makeFont(1.3));
#endif
	productInfo->setAlignment(Qt::AlignCenter);
	productInfo->setWordWrap(true);
	quantitySpinbox->setMinimum(0);
	quantitySpinbox->setMaximum(1000000);
	backButton->setText(tr("back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	backButton->setIcon(QIcon(":/res/back.png"));
	okButton->setText(tr("Ok"));
	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	okButton->setIcon(QIcon(":/res/submit.png"));

	measures = AppWorkset->dataprovider.loadEntities<NamedIdEntity>(QString::null, "Measures");
	options = AppWorkset->dataprovider.loadEntities<NamedIdEntity>(QString::null, "Options");
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

	soptionField->hide();
	toptionField->hide();

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
	currentEntry = AppWorkset->dataprovider.loadEntityAs<DocumentEntryEntity>(
		entryLoadQuery.arg(doc->documentId).arg(p->id)
		);
	if (currentEntry->entryId == 0)
	{
		currentEntry->productName = p->name;
		currentEntry->entryId = IdGenerator::generateId();
		currentEntry->parentDocId = doc->documentId;
		currentEntry->price = p->price;
		currentEntry->productId = p->id;
	}
	else
	{
		priceInfo->setText(QString::number(operatedProduct->price));
		productInfo->setText(operatedProduct->name);
		int index = findNamedId(currentEntry->measure, measures);
		if (index != -1)
			measureField->setCurrentIndex(index);
		index = findNamedId(currentEntry->option1, options);
		if (index != -1)
			foptionField->setCurrentIndex(index);
		index = findNamedId(currentEntry->option2, options);
		if (index != -1)
			soptionField->setCurrentIndex(index);
		index = findNamedId(currentEntry->option3, options);
		if (index != -1)
			toptionField->setCurrentIndex(index);
		quantitySpinbox->setDValue(currentEntry->quantity);
		commentField->setText(currentEntry->comment);
	}
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