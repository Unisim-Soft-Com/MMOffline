#include "DocumentCreationScreen.h"
#include "Dataprovider/SqliteDataProvider.h"
#include "Widgets/utils/ElementsStyles.h"

DocumentCreationScreen::DocumentCreationScreen(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	clientInfo(new QLabel(this)), formLayout(new QFormLayout(this)),
	idInfo(new QLabel(this)),
	dateSpinBox(new BigButtonsSpinbox(BigButtonsSpinbox::datespin, this, 0.05)),
	depozitField(new QComboBox(this)), tipField(new QComboBox(this)),
	summPaidField(new BigButtonsSpinbox(BigButtonsSpinbox::floatspin, this)),
	buttonLayout(new QHBoxLayout(this)), backButton(new MegaIconButton(this)),
	okButton(new MegaIconButton(this)), depozits(), tips(), currentDocument(nullptr)
{
	// emplacing widgets
	this->setLayout(mainLayout);
	this->setFont(makeFont(1));
	mainLayout->addWidget(clientInfo);
	mainLayout->addStretch(0);
	mainLayout->addLayout(formLayout);
	mainLayout->addStretch(0);
	mainLayout->addLayout(buttonLayout);
	formLayout->setLabelAlignment(Qt::AlignTop);
	formLayout->addRow(tr("Client id: "), idInfo);
	formLayout->addRow(tr("Shipping date"), dateSpinBox);
	formLayout->addRow(tr("depozit"), depozitField);
	formLayout->addRow(tr("tip"), tipField);
	formLayout->addRow(tr("summ paid"), summPaidField);
	buttonLayout->addWidget(backButton);
	buttonLayout->addWidget(okButton);

	// removing margins and spacing to avoid space loss
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	buttonLayout->setSpacing(0);
	buttonLayout->setContentsMargins(0, 0, 0, 0);
	formLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
	formLayout->setContentsMargins(8, 8, 8, 8);

	// scale font to the size of the screen
#ifdef Q_OS_ANDROID
	clientInfo->setFont(makeFont(2));
#else
	clientInfo->setFont(makeFont(1.3));
#endif

	//setting labels and buttons appearance
	clientInfo->setWordWrap(true);
	clientInfo->setAlignment(Qt::AlignCenter);
	okButton->setText(tr("Ok"));
	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	okButton->setIcon(QIcon(":/res/submit.png"));
	backButton->setText(tr("back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	// setting spinbox options
	summPaidField->setMinimum(0);
	summPaidField->setMaximum(50000);
	summPaidField->setPrecision(2);

	// loading from database depozits and tips, then emplacing it into combo boxes
	depozits = AppData->loadEntities<NamedIdEntity>(QString::null, "Depozits");
	tips = AppData->loadEntities<NamedIdEntity>(QString::null, "Tips");

	for (NamedId dep : depozits)
	{
		depozitField->addItem(dep->name);
	}
	for (NamedId tip : tips)
	{
		tipField->addItem(tip->name);
	}
	// connecting buttons
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
		currentDocument.reset(new DocumentEntity(IdGenerator::generateId()));
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