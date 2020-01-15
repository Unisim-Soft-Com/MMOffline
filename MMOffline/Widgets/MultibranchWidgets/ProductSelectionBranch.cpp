#include "ProductSelectionBranch.h"

void ProductSelectionBranch::groupReady(GroupEntity g)
{
	Group t(dynamic_cast<GroupEntity*>(g.clone()));
	productSelection->primeSelection(t, currentClient);
	_hideAny(productSelection);
}

ProductSelectionBranch::ProductSelectionBranch(QWidget* parent)
	: inframedWidget(parent), abstractNode(), mainLayout(new QVBoxLayout(this)), groupSelection(new GroupSelectionWidget(this)),
	productSelection(new ProductSelectionWidget(this)), currentClient(nullptr)
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(groupSelection);
	mainLayout->addWidget(productSelection);
	main = this;
	current = groupSelection;
	untouchable = groupSelection;
	productSelection->hide();
	QObject::connect(groupSelection, &GroupSelectionWidget::backRequired, this, &ProductSelectionBranch::hideCurrent);
	QObject::connect(productSelection, &ProductSelectionWidget::backRequired, this, &ProductSelectionBranch::hideCurrent);
	QObject::connect(groupSelection, &GroupSelectionWidget::groupSelected, this, &ProductSelectionBranch::groupReady);
	QObject::connect(productSelection, &ProductSelectionWidget::productObtained, this, &ProductSelectionBranch::productReady);
}

void ProductSelectionBranch::primeSelection(Client client)
{
	currentClient = client;
}

void ProductSelectionBranch::productReady(Product p)
{
	_hideCurrent(groupSelection);
	emit selectionDone(Product(dynamic_cast<ProductEntity*>(p->clone())));
}

void ProductSelectionBranch::hideCurrent()
{
	if (current == untouchable)
		emit backRequired();
	else if (current = productSelection)
		_hideCurrent(untouchable);
}