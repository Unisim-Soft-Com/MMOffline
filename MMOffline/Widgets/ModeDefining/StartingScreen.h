#pragma once
#include "Widgets/ModeDefining/OnlineLoginWidget.h"
#include "Widgets/ModeDefining/SettingsScreen.h"

/*
	This widget is used to allow user to pick branch. This is default inner view
	of root widget. It handles little branches by itself, without using memory 
	reallocations. This widget is not dynamic, i.e. it's better to avoid adding more
	subbranches here. This widgets does not affects anything in database or internet. 
*/


class StartingScreen : public inframedWidget, abstractNode
	// Allows user to pick subbranch
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

	// translates texts again
	void fillTexts();
protected slots:
	// goes to online login subbranch
	void toOnlineLogin();
	// goes to settings subbranch
	void toSettings();
	// attempts to hide current subbranch or quits from the app
	void hideCurrent();
	// changes text of infoLabel to correspond to new data
	void userLoggedIn(QString login);
	// retranslates this widget and login subbranch
	void translationHappened(int);
public slots:
	// refreshes info label
	void setInfoLabel();
signals:
	// emitted when user picks new document branch
	void documentCreationInitiated();
	// emitted when user picks logs branch
	void logsRequired();
};