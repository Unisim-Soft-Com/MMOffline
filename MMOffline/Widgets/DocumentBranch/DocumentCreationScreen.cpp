#include "DocumentCreationScreen.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
#include "Widgets/utils/ElementsStyles.h"




DocumentCreationScreen::DocumentCreationScreen(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	clientInfo(new QLabel(this)), formLayout(new QFormLayout(this)),
	idInfo(new QLabel(this)),
	dateSpinBox(new QDateEdit(this)),
	depozitField(new QComboBox(this)), tipField(new QComboBox(this)),
	contactField(new QLineEdit(this)),
	summPaidField(new QSpinBox(this)),
	buttonLayout(new QHBoxLayout(this)), backButton(new MegaIconButton(this)),
	okButton(new MegaIconButton(this)), depozits(), tips(), currentDocument(nullptr)
{
	this->setLayout(mainLayout);
	this->setFont(QFont("Times New Roman", 17, 17));
	mainLayout->addWidget(clientInfo);
	formLayout->setContentsMargins(4, 4, 5, 5);
	clientInfo->setAlignment(Qt::AlignCenter);
	mainLayout->addLayout(formLayout);
	mainLayout->addLayout(buttonLayout);
	formLayout->addRow(tr("Client id: "), idInfo);
	formLayout->addRow(tr("Shipping date"), dateSpinBox);
	formLayout->addRow(tr("depozit"), depozitField);
	formLayout->addRow(tr("tip"), tipField);
	//formLayout->addRow(tr("contact"), contactField);
	formLayout->addRow(tr("summ paid"), summPaidField);
	formLayout->setRowWrapPolicy(QFormLayout::RowWrapPolicy::WrapLongRows);
	formLayout->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy::AllNonFixedFieldsGrow);
	formLayout->setFormAlignment(Qt::AlignCenter | Qt::AlignTop);
	formLayout->setLabelAlignment(Qt::AlignRight);
	dateSpinBox->setCalendarPopup(true);
	buttonLayout->addWidget(backButton);
	buttonLayout->addWidget(okButton);
	contactField->hide();
	depozits = AppWorkset->dataprovider.loadEntitiesFromTable<NamedIdEntity>("Depozits");
	tips = AppWorkset->dataprovider.loadEntitiesFromTable<NamedIdEntity>("Tips");

	okButton->setText(tr("Ok"));
	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	okButton->setIcon(QIcon(":/res/submit.png"));
	backButton->setText(tr("back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	summPaidField->setMinimum(0);
	summPaidField->setMaximum(50000);
	for (NamedId dep : depozits)
	{
		depozitField->addItem(dep->name);
	}
	for (NamedId tip : tips)
	{
		tipField->addItem(tip->name);
	}
	QObject::connect(okButton, &MegaIconButton::clicked, this, &DocumentCreationScreen::okPressed);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &DocumentCreationScreen::backRequired);
}

void DocumentCreationScreen::primeCreation(Client client)
{
	if (client == nullptr)
	{
		clientInfo->setText(tr("No client"));
		idInfo->setText("00000000");
	}
	else
	{
		clientInfo->setText(client->name);
		idInfo->setText(QString::number(client->id));
		currentDocument.reset(new DocumentEntity( IdGenerator::generateId()));
		currentDocument->clientId = client->id;
		currentDocument->clientName = client->name;
		currentDocument->dateWhenCreated = QDateTime::currentDateTime();
		dateSpinBox->setDate(currentDocument->dateWhenCreated.addDays(1).date());
	}
}

void DocumentCreationScreen::show()
{
	summPaidField->setFocus();
	inframedWidget::show();
}

void DocumentCreationScreen::okPressed()
{
	if (currentDocument == nullptr)
	{
		clientInfo->setText(tr("Can not create doc without client"));
	}
	else
	{
		currentDocument->shippingDate = dateSpinBox->date();
		int index = findNamedId(depozitField->currentText(), depozits);
		if (index != -1)
		{
			currentDocument->warehouseId = depozits.at(index)->id;
			currentDocument->warehouseName = depozits.at(index)->name;
		}
		index = findNamedId(tipField->currentText(), tips);
		if (index != -1)
		{
			currentDocument->documentType = tips.at(index)->id;
			currentDocument->documentTypeName = tips.at(index)->name;
		}
		currentDocument->alreadyPaid = summPaidField->value();
		summPaidField->setValue(0);
		emit documentCreated(currentDocument);
	}
}
