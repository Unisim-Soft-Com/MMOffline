#include "StartingScreen.h"
#include <QApplication>

StartingScreen::StartingScreen(QWidget* parent)
	: inframedWidget(parent), abstractNode(), mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	infoLabel(new QLabel(innerWidget)), onlineLoginButton(new MegaIconButton(innerWidget)),
	createDocumentButton(new MegaIconButton(innerWidget)),
	logsButton(new MegaIconButton(innerWidget)), footerLayout(new QHBoxLayout(innerWidget)),
	settingsButton(new MegaIconButton(innerWidget)), quitButton(new MegaIconButton(innerWidget)),
	onlineLogin(new OnlineLoginWidget(this)), settings(new SettingsScreen(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	mainLayout->addWidget(onlineLogin);
	mainLayout->addWidget(settings);
	innerWidget->setLayout(innerLayout);
	innerLayout->addWidget(infoLabel);
	innerLayout->addWidget(createDocumentButton);
	innerLayout->addWidget(onlineLoginButton);
	innerLayout->addWidget(logsButton);
	innerLayout->addLayout(footerLayout);
	footerLayout->addWidget(quitButton);
	footerLayout->addWidget(settingsButton);

	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);
	footerLayout->setContentsMargins(0, 0, 0, 0);
	footerLayout->setSpacing(0);

	current = innerWidget;
	untouchable = innerWidget;
	main = this;
	settings->hide();
	onlineLogin->hide();

	infoLabel->setAlignment(Qt::AlignCenter);
	infoLabel->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	infoLabel->setWordWrap(true);

	createDocumentButton->setIcon(QIcon(":/res/add.png"));
	onlineLoginButton->setIcon(QIcon(":/res/login.png"));
	logsButton->setIcon(QIcon(":/res/mlogs.png"));
	settingsButton->setIcon(QIcon(":/res/settings.png"));
	quitButton->setIcon(QIcon(":/res/exit.png"));
	fillTexts();
	QObject::connect(createDocumentButton, &MegaIconButton::clicked, this, &StartingScreen::documentCreationInitiated);
	QObject::connect(onlineLoginButton, &MegaIconButton::clicked, this, &StartingScreen::toOnlineLogin);
	QObject::connect(logsButton, &MegaIconButton::clicked, this, &StartingScreen::logsRequired);
	QObject::connect(settingsButton, &MegaIconButton::clicked, this, &StartingScreen::toSettings);
	QObject::connect(onlineLogin, &OnlineLoginWidget::backRequired, this, &StartingScreen::hideCurrent);
	QObject::connect(settings, &SettingsScreen::backRequired, this, &StartingScreen::hideCurrent);
	QObject::connect(onlineLogin, &OnlineLoginWidget::loginReady, this, &StartingScreen::userLoggedIn);
	QObject::connect(quitButton, &MegaIconButton::clicked, qApp, QApplication::quit);
	QObject::connect(settings, &SettingsScreen::translating, this, &StartingScreen::translationHappened);
}

void StartingScreen::fillTexts()
{
	createDocumentButton->setText(tr("New document"));
	onlineLoginButton->setText(tr("Login"));
	logsButton->setText(tr("Logs"));
	settingsButton->setText(tr("Settings"));
	quitButton->setText(tr("Quit!"));
	setInfoLabel();
}

void StartingScreen::toOnlineLogin()
{
	_hideAny(onlineLogin);
}

void StartingScreen::toSettings()
{
	_hideAny(settings);
}

void StartingScreen::hideCurrent()
{
	_hideCurrent(innerWidget);
}

void StartingScreen::userLoggedIn(QString login)
{
	setInfoLabel();
	_hideCurrent(innerWidget);
}

void StartingScreen::translationHappened(int)
{
	fillTexts();
	onlineLogin->fillTexts();
}

void StartingScreen::setInfoLabel()
{
	if (GlobalAppSettings::instance()->localLogin.isEmpty())
	{
		infoLabel->setText(tr("Currently no offline user is registered"));
	}
	else
	{
		infoLabel->setText(tr("Currently used offline user: ") + GlobalAppSettings::instance()->localLogin);
	}
}