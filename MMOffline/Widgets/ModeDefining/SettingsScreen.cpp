#include "SettingsScreen.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
SettingsScreen::SettingsScreen(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	urlBaseInfo(new QLabel(this)), urlBaseField(new QComboBox(this)),
	buttonsLayout(new QHBoxLayout(this)), backButton(new MegaIconButton(this)),
	okButton(new MegaIconButton(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(urlBaseInfo);
	mainLayout->addWidget(urlBaseField);
	mainLayout->addLayout(buttonsLayout);
	buttonsLayout->addWidget(backButton);
	buttonsLayout->addWidget(okButton);

	urlBaseInfo->setText(tr("url for sync"));

	urlBaseField->setEditable(true);
	urlBaseField->setInsertPolicy(QComboBox::InsertAtTop);
	urlBaseField->addItems(GlobalAppSettings::instance()->alternativeUrls);
	urlBaseField->addItem(GlobalAppSettings::instance()->HttpUrl);
	urlBaseField->setCurrentIndex(0);

	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	backButton->setIcon(QIcon(":/res/back.png"));

	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	okButton->setIcon(QIcon(":/res/ok.png"));

	QObject::connect(okButton, &MegaIconButton::clicked, this, &SettingsScreen::allConfirmed);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &SettingsScreen::backRequired);
	QObject::connect(urlBaseField, QOverload<const QString&>::of(&QComboBox::activated), this, &SettingsScreen::urlPicked);

}

void SettingsScreen::allConfirmed()
{
	GlobalAppSettings::instance()->HttpUrl= (urlBaseField->currentText().isEmpty()) ? GlobalAppSettings::instance()->HttpUrl : urlBaseField->currentText();
	if (!GlobalAppSettings::instance()->alternativeUrls.contains(GlobalAppSettings::instance()->HttpUrl))
	{
		GlobalAppSettings::instance()->alternativeUrls << GlobalAppSettings::instance()->HttpUrl;
		GlobalAppSettings::instance()->dump();
		urlBaseField->addItem(GlobalAppSettings::instance()->HttpUrl);
	}
	globalWorkset.networkingEngine->setUrl(GlobalAppSettings::instance()->HttpUrl);
}

void SettingsScreen::urlPicked(QString url)
{
	GlobalAppSettings::instance()->HttpUrl = url;
}
