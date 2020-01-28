#pragma once
#include "Widgets/parents/inframedWidget.h"
#include "Dataprovider/DataEntities.h"
#include <QtWidgets/qtableview.h>
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include <QBoxLayout>
#include <QLabel>


/*
	This widget is allowing user to select group of products. It drops
	it's state after selection the lowerest item in group tree.
	It is using custom select query to avoid showing empty groups. 
	It is branch independent.

	Affected tables:
	V:	Groups, Products, Clients
*/


class GroupSelectionWidget : public inframedWidget
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	QLabel* info;
	QTableView* groupView;
	GroupTreeModel* innerModel;
	QHBoxLayout* buttonLayout;
	MegaIconButton* backButton;
	MegaIconButton* okButton;
public:
	GroupSelectionWidget(QWidget* parent);
public slots:
	// emits signal and clears state of selection
	void gselected(const Group& g);
	// wrapper for ok button, same as double click on item
	void okClicked();
signals:
	void groupSelected(GroupEntity);
};