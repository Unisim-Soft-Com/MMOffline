#include "OnlineLoginWidget.h"
#include "Widgets/utils/GlobalAppSettings.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Networking/dataupdateengine-http.h"
#include "Networking/Parsers/RequestParser.h"

OnlineLoginWidget::OnlineLoginWidget(QWidget* parent)
	:inframedWidget(parent), abstractNode(), mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	errorLog(new QLabel(innerWidget)), loginInfo(new QLabel(innerWidget)),
	loginField(new QLineEdit(innerWidget)), passwordInfo(new QLabel(innerWidget)),
	passwordField(new QLineEdit(innerWidget)), buttonLayout(new QHBoxLayout(innerWidget)),
	okButton(new MegaIconButton(innerWidget)), backButton(new MegaIconButton(innerWidget)),
	awaiter(new RequestAwaiter(AppSettings->timeoutint, this)), syncMenu(new SyncMenuWidget(this))
{
	// Allocating widgets
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

	// removing margins to avoid space loss
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	// setting up abstract node interfaces
	main = this;
	current = innerWidget;
	untouchable = innerWidget;
	// setting up subbranch
	mainLayout->addWidget(syncMenu);
	syncMenu->hide();

	// setting up labels and buttons appearance
	errorLog->setStyleSheet(ERROR_TEXT_STYLESHEET);
	loginInfo->setAlignment(Qt::AlignCenter);
	passwordInfo->setAlignment(Qt::AlignCenter);
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	backButton->setIcon(QIcon(":/res/back.png"));
	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	okButton->setIcon(QIcon(":/res/submit.png"));
	fillTexts();

	// setting up login&password values
	loginField->setText(GlobalAppSettings::instance()->localLogin);
	passwordField->setEchoMode(QLineEdit::Password);

	// connecting slots
	QObject::connect(loginField, &QLineEdit::returnPressed, passwordField, QOverload<>::of(&QLineEdit::setFocus));
	QObject::connect(passwordField, &QLineEdit::returnPressed, this, &OnlineLoginWidget::passwordConfirmed);
	QObject::connect(okButton, &MegaIconButton::clicked, this, &OnlineLoginWidget::passwordConfirmed);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &OnlineLoginWidget::backRequired);
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &OnlineLoginWidget::processResponse);
	QObject::connect(awaiter, &RequestAwaiter::requestTimeout, this, &OnlineLoginWidget::was_timeout);
	// because of this login window is not showing again
	QObject::connect(syncMenu, &SyncMenuWidget::backRequired, this, &OnlineLoginWidget::hideCurrent);
}

void OnlineLoginWidget::show()
{
	if (loginField->text().isEmpty())
	{
		loginField->setFocus();
	}
	else
	{
		passwordField->setFocus();
	}
	inframedWidget::show();
}

void OnlineLoginWidget::fillTexts()
{
	loginInfo->setText(tr("write your login:"));
	okButton->setText(tr("Ok!"));
	backButton->setText(tr("back"));
	passwordInfo->setText(tr("write your password:"));
	syncMenu->fillTexts();
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
		AppNetwork->setSession(resp.session, resp.uid);
		syncMenu->setLogin(pendingLogin, AppSettings->localLogin);
		AppSettings->localLogin = pendingLogin;
		AppSettings->dump();
		emit loginReady(AppSettings->localLogin);
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
	emit backRequired();
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
	AppNetwork->initiateSession(loginField->text(), passwordField->text(), awaiter);
}