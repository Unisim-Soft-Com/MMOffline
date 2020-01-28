#include "ClientSelectionWidget.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Dataprovider/SqliteDataProvider.h"
#include <QScroller>
#include "Widgets/ExtendedDelegates/ClientsDelegate.h"


/*
	This widget loads all data and creates subwidgets on creation, so do not create 
	instances when you don't need them, or make delayed constructor

*/


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
	// emplacing wingets
	this->setLayout(mainLayout);
	mainLayout->addWidget(userInfo);
	mainLayout->addLayout(searchPanel);
	mainLayout->addWidget(clientView);
	mainLayout->addLayout(buttonPanel);
	buttonPanel->addWidget(backButton);
	buttonPanel->addWidget(okButton);
	searchPanel->addWidget(searchInfo);
	searchPanel->addWidget(searchLine);

	// removing spacing and margins to avoid space loss on inframing
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	buttonPanel->setSpacing(0);
	buttonPanel->setContentsMargins(0, 0, 0, 0);

	// installing button and label appearances
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




	// establing models and view
	auto temp = AppData->loadDataAs(DataEntity(new ClientEntity()));
	std::sort(temp.begin(), temp.end(), [](const DataEntity a, const DataEntity b) {return !a->sortingCompare(&(*(b))); });
	innerModel->setData(temp);
	innerModel->assignQuantityInfo(std::move(AppData->loadIdPairs(ComplexFilters::ClientQuantityLinking)));
	searchProxy->setSourceModel(innerModel);
	searchProxy->setFilterRole(DataEntityListModel::SearchRole);
	searchProxy->setFilterFixedString("");
	clientView->setModel(searchProxy);

	// setting view appearance and scroll
	clientView->setItemDelegate(new ClientsDelegate(this));
	QScroller::grabGesture(clientView, QScroller::LeftMouseButtonGesture);
	clientView->setVerticalScrollMode(QListView::ScrollPerPixel);
	
	// connecting slots
	QObject::connect(searchLine, &QLineEdit::textChanged, searchProxy, &QSortFilterProxyModel::setFilterFixedString);
	QObject::connect(searchProxy, &DataEntityFilterModel::dataEntityClicked, this, &ClientSelectionWidget::clientClicked);
	QObject::connect(clientView, SIGNAL(doubleClicked(const QModelIndex&)), searchProxy, SLOT(mapClickToEntity(const QModelIndex&)));
	QObject::connect(searchLine, &QLineEdit::returnPressed, QGuiApplication::inputMethod(), &QInputMethod::hide);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &ClientSelectionWidget::backRequired);
	QObject::connect(okButton, &MegaIconButton::clicked, this, &ClientSelectionWidget::okClicked);

	// on android disable prediction for insta-search
#ifdef Q_OS_ANDROID
	searchLine->setInputMethodHints(Qt::InputMethodHint::ImhNoPredictiveText);
#endif
}

void ClientSelectionWidget::incrementDocCounter(IdInt id, int q)
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