#pragma once
#include "Dataprovider/DataEntities.h"
#include "Widgets/parents/inframedWidget.h"
#include <QtWidgets/QListView>
#include <QtWidgets/QBoxlayout>
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include <QLabel>
#include "Widgets/ExtendedDelegates/ProductsDelegate.h"
#include <QLineEdit>

class ProductSelectionWidget : public inframedWidget
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
	void setDataLoadParameters(Group group, Client client);
	void primeSelection(Group group, Client client);
	void incrementQuantityCounter(int id, int q = 1);
	void setQuantityCounter(int id, int q = 0);
protected slots:
	void productSelected(DataEntity product);
	void okClicked();
signals:
	void productObtained(const Product);
};