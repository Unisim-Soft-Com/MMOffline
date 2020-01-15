#include "SettingsScreen.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
#include "Dataprovider/FileParser.h"
void SettingsScreen::fillTexts()
{
	urlBaseInfo->setText(tr("url for sync"));
	makeLoadFromFile->setText(tr("Load from file"));
	translateButton->setText(tr("Translate"));
	translateButton->setIcon(langIcons[currentLanguage]);
}
SettingsScreen::SettingsScreen(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	urlBaseInfo(new QLabel(this)), urlBaseField(new QComboBox(this)),
	buttonsLayout(new QHBoxLayout(this)), backButton(new MegaIconButton(this)),
	okButton(new MegaIconButton(this)), filepathField(new QLineEdit(this)),
	makeLoadFromFile(new MegaIconButton(this)), translateButton(new MegaIconButton(this)),
	errorLog(new AssertionLabel(this)),
	langIcons{ QIcon(":/res/eng.png"), QIcon(":/res/ro.png"), QIcon(":/res/ru.png") },
	currentLanguage(Translations::langFromString(AppSettings->language))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(urlBaseInfo);
	mainLayout->addWidget(urlBaseField);
	mainLayout->addWidget(filepathField);
	mainLayout->addWidget(makeLoadFromFile);
	mainLayout->addWidget(translateButton);
	mainLayout->addWidget(errorLog);
	mainLayout->addStretch(0);
	mainLayout->addLayout(buttonsLayout);
	buttonsLayout->addWidget(backButton);
	buttonsLayout->addWidget(okButton);

	urlBaseField->setEditable(true);
	urlBaseField->setInsertPolicy(QComboBox::InsertAtTop);
	if (GlobalAppSettings::instance()->alternativeUrls.count() > 0)
		urlBaseField->addItems(GlobalAppSettings::instance()->alternativeUrls);
	if (!GlobalAppSettings::instance()->HttpUrl.isEmpty())
		urlBaseField->addItem(GlobalAppSettings::instance()->HttpUrl);
	urlBaseField->setCurrentIndex(0);

	filepathField->setText(AppSettings->filePath);

	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	backButton->setIcon(QIcon(":/res/back.png"));

	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	okButton->setIcon(QIcon(":/res/submit.png"));

	makeLoadFromFile->setIcon(QIcon(":/res/viaFile.png"));

	currentLanguage = Translations::langFromString(AppSettings->language);

	fillTexts();

	QObject::connect(okButton, &MegaIconButton::clicked, this, &SettingsScreen::allConfirmed);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &SettingsScreen::backRequired);
	QObject::connect(urlBaseField, QOverload<const QString&>::of(&QComboBox::activated), this, &SettingsScreen::urlPicked);
	QObject::connect(filepathField, &QLineEdit::returnPressed, this, &SettingsScreen::loadFromFile);
	QObject::connect(makeLoadFromFile, &MegaIconButton::clicked, this, &SettingsScreen::loadFromFile);
	QObject::connect(translateButton, &MegaIconButton::clicked, this, &SettingsScreen::translateAll);
}

void SettingsScreen::allConfirmed()
{
	GlobalAppSettings::instance()->HttpUrl = (urlBaseField->currentText().isEmpty()) ? GlobalAppSettings::instance()->HttpUrl : urlBaseField->currentText();
	if (!GlobalAppSettings::instance()->alternativeUrls.contains(GlobalAppSettings::instance()->HttpUrl))
	{
		if (!GlobalAppSettings::instance()->HttpUrl.isEmpty())
			GlobalAppSettings::instance()->alternativeUrls << GlobalAppSettings::instance()->HttpUrl;
		GlobalAppSettings::instance()->dump();
		urlBaseField->addItem(GlobalAppSettings::instance()->HttpUrl);
	}
	AppSettings->filePath = filepathField->text();
	DataWorkset::instance()->networkingEngine->setUrl(GlobalAppSettings::instance()->HttpUrl);
	AppSettings->dump();
	emit backRequired();
}

void SettingsScreen::loadFromFile()
{
	if (filepathField->text().isEmpty())
	{
		return;
	}
	if (filepathField->text() == LOCAL_LOAD_PATH)
	{
		QDir dir(LOCAL_LOAD_PATH);
		if (!dir.exists())
			dir.mkdir(LOCAL_LOAD_PATH);
	}
	CsvFileParser parser(filepathField->text());
	parser.doParsing();
	errorLog->setErrorlist(parser.errorList());
}

void SettingsScreen::urlPicked(QString url)
{
	GlobalAppSettings::instance()->HttpUrl = url;
}

void SettingsScreen::translateAll()
{
	using namespace Translations;
	++currentLanguage;
	AppSettings->setTranslator(currentLanguage);
	fillTexts();
	emit translating(currentLanguage);
}