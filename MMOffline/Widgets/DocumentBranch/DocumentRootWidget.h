#pragma once
#include <QtWidgets/QVBoxLayout>
#include "Widgets/parents/inframedWidget.h"
#include "Widgets/parents/abstractNodeInterface.h"
#include "Widgets/DocumentBranch/ClientSelectionWidget.h"



class DocumentRootWidget : public inframedWidget, abstractNode
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	ClientSelectionWidget* clientSelection;



public:
	DocumentRootWidget(QWidget* parent = nullptr);
};