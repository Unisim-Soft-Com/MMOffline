#pragma once
#include "Widgets/parents/inframedWidget.h"
#include <QtWidgets/QBoxLayout>
#include "OnlineLoginWidget.h"
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include "Widgets/parents/abstractNodeInterface.h"
#include "Widgets/utils/GlobalAppSettings.h"
#include <QtWidgets/QLabel>

class OnlineLoginWidget;
class SettingsScreen;
class LogsWidget;

class StartingScreen : public inframedWidget, abstractNode
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QLabel* infoLabel;
	MegaIconButton* onlineLoginButton;
	MegaIconButton* logsButton;
	QHBoxLayout* footerLayout;
	MegaIconButton* settingsButton;
	MegaIconButton* quitButton;

	OnlineLoginWidget* onlineLogin;
	SettingsScreen* settings;
	LogsWidget* logsScreen;
public:
	StartingScreen(GlobalAppSettings& go, QWidget* parent);

protected slots:;
	void toOnlineLogin();
	void toOfflineLogin();
	void toLogs();
	void toSettings();

};