#include "GroupSelectionWidget.h"
#include "Widgets/ExtendedDelegates/GroupDelegate.h"
#include <QtWidgets/qheaderview.h>
#include "Widgets/utils/ApplicationDataWorkset.h"



GroupSelectionWidget::GroupSelectionWidget(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	info(new QLabel(this)),
	groupView(new QTableView(this)), innerModel(new GroupTreeModel(this)),
	backButton(new MegaIconButton(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(info);
	mainLayout->addWidget(groupView);
	mainLayout->addWidget(backButton);

	innerModel->setList(AppWorkset->dataprovider.loadEntities<GroupEntity>());
	groupView->setModel(innerModel);
	groupView->verticalHeader()->hide();
	groupView->horizontalHeader()->hide();
	groupView->setItemDelegate(new GroupDelegate(this));
	groupView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
	groupView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	groupView->setFont(QFont("Times new Roman", 20, 20));
	groupView->setShowGrid(false);
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setText(QStringLiteral("back"));
	

	QObject::connect(groupView, &QTableView::clicked, innerModel, &GroupTreeModel::stepToNextLayer);
	QObject::connect(backButton, &MegaIconButton::clicked, innerModel, &GroupTreeModel::stepToUpperLevel);
	QObject::connect(innerModel, &GroupTreeModel::backRequired, this, &GroupSelectionWidget::backRequired);
	QObject::connect(innerModel, &GroupTreeModel::groupSelected, this, &GroupSelectionWidget::gselected);
}


void GroupSelectionWidget::gselected(const Group& g)
{
	emit groupSelected(*g);
	innerModel->clearLayers();
}
