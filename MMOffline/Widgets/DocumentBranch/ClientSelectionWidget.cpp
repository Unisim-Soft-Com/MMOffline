#include "ClientSelectionWidget.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
#include <QScroller>
#include "Widgets/ExtendedDelegates/ClientsDelegate.h"

ClientSelectionWidget::ClientSelectionWidget(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),

	userInfo(new QLabel(this)), searchPanel(new QHBoxLayout(this)),
	searchInfo(new QLabel(this)),
	searchLine(new QLineEdit(this)),
	clientView(new QListView(this)),
	buttonPanel(new QHBoxLayout(this)),
	backButton(new MegaIconButton(this)),
	okButton(new MegaIconButton(this)),
	innerModel(new DataCountingDataModel(this)),
	searchProxy(new DataEntityFilterModel(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(userInfo);
	mainLayout->addLayout(searchPanel);
	mainLayout->addWidget(clientView);
	mainLayout->addLayout(buttonPanel);
	buttonPanel->addWidget(backButton);
	buttonPanel->addWidget(okButton);
	searchPanel->addWidget(searchInfo);
	searchPanel->addWidget(searchLine);

	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	buttonPanel->setSpacing(0);
	buttonPanel->setContentsMargins(0, 0, 0, 0);

	userInfo->setText(tr("Select client"));
	userInfo->setAlignment(Qt::AlignCenter);
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setText(tr("back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	okButton->setIcon(QIcon(":/res/submit.png"));
	okButton->setText(tr("OK"));
	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);

	searchInfo->setText(tr("Search: "));
	searchInfo->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

	innerModel->setData(AppWorkset->dataprovider.loadDataAs<ClientEntity>());
	innerModel->assignQuantityInfo(std::move(AppWorkset->dataprovider.loadIdPairs(ComplexFilters::ClientQuantityLinking)));
	searchProxy->setSourceModel(innerModel);
	searchProxy->setFilterRole(DataEntityListModel::SearchRole);
	searchProxy->setFilterFixedString("");
	clientView->setModel(searchProxy);
	clientView->setItemDelegate(new ClientsDelegate(this));
	QScroller::grabGesture(clientView, QScroller::LeftMouseButtonGesture);
	clientView->setVerticalScrollMode(QListView::ScrollPerPixel);
	QObject::connect(searchLine, &QLineEdit::textChanged, searchProxy, &QSortFilterProxyModel::setFilterFixedString);
	QObject::connect(searchProxy, &DataEntityFilterModel::dataEntityClicked, this, &ClientSelectionWidget::clientClicked);
	QObject::connect(clientView, SIGNAL(doubleClicked(const QModelIndex&)), searchProxy, SLOT(mapClickToEntity(const QModelIndex&)));
#ifdef Q_OS_ANDROID
	searchLine->setInputMethodHints(Qt::InputMethodHint::ImhNoPredictiveText);
#endif
	QObject::connect(searchLine, &QLineEdit::returnPressed, QGuiApplication::inputMethod(), &QInputMethod::hide);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &ClientSelectionWidget::backRequired);
	QObject::connect(okButton, &MegaIconButton::clicked, this, &ClientSelectionWidget::okClicked);
}

void ClientSelectionWidget::incrementDocCounter(int id, int q)
{
	innerModel->assignQuantityUpdate(id, q);
}

void ClientSelectionWidget::okClicked()
{
	if (clientView->currentIndex().isValid())
	{
		searchProxy->mapClickToEntity(clientView->currentIndex());
	}
}

void ClientSelectionWidget::clientClicked(DataEntity client)
{
	Client temp = std::static_pointer_cast<ClientEntity>(client);
	emit clientSelected(*temp);
}