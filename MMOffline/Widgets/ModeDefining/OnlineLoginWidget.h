#pragma once
#include "Widgets/parents/inframedWidget.h"
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include "Networking/RequestAwaiter.h"
#include "Widgets/parents/abstractNodeInterface.h"
#include "Widgets/Syncing/SyncMenuWidget.h"

class OnlineLoginWidget : public inframedWidget, abstractNode
{
	Q_OBJECT
protected:
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
	
	RequestAwaiter* awaiter;

	SyncMenuWidget* syncMenu;

	QString pendingLogin;
public:
	OnlineLoginWidget(QWidget* parent);

protected slots:
	void passwordConfirmed();
	void processResponse();
	void was_timeout();
	void hideCurrent();
signals:
	void loginReady(QString login);

};