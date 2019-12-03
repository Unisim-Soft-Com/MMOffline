#include "OnlineLoginWidget.h"
#include "Widgets/utils/GlobalAppSettings.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
OnlineLoginWidget::OnlineLoginWidget(QWidget* parent)
:inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
errorLog(new QLabel(this)), loginInfo(new QLabel(this)),
loginField(new QLineEdit(this)), passwordInfo(new QLabel(this)),
passwordField(new QLineEdit(this)), buttonLayout(new QHBoxLayout(this)),
okButton(new MegaIconButton(this)), backButton(new MegaIconButton(this)),
awaiter(new RequestAwaiter(GlobalAppSettings::instance()->timeoutint, this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(errorLog);
	mainLayout->addStretch(0);
	mainLayout->addWidget(loginInfo);
	mainLayout->addWidget(loginField);
	mainLayout->addWidget(passwordInfo);
	mainLayout->addWidget(passwordField);
	mainLayout->addLayout(buttonLayout);
	mainLayout->addStretch(0);
	buttonLayout->addWidget(backButton);
	buttonLayout->addWidget(okButton);

	errorLog->setStyleSheet(ERROR_TEXT_STYLESHEET);
	loginInfo->setAlignment(Qt::AlignCenter);
	loginInfo->setText(tr("write your login:"));
	loginField->setText(GlobalAppSettings::instance()->localLogin);
	passwordInfo->setText(tr("write your password:"));
	passwordInfo->setAlignment(Qt::AlignCenter);
	passwordField->setEchoMode(QLineEdit::Password);

	QObject::connect(loginField, &QLineEdit::editingFinished, passwordField, QOverload<>::of(&QLineEdit::setFocus));
	QObject::connect(passwordField, &QLineEdit::editingFinished, this, &OnlineLoginWidget::passwordConfirmed);
	QObject::connect(okButton, &MegaIconButton::clicked, this, &OnlineLoginWidget::passwordConfirmed);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &OnlineLoginWidget::backRequired);
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &OnlineLoginWidget::processResponse);
	QObject::connect(awaiter, &RequestAwaiter::requestTimeout, this, &OnlineLoginWidget::was_timeout);

}

void OnlineLoginWidget::processResponse()
{

}

void OnlineLoginWidget::was_timeout()
{
}

void OnlineLoginWidget::passwordConfirmed()
{
	if (loginField->text().isEmpty() || passwordField->text().isEmpty())
	{
		errorLog->setText(tr("Please enter your login and password!"));
		return;
	}
	globalWorkset.networkingEngine->initiateSession(loginField->text(), passwordField->text(), awaiter);
}
