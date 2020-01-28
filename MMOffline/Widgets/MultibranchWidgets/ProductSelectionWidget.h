#pragma once
#include "Dataprovider/DataEntities.h"
#include "Widgets/parents/inframedWidget.h"
#include <QtWidgets/QListView>
#include <QtWidgets/QBoxlayout>
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include <QLabel>
#include "Widgets/ExtendedDelegates/ProductsDelegate.h"
#include <QLineEdit>

/*
	This widget allows user to pick product from database. This widget must be primed.
	This widget can perform counting of affected products with corresponding data.
	
	Affected tables:
	V	Products
*/


class ProductSelectionWidget : public inframedWidget
	// allows to pick product
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	QLabel* info;
	QHBoxLayout* searchPanel;
	QLabel* searchInfo;
	QLineEdit* searchLine;
	QListView* productView;
	DataCountingDataModel* dataModel;
	DataEntityFilterModel* searchProxy;
	ProductsDelegate* innerDelegate;
	QHBoxLayout* buttonPanel;
	MegaIconButton* backButton;
	MegaIconButton* okButton;
public:
	ProductSelectionWidget(QWidget* parent);

	// loads fresh data based on provided variables
	void primeSelection(Group group, Client client);
	// increments counter of the product with provided id
	void incrementQuantityCounter(IdInt id, int q = 1);
	// rewrites counter of the product with provided id
	void setQuantityCounter(IdInt id, int q = 0);
protected slots:
	// converts abstract entity to product, then emits it
	void productSelected(DataEntity product);
	// wrapper for okButton
	void okClicked();
signals:
	void productObtained(const Product);
};