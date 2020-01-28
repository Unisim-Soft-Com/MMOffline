#pragma once
#include "Widgets/parents/inframedWidget.h"
#include "Dataprovider/SqliteDataProvider.h"
#include "Widgets/utils/GlobalAppSettings.h"
#include <QtWidgets/QLabel>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QComboBox>
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include <QtWidgets/QLineEdit>
#include <QtWidgets/qspinbox.h>
#include "Widgets/ModeDefining/AssertionLabel.h"

/*
	This widget represents screen for setting up system variables. It 
	also allows user to load database from file. Here are displayed version 
	and it's prefix. Here is possible to switch debugging mode which echoes 
	all queries into log file. 

	Affected tables: 
	EV			All outer
*/


class SettingsScreen : public inframedWidget
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	QLabel* appInfo;
	QLabel* urlBaseInfo;
	QComboBox* urlBaseField;
	QHBoxLayout* buttonsLayout;
	MegaIconButton* backButton;
	MegaIconButton* okButton;
	QLineEdit* filepathField;
	MegaIconButton* makeLoadFromFile;
	MegaIconButton* translateButton;
	QLabel* timeoutInfo;
	QSpinBox* timeoutField;
	MegaIconButton* debugMode;
	AssertionLabel* errorLog;

	// cached language icons
	QIcon langIcons[3];
	// currently selected language
	Translations::Languages currentLanguage;

	// translates all text on this page
	void fillTexts();
public:
	SettingsScreen(QWidget* parent);

private slots:
	// saves everything
	void allConfirmed();
	// attempts to create CSVParser and load database from provided path
	void loadFromFile();
	// saves url picked in combobox 
	void urlPicked(QString url);
	// changes language button state, translates application and emits retranslation signal
	void translateAll();
signals:
	// notificates outer branches that retranslation required
	void translating(int lang);
};
