#include "ClientSelectionWidget.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Dataprovider/SqliteDataProvider.h"
#include <QScroller>
#include "Widgets/ExtendedDelegates/ClientsDelegate.h"
#include "Widgets/ElementWidgets/OutlineActionButton.h"
#include "Widgets/utils/SvgHelper.h"

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
    backButton(new OutlineActionButton(OutlineActionButton::Blue, this)),
    okButton(new OutlineActionButton(OutlineActionButton::Green, this)),
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
    buttonPanel->setSpacing(12);  // Space between buttons
    buttonPanel->setContentsMargins(12, 8, 12, 12);  // Margins around button panel

	// installing button and label appearances
	userInfo->setText(tr("Select client"));
	userInfo->setAlignment(Qt::AlignCenter);

    backButton->setIcon(SvgHelper::iconFromSvg(SvgHelper::SVG_ARROW_LEFT, "#1976D2", QSize(28, 28)));
    backButton->setText(tr("Back"));

    okButton->setIcon(SvgHelper::iconFromSvg(SvgHelper::SVG_CHECK, "#43A047", QSize(28, 28)));
    okButton->setText(tr("OK"));

	searchInfo->setText(tr("Search: "));
	searchInfo->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

    // Modern styling for header
    userInfo->setStyleSheet(
        "QLabel {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #1976D2, stop:1 #1565C0);"
        "   color: white;"
        "   padding: 16px;"
        "   font-size: 20px;"
        "   font-weight: 500;"
        "}"
        );

    // Modern search container styling
    searchPanel->setContentsMargins(12, 12, 12, 12);
    searchPanel->setSpacing(0);
    searchInfo->hide();  // Hide "Search:" label, use placeholder instead

    searchLine->setPlaceholderText(tr("Search..."));
    searchLine->setStyleSheet(
        "QLineEdit {"
        "   padding: 12px 16px;"
        "   border: 1px solid #E0E0E0;"
        "   border-radius: 10px;"
        "   font-size: 15px;"
        "   background: #F8F9FA;"
        "}"
        "QLineEdit:focus {"
        "   border: 2px solid #1976D2;"
        "   background: white;"
        "}"
        );

    // Background for list
    clientView->setStyleSheet(
        "QListView {"
        "   background: #F5F5F5;"
        "   border: none;"
        "   padding: 4px;"
        "}"
        );

    // Background for button panel
    // Create a widget wrapper if needed, or style via parent
    searchInfo->setStyleSheet("QLabel { padding: 10px; font-size: 14px; }");

    // Background for list
    clientView->setStyleSheet("QListView { background: #F5F5F5; border: none; }");




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

    // Connect Report button click from delegate
    ClientsDelegate* delegate = qobject_cast<ClientsDelegate*>(clientView->itemDelegate());
    if (delegate) {
        QObject::connect(delegate, &ClientsDelegate::reportClicked, this, &ClientSelectionWidget::onReportClicked);
    }

    // Deselect when clicking on empty area
    QObject::connect(clientView, &QListView::clicked, this, [this](const QModelIndex& index) {
        if (!index.isValid()) {
            clientView->clearSelection();
        }
    });
    // Also handle clicks on empty space
    clientView->viewport()->installEventFilter(this);
    userInfo->installEventFilter(this);
    userInfo->setCursor(Qt::PointingHandCursor);

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

void ClientSelectionWidget::onReportClicked(const QModelIndex& index)
{
    QModelIndex sourceIndex = searchProxy->mapToSource(index);
    DataEntity entity = innerModel->data(sourceIndex, DataEntityListModel::DataCopyRole).value<DataEntity>();
    if (entity) {
        Client client = std::static_pointer_cast<ClientEntity>(entity);
        if (client) {
            emit reportRequested(*client);
            // TODO: For now, just log or show message
            qDebug() << "Report requested for client:" << client->id << client->name;
        }
    }
}

bool ClientSelectionWidget::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == userInfo && event->type() == QEvent::MouseButtonPress) {
        clientView->clearSelection();
        searchLine->clearFocus();
        QGuiApplication::inputMethod()->hide();
        return true;
    }

    if (obj == clientView->viewport() && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QModelIndex index = clientView->indexAt(mouseEvent->pos());
        if (!index.isValid()) {
            clientView->clearSelection();
            return true;
        }
    }
    return inframedWidget::eventFilter(obj, event);
}
