#pragma once
#include "Networking/RequestAwaiter.h"
#include "Widgets/parents/abstractNodeInterface.h"
#include "Widgets/Syncing/SyncMenuWidget.h"
#include <QtWidgets/QLineEdit>

/*
	This branch is concentrated over network actions. It has synchronisation subbranch and 
	connection to database. Warning - synchronization causes database drops.
	This widget itself is not affecting database. This widget inherits from abstract
	node interfaces for branching. This widget has his own awaiter. This widget hides
	after first succesfull login, but be aware that server closes session in 10 mins, so 
	sync will fail after 10 mins without new login. Possibly it must be fixed. 
*/
class OnlineLoginWidget : public inframedWidget, abstractNode
{
	Q_OBJECT
protected:
	// elements
	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QLabel* errorLog;
	QLabel* loginInfo;
	QLineEdit* loginField;
	QLabel* passwordInfo;
	QLineEdit* passwordField;
	QHBoxLayout* buttonLayout;
	MegaIconButton* okButton;
	MegaIconButton* backButton;

	//network tools
	RequestAwaiter* awaiter;
	QString pendingLogin;

	// synchronization subbranch
	SyncMenuWidget* syncMenu;

public:
	OnlineLoginWidget(QWidget* parent);

	// sets focus on right field
	virtual void show() override;
	// reloads text with actual translation
	void fillTexts();
protected slots:
	// activated on password confirmation, asserts login and password and sends login request
	void passwordConfirmed();
	// asserts response, if there is session id and user id - continues to synchroniztion
	void processResponse();
	// activated on request timeouts
	void was_timeout();
	// performs back
	void hideCurrent();
signals:
	void loginReady(QString);
};