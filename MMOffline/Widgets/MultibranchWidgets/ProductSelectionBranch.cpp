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
	// emplacing subwidgets
	this->setLayout(mainLayout);
	mainLayout->addWidget(groupSelection);
	mainLayout->addWidget(productSelection);
	
	// setting up abstract node interfaces
	main = this;
	current = groupSelection;
	untouchable = groupSelection;
	productSelection->hide();

	// connecting slots
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
	if (!_hideCurrent(untouchable))
		emit backRequired();
}