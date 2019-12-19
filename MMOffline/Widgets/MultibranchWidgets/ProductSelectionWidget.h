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
	QLabel *info;
	QLineEdit* searchLine;
	QListView* productView;
	DataEntityListModel* dataModel;
	DataEntityFilterModel* searchProxy;
	ProductsDelegate* innerDelegate;
	MegaIconButton* backButton;
public:
	ProductSelectionWidget(QWidget* parent);
	void setDataLoadParameters(Group group, Client client);

protected slots:;
	void backUsed();
	void productSelected(DataEntity product);
signals:
	void productObtained(const Product);
};