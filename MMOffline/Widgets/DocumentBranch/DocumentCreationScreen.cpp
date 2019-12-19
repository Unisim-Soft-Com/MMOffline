#include "DocumentCreationScreen.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
DocumentCreationScreen::DocumentCreationScreen(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	clientInfo(new QLabel(this)), formLayout(new QFormLayout(this)),
	dateSpinBox(new BigButtonsSpinbox(BigButtonsSpinbox::datespin ,this)),
	depozitField(new QComboBox(this)), tipField(new QComboBox(this)),
	contactField(new QLineEdit(this)),
	summPaidField(new BigButtonsSpinbox(BigButtonsSpinbox::floatspin, this)),
	buttonLayout(new QHBoxLayout(this)), backButton(new MegaIconButton(this)),
	okButton(new MegaIconButton(this)), depozits(), tips(), currentDocument(nullptr)
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(clientInfo);
	mainLayout->addLayout(formLayout);
	mainLayout->addLayout(buttonLayout);
	formLayout->addRow(tr("creation date"), dateSpinBox);
	formLayout->addRow(tr("depozit"), depozitField);
	formLayout->addRow(tr("tip"), tipField);
	formLayout->addRow(tr("contact"), contactField);
	formLayout->addRow(tr("summ paid"), summPaidField);
	buttonLayout->addWidget(backButton);
	buttonLayout->addWidget(okButton);

	depozits = AppWorkset->dataprovider.loadEntitiesFromTable<NamedIdEntity>("Depozits");
	tips = AppWorkset->dataprovider.loadEntitiesFromTable<NamedIdEntity>("Tips");

	// TODO: finish this
}

void DocumentCreationScreen::primeCreation(Client)
{
}
