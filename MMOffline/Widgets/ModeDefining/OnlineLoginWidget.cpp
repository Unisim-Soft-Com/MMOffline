#include "OnlineLoginWidget.h"
#include "Widgets/utils/GlobalAppSettings.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
#include "Networking/Parsers/RequestParser.h"


OnlineLoginWidget::OnlineLoginWidget(QWidget* parent)
:inframedWidget(parent), abstractNode(),  mainLayout(new QVBoxLayout(this)),
innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
errorLog(new QLabel(innerWidget)), loginInfo(new QLabel(innerWidget)),
loginField(new QLineEdit(innerWidget)), passwordInfo(new QLabel(innerWidget)),
passwordField(new QLineEdit(innerWidget)), buttonLayout(new QHBoxLayout(innerWidget)),
okButton(new MegaIconButton(innerWidget)), backButton(new MegaIconButton(innerWidget)),
awaiter(new RequestAwaiter(AppSettings->timeoutint, this)), syncMenu(new SyncMenuWidget(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	innerLayout->addWidget(errorLog);
	innerLayout->addStretch(0);
	innerLayout->addWidget(loginInfo);
	innerLayout->addWidget(loginField);
	innerLayout->addWidget(passwordInfo);
	innerLayout->addWidget(passwordField);
	innerLayout->addStretch(0);
	innerLayout->addLayout(buttonLayout);
	buttonLayout->addWidget(backButton);
	buttonLayout->addWidget(okButton);


	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	main = this;
	current = innerWidget;
	untouchable = innerWidget;
	mainLayout->addWidget(syncMenu);
	syncMenu->hide();

	errorLog->setStyleSheet(ERROR_TEXT_STYLESHEET);
	loginInfo->setAlignment(Qt::AlignCenter);
	loginInfo->setText(tr("write your login:"));
	loginField->setText(GlobalAppSettings::instance()->localLogin);
	passwordInfo->setText(tr("write your password:"));
	passwordInfo->setAlignment(Qt::AlignCenter);
	passwordField->setEchoMode(QLineEdit::Password);
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	backButton->setText(tr("back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	okButton->setText(tr("Ok!"));
	okButton->setIcon(QIcon(":/res/submit.png"));

	QObject::connect(loginField, &QLineEdit::returnPressed, passwordField, QOverload<>::of(&QLineEdit::setFocus));
	QObject::connect(passwordField, &QLineEdit::returnPressed, this, &OnlineLoginWidget::passwordConfirmed);
	QObject::connect(okButton, &MegaIconButton::clicked, this, &OnlineLoginWidget::passwordConfirmed);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &OnlineLoginWidget::backRequired);
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &OnlineLoginWidget::processResponse);
	QObject::connect(awaiter, &RequestAwaiter::requestTimeout, this, &OnlineLoginWidget::was_timeout);
	QObject::connect(syncMenu, &SyncMenuWidget::backRequired, this, &OnlineLoginWidget::backRequired);
}

void OnlineLoginWidget::processResponse()
{
	auto resp = RequestParser::getLoginResult(awaiter->restext, awaiter->errtext);
	if (resp.isError)
	{
		errorLog->setText(resp.error);
	}
	else
	{
		DataWorkset::instance()->networkingEngine->setSession(resp.session, resp.uid);
		syncMenu->setLogin(pendingLogin);
		_hideAny(syncMenu);
		pendingLogin.clear();
	}
}

void OnlineLoginWidget::was_timeout()
{
	errorLog->setText(tr("was timeout with delay: ") + QString::number(awaiter->getInterval()));
}

void OnlineLoginWidget::hideCurrent()
{
	_hideCurrent(innerWidget);
}

void OnlineLoginWidget::passwordConfirmed()
{
	if (loginField->text().isEmpty() || passwordField->text().isEmpty())
	{
		errorLog->setText(tr("Please enter your login and password!"));
		return;
	}
	if (awaiter->isAwaiting()) return;
	pendingLogin = loginField->text();
	DataWorkset::instance()->networkingEngine->initiateSession(loginField->text(), passwordField->text(), awaiter);
}
