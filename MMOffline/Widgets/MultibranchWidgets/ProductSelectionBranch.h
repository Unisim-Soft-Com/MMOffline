#pragma once
#include "Widgets/MultibranchWidgets/GroupSelectionWidget.h"
#include "Widgets/MultibranchWidgets/ProductSelectionWidget.h"
#include "Widgets/parents/abstractNodeInterface.h"

/*
	This widget is creating subbranch which encapsulates full process of selecting 
	product with provided client. This widget was created specifically for logBranch.
	This widget is wrapping other widgets without own view. This widget is based
	on abstractNode, so it it filling on-start. This widget does not affects database
	by itself. This widget must be primed with client.
*/

class ProductSelectionBranch : public inframedWidget, abstractNode
	// encapsulates product selection process
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	// encapsulated selections
	GroupSelectionWidget* groupSelection;
	ProductSelectionWidget* productSelection;

	// client to process
	Client currentClient;
public:
	ProductSelectionBranch(QWidget* parent);

	// sets up current client and loads from database right products
	void primeSelection(Client client);
protected slots:
	// prepares product selection
	void groupReady(GroupEntity);
	// emits product and drops branch state
	void productReady(Product);
	// performs backtracking
	void hideCurrent();
signals:
	void selectionDone(Product);
};