#pragma once
#include "Widgets/parents/inframedWidget.h"
#include <QtWidgets/QBoxLayout>
#include "OnlineLoginWidget.h"
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include "Widgets/parents/abstractNodeInterface.h"
#include "Widgets/utils/GlobalAppSettings.h"
#include <QtWidgets/QLabel>
#include "Widgets/ModeDefining/OnlineLoginWidget.h"
#include "Widgets/ModeDefining/SettingsScreen.h"
#include "Widgets/LogBranch/LogsWidget.h"


class StartingScreen : public inframedWidget, abstractNode
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QLabel* infoLabel;
	MegaIconButton* onlineLoginButton;
	MegaIconButton* createDocumentButton;
	MegaIconButton* logsButton;
	QHBoxLayout* footerLayout;
	MegaIconButton* settingsButton;
	MegaIconButton* quitButton;

	OnlineLoginWidget* onlineLogin;
	SettingsScreen* settings;
public:
	StartingScreen(QWidget* parent);

protected slots:;
	void toOnlineLogin();
	void toSettings();
	void hideCurrent();
	void userLoggedIn(QString login);
public slots:
	void setInfoLabel();
signals:
	void documentCreationInitiated();
	void logsRequired();
};