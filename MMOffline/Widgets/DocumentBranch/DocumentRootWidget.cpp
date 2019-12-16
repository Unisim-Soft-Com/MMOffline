#include "DocumentRootWidget.h"

DocumentRootWidget::DocumentRootWidget(QWidget* parent)
	: inframedWidget(parent), abstractNode(),
	mainLayout(new QVBoxLayout(this)),
	clientSelection(new ClientSelectionWidget(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(clientSelection);

}
