#pragma once
#include "Widgets/parents/inframedWidget.h"
#include "Dataprovider/DataEntities.h"
#include <QtWidgets/QListView>
#include <QtWidgets/qtableview.h>
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include <QBoxLayout>
#include <QLabel>


class GroupSelectionWidget : public inframedWidget
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	QLabel* info;
	QTableView* groupView;
	GroupTreeModel* innerModel;
	MegaIconButton* backButton;

public:
	GroupSelectionWidget(QWidget* parent);
public slots:
	void gselected(const Group& g);
signals:
	void groupSelected(GroupEntity);
};