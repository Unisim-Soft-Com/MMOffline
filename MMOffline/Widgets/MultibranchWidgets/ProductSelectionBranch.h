#pragma once
#include "Widgets/MultibranchWidgets/GroupSelectionWidget.h"
#include "Widgets/MultibranchWidgets/ProductSelectionWidget.h"
#include "Widgets/parents/abstractNodeInterface.h"

class ProductSelectionBranch : public inframedWidget, abstractNode
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	GroupSelectionWidget* groupSelection;
	ProductSelectionWidget* productSelection;

	Client currentClient;
public:
	ProductSelectionBranch(QWidget* parent);

	void primeSelection(Client client);
protected slots:
	void groupReady(GroupEntity);
	void productReady(Product);
	void hideCurrent();
signals:
	void selectionDone(Product);
};