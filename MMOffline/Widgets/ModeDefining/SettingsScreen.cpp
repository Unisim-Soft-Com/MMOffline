#include "SettingsScreen.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Dataprovider/SqliteDataProvider.h"
#include "Dataprovider/FileParser.h"
#include "Networking/dataupdateengine-http.h"

// this template is used during version showing
const QString VERSION_TEMPLATE = QStringLiteral("UNAOrders %1 %2");

void SettingsScreen::fillTexts()
{
	urlBaseInfo->setText(tr("url for sync"));
	makeLoadFromFile->setText(tr("Load from file"));
	translateButton->setText(tr("Translate"));
	translateButton->setIcon(langIcons[currentLanguage]);
	timeoutInfo->setText(tr("Timeout interval"));
}
SettingsScreen::SettingsScreen(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	appInfo(new QLabel(this)),
	urlBaseInfo(new QLabel(this)), urlBaseField(new QComboBox(this)),
	buttonsLayout(new QHBoxLayout(this)), backButton(new MegaIconButton(this)),
	okButton(new MegaIconButton(this)), filepathField(new QLineEdit(this)),
	makeLoadFromFile(new MegaIconButton(this)), translateButton(new MegaIconButton(this)),
	timeoutInfo(new QLabel(this)),
	timeoutField(new QSpinBox(this)), debugMode(new MegaIconButton(this)),
	errorLog(new AssertionLabel(this)),
	langIcons{ QIcon(":/res/eng.png"), QIcon(":/res/ro.png"), QIcon(":/res/ru.png") },
	currentLanguage(Translations::langFromString(AppSettings->language))
{
	// allocating widgets on layout
	this->setLayout(mainLayout);
	mainLayout->addWidget(appInfo);
	mainLayout->addWidget(urlBaseInfo);
	mainLayout->addWidget(urlBaseField);
	mainLayout->addWidget(filepathField);
	mainLayout->addWidget(makeLoadFromFile);
	mainLayout->addWidget(translateButton);
	mainLayout->addWidget(timeoutInfo);
	mainLayout->addWidget(timeoutField);
	mainLayout->addWidget(debugMode);
	mainLayout->addWidget(errorLog);
	mainLayout->addStretch(0);
	mainLayout->addLayout(buttonsLayout);
	buttonsLayout->addWidget(backButton);
	buttonsLayout->addWidget(okButton);

	// setting up version label
	appInfo->setFont(makeFont(1.25));
	appInfo->setText(VERSION_TEMPLATE.arg(VERSION).arg(SUFFIX));
	appInfo->setAlignment(Qt::AlignCenter);

	// setting up url field
	urlBaseField->setEditable(true);
	urlBaseField->setInsertPolicy(QComboBox::InsertAtTop);
	if (!GlobalAppSettings::instance()->HttpUrl.isEmpty())
		urlBaseField->addItem(GlobalAppSettings::instance()->HttpUrl);
	if (GlobalAppSettings::instance()->alternativeUrls.count() > 0)
		urlBaseField->addItems(GlobalAppSettings::instance()->alternativeUrls);
	urlBaseField->setCurrentIndex(0);

	// setting up other fields
	filepathField->setText(AppSettings->filePath);

	timeoutField->setMinimum(0);
	timeoutField->setMaximum(10000000);
	timeoutField->setValue(AppSettings->timeoutint);

	// setting up buttons
	debugMode->setIcon(QIcon(":/res/check.png"));
	debugMode->setText("DEBUG MODE");
	debugMode->setMaximumHeight(calculateAdaptiveButtonHeight(0.08));
	debugMode->setCheckable(true);
	debugMode->setChecked(AppSettings->dbTracing);
	debugMode->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);

	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	backButton->setIcon(QIcon(":/res/back.png"));

	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	okButton->setIcon(QIcon(":/res/submit.png"));

	makeLoadFromFile->setIcon(QIcon(":/res/viaFile.png"));

	// applying current language
	currentLanguage = Translations::langFromString(AppSettings->language);
	fillTexts();

	// connecting slots
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
	AppSettings->timeoutint = timeoutField->value();
	AppSettings->packetTracing = AppSettings->dbTracing = debugMode->isChecked();
	AppNetwork->setUrl(GlobalAppSettings::instance()->HttpUrl);
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