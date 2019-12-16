#include "StartingScreen.h"
#include <QApplication>
StartingScreen::StartingScreen(QWidget* parent)
	: inframedWidget(parent), abstractNode(), mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	infoLabel(new QLabel(innerWidget)), onlineLoginButton(new MegaIconButton(innerWidget)),
	createDocumentButton(new MegaIconButton(innerWidget)),
	logsButton(new MegaIconButton(innerWidget)), footerLayout(new QHBoxLayout(innerWidget)),
	settingsButton(new MegaIconButton(innerWidget)), quitButton(new MegaIconButton(innerWidget)),
	onlineLogin(new OnlineLoginWidget(this)), settings(new SettingsScreen(this)),
	logsScreen(new LogsWidget(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	mainLayout->addWidget(onlineLogin);
	mainLayout->addWidget(settings);
	mainLayout->addWidget(logsScreen);
	innerWidget->setLayout(innerLayout);
	innerLayout->addWidget(infoLabel);
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
	logsScreen->hide();

	infoLabel->setAlignment(Qt::AlignCenter);
	infoLabel->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	setInfoLabel();
	onlineLoginButton->setIcon(QIcon(":/res/login.png"));
	onlineLoginButton->setText(tr("Login"));
	logsButton->setIcon(QIcon(":/res/mlogs.png"));
	logsButton->setText(tr("Logs"));
	settingsButton->setIcon(QIcon(":/res/settings.png"));
	settingsButton->setText(tr("Settings"));
	quitButton->setIcon(QIcon(":/res/exit.png"));
	quitButton->setText(tr("Quit!"));

	QObject::connect(onlineLoginButton, &MegaIconButton::clicked, this, &StartingScreen::toOnlineLogin);
	QObject::connect(logsButton, &MegaIconButton::clicked, this, &StartingScreen::toLogs);
	QObject::connect(settingsButton, &MegaIconButton::clicked, this, &StartingScreen::toSettings);
	QObject::connect(onlineLogin, &OnlineLoginWidget::backRequired, this, &StartingScreen::hideCurrent);
	QObject::connect(settings, &SettingsScreen::backRequired, this, &StartingScreen::hideCurrent);
	QObject::connect(logsScreen, &LogsWidget::backRequired, this, &StartingScreen::hideCurrent);
	QObject::connect(onlineLogin, &OnlineLoginWidget::loginReady, this, &StartingScreen::userLoggedIn);
	QObject::connect(quitButton, &MegaIconButton::clicked, qApp, QApplication::quit);
}

void StartingScreen::toOnlineLogin()
{
	_hideAny(onlineLogin);
}


void StartingScreen::toLogs()
{
	_hideAny(logsScreen);
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
