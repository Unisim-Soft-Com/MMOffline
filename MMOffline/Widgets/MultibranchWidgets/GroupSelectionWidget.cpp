#include "GroupSelectionWidget.h"
#include "Widgets/ExtendedDelegates/GroupDelegate.h"
#include <QtWidgets/qheaderview.h>
#include "Dataprovider/SqliteDataProvider.h"
#include <QScroller>
#include "Widgets/utils/ElementsStyles.h"

GroupSelectionWidget::GroupSelectionWidget(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	info(new QLabel(this)),
	groupView(new QTableView(this)), innerModel(new GroupTreeModel(this)),
	buttonLayout(new QHBoxLayout(this)),
	backButton(new MegaIconButton(this)), okButton(new MegaIconButton(this))
{
	// emplacing widgets
	this->setLayout(mainLayout);
	mainLayout->addWidget(info);
	mainLayout->addWidget(groupView);
	mainLayout->addLayout(buttonLayout);
	buttonLayout->addWidget(backButton);
	buttonLayout->addWidget(okButton);

	// removing margins to avoid space loss
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	buttonLayout->setSpacing(0);
	buttonLayout->setContentsMargins(0, 0, 0, 0);

	// setting up model
	innerModel->setList(
		AppData->loadEntitiesWithForcedQuery<GroupEntity>(
			ComplexFilters::TruncateGroupsWithoutProducts));
	
	// setting up view appearance
	groupView->setModel(innerModel);
	groupView->verticalHeader()->hide();
	groupView->horizontalHeader()->hide();
	groupView->setItemDelegate(new GroupDelegate(this));
	groupView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
	groupView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	groupView->setFont(makeFont(1));
	groupView->setShowGrid(false);
	QScroller::grabGesture(groupView, QScroller::LeftMouseButtonGesture);
	groupView->setVerticalScrollMode(QTableView::ScrollPerPixel);

	
	// setting up buttons appearance
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setText(tr("back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	okButton->setText(tr("OK"));
	okButton->setIcon(QIcon(":/res/submit.png"));
	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);

	// connecting slots	
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