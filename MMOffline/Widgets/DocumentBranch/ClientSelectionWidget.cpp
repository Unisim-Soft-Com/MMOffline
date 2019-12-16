#include "ClientSelectionWidget.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
ClientSelectionWidget::ClientSelectionWidget(QWidget* parent)
	: inframedWidget(parent), abstractNode(), mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	userInfo(new QLabel(innerWidget)), searchPanel(new QHBoxLayout(innerWidget)),
	searchLine(new QLineEdit(innerWidget)), doSearchButton(new MegaIconButton(innerWidget)),
	clientView(new QListView(innerWidget)), backButton(new MegaIconButton(innerWidget)),
	innerModel(new ClientDataModel(this)), innerDelegate(new ClientsDelegate(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	innerLayout->addWidget(userInfo);
	innerLayout->addLayout(searchPanel);
	innerLayout->addWidget(clientView);
	innerLayout->addWidget(backButton);
	searchPanel->addWidget(searchLine);
	searchPanel->addWidget(doSearchButton);

	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);
	innerLayout->setContentsMargins(0, 0, 0, 0);


	clientView->setModel(innerModel);
	clientView->setItemDelegate(innerDelegate);

	userInfo->setText(tr("Select client"));
	doSearchButton->setText(tr("Search!"));
	doSearchButton->setStyleSheet(CHANGE_BUTTONS_STYLESHEET);
	doSearchButton->setIcon(QIcon(":/res/search.png"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setText(tr("back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	current = innerWidget;
	untouchable = innerWidget;
	main = this;

	innerModel->setList(AppWorkset->dataprovider.loadEntities<ClientEntity>());

	QObject::connect(doSearchButton, &MegaIconButton::clicked, this, &ClientSelectionWidget::doSearch);
}

void ClientSelectionWidget::doSearch()
{
}
