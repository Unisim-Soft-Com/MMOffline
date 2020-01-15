#pragma once
#include "Widgets/parents/inframedWidget.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
#include "Widgets/utils/GlobalAppSettings.h"
#include <QtWidgets/QLabel>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QComboBox>
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include <QtWidgets/QLineEdit>
#include "Widgets/ModeDefining/AssertionLabel.h"

class SettingsScreen : public inframedWidget
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	QLabel* urlBaseInfo;
	QComboBox* urlBaseField;
	QHBoxLayout* buttonsLayout;
	MegaIconButton* backButton;
	MegaIconButton* okButton;
	QLineEdit* filepathField;
	MegaIconButton* makeLoadFromFile;
	MegaIconButton* translateButton;
	AssertionLabel* errorLog;

	QIcon langIcons[3];
	Translations::Languages currentLanguage;

	void fillTexts();
public:
	SettingsScreen(QWidget* parent);

private slots:
	void allConfirmed();
	void loadFromFile();
	void urlPicked(QString url);
	void translateAll();
signals:
	void translating(int lang);
};
