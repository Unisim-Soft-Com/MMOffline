#pragma once
#include "Widgets/parents/inframedWidget.h"
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/qlistwidget.h>




class LogsWidget : public inframedWidget
{
protected:
	QVBoxLayout* mainLayout;
	QLabel* totalInfo;
	MegaIconButton* deleteAllButton;
	QListWidget* logView;
	MegaIconButton* backButton;

public:
	LogsWidget(QWidget* parent);

private slots:
	void deleteAllActions();

};