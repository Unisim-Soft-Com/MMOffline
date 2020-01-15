#pragma once
#include "Widgets/ModeDefining/OnlineLoginWidget.h"
#include "Widgets/ModeDefining/SettingsScreen.h"

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

	void fillTexts();
protected slots:
	void toOnlineLogin();
	void toSettings();
	void hideCurrent();
	void userLoggedIn(QString login);
	void translationHappened(int);
public slots:
	void setInfoLabel();
signals:
	void documentCreationInitiated();
	void logsRequired();
};