#include "GroupSelectionWidget.h"
#include "Widgets/ExtendedDelegates/GroupDelegate.h"
#include <QtWidgets/qheaderview.h>
#include "Widgets/utils/ApplicationDataWorkset.h"
GroupList glist
{
	Group(new GroupEntity("001", 100, 0)),
	Group(new GroupEntity("012", 012, 0)),
	Group(new GroupEntity("013", 013, 0)),
	Group(new GroupEntity("004", 400, 100)),
Group(new GroupEntity("005", 500, 400)),
Group(new GroupEntity("006", 600, 400)),
Group(new GroupEntity("007", 700, 500)),
Group(new GroupEntity("008", 800, 500)),
Group(new GroupEntity("010", 010, 600)),
Group(new GroupEntity("011", 011, 600)),


};


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
	QObject::connect(innerModel, &GroupTreeModel::backRequired, this, &GroupSelectionWidget::backUsed);
	QObject::connect(innerModel, &GroupTreeModel::groupSelected, this, &GroupSelectionWidget::gselected);
}

void GroupSelectionWidget::backUsed()
{
	info->setText("BACK!!!111");
}

void GroupSelectionWidget::gselected(const Group& g)
{
	info->setText("GroupSelected: " + g->name);
}
