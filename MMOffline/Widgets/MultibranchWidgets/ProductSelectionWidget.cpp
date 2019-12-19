#include "ProductSelectionWidget.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
#include "Widgets/utils/ElementsStyles.h"
ProductList plist
{
	Product(new ProductEntity(001, "Test product 1 ", "testpr", 14.88, 32.20)),
	Product(new ProductEntity(002, "Test product 2 ", "testpr", 14.89, 32.20)),
	Product(new ProductEntity(003, "Test product 3 ", "testpr", 14.90, 32.20)),
	Product(new ProductEntity(004, "Test product 4 ", "testpr", 14.91, 32.20)),
	Product(new ProductEntity(005, "Test product 5 ", "testpr", 14.92, 32.20)),
	Product(new ProductEntity(006, "Test product 6 ", "testpr", 14.93, 32.20)),
	Product(new ProductEntity(007, "Test product 7 ", "testpr", 14.94, 32.20)),
	Product(new ProductEntity(8, "Test product 8 ", "testpr", 14.95, 32.20)),
	Product(new ProductEntity(9, "Test product 9 ", "testpr", 14.96, 32.20)),
	Product(new ProductEntity(010, "Test product 10 ", "testpr", 14.97, 32.20)),
	Product(new ProductEntity(011, "Test product 11 ", "testpr", 14.98, 32.20)),
	Product(new ProductEntity(012, "Test product 12 ", "testpr", 14.99, 32.20)),
	Product(new ProductEntity(013, "Test product 13 ", "testpr", 14.81, 32.20)),
	Product(new ProductEntity(014, "Test product 14 ", "testpr", 14.82, 32.20)),
	Product(new ProductEntity(015, "Test product 15 ", "testpr", 14.83, 32.20)),
	Product(new ProductEntity(016, "Test product 16 ", "testpr", 14.84, 32.20)),
};





ProductSelectionWidget::ProductSelectionWidget(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)), info(new QLabel(this)),
	searchLine(new QLineEdit(this)),
	productView(new QListView(this)), dataModel(new DataEntityListModel(this)),
	searchProxy(new DataEntityFilterModel(this)), innerDelegate(new ProductsDelegate(this)),
	backButton(new MegaIconButton(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(info);
	mainLayout->addWidget(searchLine);
	mainLayout->addWidget(productView);
	mainLayout->addWidget(backButton);

	dataModel->setData(AppWorkset->dataprovider.loadDataAs<ProductEntity>());
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
	QObject::connect(backButton, &MegaIconButton::clicked, this, &ProductSelectionWidget::backUsed);
	QObject::connect(searchLine, &QLineEdit::textChanged, searchProxy, &DataEntityFilterModel::setFilterFixedString);
}

void ProductSelectionWidget::setDataLoadParameters(Group group, Client client)
{
	
}

void ProductSelectionWidget::backUsed()
{
	info->setText("BACK1111!!");
}

void ProductSelectionWidget::productSelected(DataEntity product)
{
	info->setText("Product selected!: " + std::static_pointer_cast<ProductEntity>(product)->name);
}
