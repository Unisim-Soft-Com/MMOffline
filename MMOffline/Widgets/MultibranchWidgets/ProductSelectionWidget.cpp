#include "ProductSelectionWidget.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
#include "Widgets/utils/ElementsStyles.h"
#include <QScroller>
const QString productFilter = "groupId = %1 and clientIds like \"%%2%\"";

ProductSelectionWidget::ProductSelectionWidget(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)), info(new QLabel(this)),
	searchPanel(new QHBoxLayout(this)), searchInfo(new QLabel(this)), searchLine(new QLineEdit(this)),
	productView(new QListView(this)), dataModel(new DataCountingDataModel(this)),
	searchProxy(new DataEntityFilterModel(this)), innerDelegate(new ProductsDelegate(this)),
	buttonPanel(new QHBoxLayout(this)),
	backButton(new MegaIconButton(this)), okButton(new MegaIconButton(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(info);
	mainLayout->addLayout(searchPanel);
	searchPanel->addWidget(searchInfo);
	searchPanel->addWidget(searchLine);
	mainLayout->addWidget(productView);
	mainLayout->addLayout(buttonPanel);
	buttonPanel->addWidget(backButton);
	buttonPanel->addWidget(okButton);

	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	buttonPanel->setSpacing(0);
	buttonPanel->setContentsMargins(0, 0, 0, 0);

	searchInfo->setText(tr("Search: "));
	searchInfo->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

	searchProxy->setSourceModel(dataModel);
	searchProxy->setFilterRole(Qt::UserRole);
	searchProxy->setFilterFixedString("");
	productView->setModel(searchProxy);
	productView->setFont(QFont("Times new Roman", 20, 20));
	productView->setItemDelegate(innerDelegate);
	backButton->setIcon(QIcon(":/res/back.png")); backButton->setText(tr("back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	okButton->setIcon(QIcon(":/res/submit.png"));
	okButton->setText(tr("OK"));
	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);

	QScroller::grabGesture(productView, QScroller::LeftMouseButtonGesture);
	productView->setVerticalScrollMode(QListView::ScrollPerPixel);
	QObject::connect(productView, &QListView::doubleClicked, searchProxy, &DataEntityFilterModel::mapClickToEntity);
	QObject::connect(searchProxy, &DataEntityFilterModel::dataEntityClicked, this, &ProductSelectionWidget::productSelected);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &ProductSelectionWidget::backRequired);
	QObject::connect(okButton, &MegaIconButton::clicked, this, &ProductSelectionWidget::okClicked);
	QObject::connect(searchLine, &QLineEdit::textChanged, searchProxy, &DataEntityFilterModel::setFilterFixedString);
#ifdef Q_OS_ANDROID
	searchLine->setInputMethodHints(Qt::InputMethodHint::ImhNoPredictiveText);
#endif
}

void ProductSelectionWidget::setDataLoadParameters(Group group, Client client)
{
}

void ProductSelectionWidget::primeSelection(Group group, Client client)
{
	dataModel->setData(AppWorkset->dataprovider.loadDataAs<ProductEntity>(
		productFilter.arg(group->id).arg(client->id)
		));
	dataModel->assingEmptyCounters();
	searchLine->clear();
	searchProxy->setSourceModel(dataModel);
	productView->update();
	searchProxy->setFilterRole(DataEntityListModel::ExtendedRoles::SearchRole);
	searchProxy->setFilterFixedString("");
}

void ProductSelectionWidget::incrementQuantityCounter(int id, int q)
{
	dataModel->incrementQuantity(id, q);
}

void ProductSelectionWidget::setQuantityCounter(int id, int q)
{
	dataModel->assignQuantityUpdate(id, q);
}

void ProductSelectionWidget::okClicked()
{
	if (productView->currentIndex().isValid())
	{
		searchProxy->mapClickToEntity(productView->currentIndex());
	}
}

void ProductSelectionWidget::productSelected(DataEntity product)
{
	Product temp = std::dynamic_pointer_cast<ProductEntity>(product);
	if (temp != nullptr)
		emit productObtained(temp);
}