#pragma once
#include "Widgets/parents/inframedWidget.h"
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include "Networking/RequestAwaiter.h"



class OnlineLoginWidget : public inframedWidget
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	QLabel* errorLog;
	QLabel* loginInfo;
	QLineEdit* loginField;
	QLabel* passwordInfo;
	QLineEdit* passwordField;
	QHBoxLayout* buttonLayout;
	MegaIconButton* okButton;
	MegaIconButton* backButton;
	
	RequestAwaiter* awaiter;
public:
	OnlineLoginWidget(QWidget* parent);

protected slots:
	void passwordConfirmed();
	void processResponse();
	void was_timeout();
signals:
	void loginReady();

};