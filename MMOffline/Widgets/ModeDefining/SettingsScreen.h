#pragma once
#include "Widgets/parents/inframedWidget.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
#include "Widgets/utils/GlobalAppSettings.h"
#include <QtWidgets/QLabel>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QComboBox>
#include "Widgets/ElementWidgets/MegaIconButton.h"



class SettingsScreen : public inframedWidget
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	QLabel* urlBaseInfo;
	QComboBox * urlBaseField;
	QHBoxLayout* buttonsLayout;
	MegaIconButton* backButton;
	MegaIconButton* okButton;

public:
	SettingsScreen(QWidget* parent);

private slots:;
	void allConfirmed();
	void urlPicked(QString url);
};



