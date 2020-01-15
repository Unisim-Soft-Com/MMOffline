#include "GroupSelectionWidget.h"
#include "Widgets/ExtendedDelegates/GroupDelegate.h"
#include <QtWidgets/qheaderview.h>
#include "Widgets/utils/ApplicationDataWorkset.h"
#include <QScroller>
#include "Widgets/utils/ElementsStyles.h"

GroupSelectionWidget::GroupSelectionWidget(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	info(new QLabel(this)),
	groupView(new QTableView(this)), innerModel(new GroupTreeModel(this)),
	buttonLayout(new QHBoxLayout(this)),
	backButton(new MegaIconButton(this)), okButton(new MegaIconButton(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(info);
	mainLayout->addWidget(groupView);
	mainLayout->addLayout(buttonLayout);
	buttonLayout->addWidget(backButton);
	buttonLayout->addWidget(okButton);

	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	buttonLayout->setSpacing(0);
	buttonLayout->setContentsMargins(0, 0, 0, 0);

	innerModel->setList(
		AppWorkset->dataprovider.loadEntitiesWithForcedQuery<GroupEntity>(
			ComplexFilters::TruncateGroupsWithoutProducts));
	groupView->setModel(innerModel);
	groupView->verticalHeader()->hide();
	groupView->horizontalHeader()->hide();
	groupView->setItemDelegate(new GroupDelegate(this));
	groupView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
	groupView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	groupView->setFont(makeFont(1));
	groupView->setShowGrid(false);
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setText(tr("back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	okButton->setText(tr("OK"));
	okButton->setIcon(QIcon(":/res/submit.png"));
	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);

	QScroller::grabGesture(groupView, QScroller::LeftMouseButtonGesture);
	groupView->setVerticalScrollMode(QTableView::ScrollPerPixel);

	QObject::connect(groupView, &QTableView::doubleClicked, innerModel, &GroupTreeModel::stepToNextLayer);
	QObject::connect(backButton, &MegaIconButton::clicked, innerModel, &GroupTreeModel::stepToUpperLevel);
	QObject::connect(innerModel, &GroupTreeModel::backRequired, this, &GroupSelectionWidget::backRequired);
	QObject::connect(innerModel, &GroupTreeModel::groupSelected, this, &GroupSelectionWidget::gselected);
	QObject::connect(okButton, &MegaIconButton::clicked, this, &GroupSelectionWidget::okClicked);
}

void GroupSelectionWidget::okClicked()
{
	if (groupView->currentIndex().isValid())
	{
		innerModel->stepToNextLayer(groupView->currentIndex());
	}
}

void GroupSelectionWidget::gselected(const Group& g)
{
	emit groupSelected(*g);
	innerModel->clearLayers();
}