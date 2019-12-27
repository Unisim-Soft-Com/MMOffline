#include "ProductSelectionWidget.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
#include "Widgets/utils/ElementsStyles.h"

const QString productFilter = "groupId == %1 and clientIds like \"%%2%\"";

ProductSelectionWidget::ProductSelectionWidget(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)), info(new QLabel(this)),
	searchLine(new QLineEdit(this)),
	productView(new QListView(this)), dataModel(new DataCountingDataModel(this)),
	searchProxy(new DataEntityFilterModel(this)), innerDelegate(new ProductsDelegate(this)),
	backButton(new MegaIconButton(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(info);
	mainLayout->addWidget(searchLine);
	mainLayout->addWidget(productView);
	mainLayout->addWidget(backButton);

	searchProxy->setSourceModel(dataModel);
	searchProxy->setFilterRole(Qt::UserRole);
	searchProxy->setFilterFixedString("");
	productView->setModel(searchProxy);
	productView->setFont(QFont("Times new Roman", 20, 20));
	productView->setItemDelegate(innerDelegate);
	backButton->setIcon(QIcon(":/res/back.png")); backButton->setText(tr("back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	QObject::connect(productView, &QListView::clicked, dataModel, &DataEntityListModel::mapClickToEntity);
	QObject::connect(dataModel, &DataEntityListModel::dataEntityClicked, this, &ProductSelectionWidget::productSelected);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &ProductSelectionWidget::backRequired);
	QObject::connect(searchLine, &QLineEdit::textChanged, searchProxy, &DataEntityFilterModel::setFilterFixedString);
}

void ProductSelectionWidget::setDataLoadParameters(Group group, Client client)
{
	
}

void ProductSelectionWidget::primeSelection(Group group, Client client)
{
	dataModel->setData(AppWorkset->dataprovider.loadDataWithFieldFilter<ProductEntity>(
		productFilter.arg(group->id).arg(client->id)
		));
	dataModel->assignQuantityInfo(std::move(AppWorkset->dataprovider.loadIdPairs(
		ComplexFilters::ProductQuantityLinking.arg(client->id).arg(group->id))));
	searchLine->clear();
	searchProxy->setSourceModel(dataModel);
	productView->update();
	searchProxy->setFilterRole(DataEntityListModel::ExtendedRoles::SearchRole);
	searchProxy->setFilterFixedString("");

}

void ProductSelectionWidget::incrementQuantityCounter(int id, int q)
{
	dataModel->assignQuantityUpdate(id, q);
}


void ProductSelectionWidget::productSelected(DataEntity product)
{
	Product temp = std::dynamic_pointer_cast<ProductEntity>(product);
	if (temp != nullptr)
		emit productObtained(temp);
}
