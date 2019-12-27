#include "ClientSelectionWidget.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Widgets/utils/ApplicationDataWorkset.h"


ClientSelectionWidget::ClientSelectionWidget(QWidget* parent)
	: inframedWidget(parent), abstractNode(), mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	userInfo(new QLabel(innerWidget)), searchPanel(new QHBoxLayout(innerWidget)),
	searchLine(new QLineEdit(innerWidget)),
	clientView(new QListView(innerWidget)), backButton(new MegaIconButton(innerWidget)),
	innerModel(new DataCountingDataModel(this)),
	innerDelegate(new ClientsDelegate(this)),
	searchProxy(new DataEntityFilterModel(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	innerLayout->addWidget(userInfo);
	innerLayout->addLayout(searchPanel);
	innerLayout->addWidget(clientView);
	innerLayout->addWidget(backButton);
	searchPanel->addWidget(searchLine);

	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);
	innerLayout->setContentsMargins(0, 0, 0, 0);


	userInfo->setText(tr("Select client"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setText(tr("back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	current = innerWidget;
	untouchable = innerWidget;
	main = this;

	innerModel->setData(AppWorkset->dataprovider.loadDataAs<ClientEntity>());
	searchProxy->setSourceModel(innerModel);
	clientView->setModel(searchProxy);
	clientView->setItemDelegate(innerDelegate);
	innerModel->assignQuantityInfo(std::move(AppWorkset->dataprovider.loadIdPairs(ComplexFilters::ClientQuantityLinking)));
	QObject::connect(searchLine, &QLineEdit::textChanged, searchProxy, &QSortFilterProxyModel::setFilterFixedString);
	QObject::connect(searchProxy, &DataEntityFilterModel::dataEntityClicked, this, &ClientSelectionWidget::clientClicked);
	QObject::connect(clientView, &QListView::clicked, searchProxy, &DataEntityFilterModel::mapClickToEntity);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &ClientSelectionWidget::backRequired);
}

void ClientSelectionWidget::incrementDocCounter(int id, int q)
{
	innerModel->assignQuantityUpdate(id, q);
}

void ClientSelectionWidget::clientClicked(DataEntity client)
{
	Client temp = std::static_pointer_cast<ClientEntity>(client);
	emit clientSelected(*temp);
}

