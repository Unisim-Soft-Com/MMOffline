#include "DocumentSelectionWidget.h"
#include "Dataprovider/SqliteDataProvider.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Widgets/ExtendedDelegates/ModernDocumentsDelegate.h"
#include "Widgets/utils/SvgHelper.h"
#include "Widgets/ElementWidgets/StatusDialog.h"
#include <QScroller>

DocumentSelectionWidget::DocumentSelectionWidget(QWidget* parent)
    : mainLayout(new QVBoxLayout(this)),
    mainView(new QListView(this)),
    coreModel(new DataEntityListModel(this)),
    filterModel(new DataEntityFilterModel(this)),
    limitModel(new LimitedProxyModel(this)),
    searchField(new QLineEdit(this)),
    showMoreButton(new QPushButton(this)),
    buttonsLayout(new QHBoxLayout()),
    backButton(new OutlineActionButton(OutlineActionButton::Blue, this)),
    deleteButton(new OutlineActionButton(OutlineActionButton::Red, this)),
    confirmButton(new OutlineActionButton(OutlineActionButton::Green, this))
{
    // ==================== Background ====================
    this->setStyleSheet("DocumentSelectionWidget { background-color: #F3F6FB; }");

    this->setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ==================== Header ====================
    headerWidget = new QWidget(this);
    headerWidget->setFixedHeight(56);
    headerWidget->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        " stop:0 #1565C0, stop:0.5 #1976D2, stop:1 #2196F3);"
        );

    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(16, 0, 16, 0);

    headerLabel = new QLabel(tr("Accumulated info"), headerWidget);
    headerLabel->setStyleSheet(
        "color: #FFFFFF;"
        " font-size: 19px;"
        " font-weight: 600;"
        " background: transparent;"
        );
    headerLabel->setAlignment(Qt::AlignCenter);
    headerLayout->addWidget(headerLabel);

    mainLayout->addWidget(headerWidget);

    // ==================== Search field ====================
    searchContainer = new QWidget(this);
    searchContainer->setStyleSheet("background: #F3F6FB;");
    QVBoxLayout* searchLayout = new QVBoxLayout(searchContainer);
    searchLayout->setContentsMargins(16, 12, 16, 8);

    searchField->setPlaceholderText(tr("Search..."));
    searchField->setStyleSheet(
        "QLineEdit {"
        " background: #FFFFFF;"
        " border: 1.5px solid #E2E8F0;"
        " border-radius: 12px;"
        " padding: 10px 16px;"
        " font-size: 15px;"
        " color: #334155;"
        "}"
        "QLineEdit:focus {"
        " border-color: #1976D2;"
        "}"
        "QLineEdit::placeholder {"
        " color: #94A3B8;"
        "}"
        );
    searchLayout->addWidget(searchField);
    mainLayout->addWidget(searchContainer);

    // ==================== List view ====================
    mainView->setStyleSheet(
        "QListView {"
        " background: transparent;"
        " border: none;"
        " outline: none;"
        "}"
        "QListView::item {"
        " background: transparent;"
        " border: none;"
        "}"
        "QListView::item:selected {"
        " background: transparent;"
        "}"
        );

    // Setting up models: coreModel -> filterModel -> limitModel -> view
    coreModel->setData(AppData->loadDataAs(DataEntity(new DocumentEntity())));
    filterModel->setSourceModel(coreModel);
    filterModel->setFilterRole(DataEntityListModel::SearchRole);
    limitModel->setSourceModel(filterModel);
    mainView->setModel(limitModel);

    // Setting up view
    mainView->setItemDelegate(new ModernDocumentsDelegate(this));
    QScroller::grabGesture(mainView, QScroller::LeftMouseButtonGesture);
    mainView->setVerticalScrollMode(QListView::ScrollPerPixel);

    mainLayout->addWidget(mainView, 1);

    // ==================== "Show more" button ====================
    showMoreButton->setText(tr("Show more..."));
    showMoreButton->setStyleSheet(
        "QPushButton {"
        " background: #FFFFFF;"
        " border: 1.5px solid #1976D2;"
        " border-radius: 12px;"
        " color: #1976D2;"
        " font-size: 14px;"
        " font-weight: 600;"
        " padding: 10px 0;"
        " margin: 4px 16px 8px 16px;"
        "}"
        "QPushButton:pressed {"
        " background: #E3F2FD;"
        "}"
        );
    showMoreButton->setVisible(false);
    mainLayout->addWidget(showMoreButton);

    // ==================== Footer buttons ====================
    QWidget* footerWidget = new QWidget(this);
    footerWidget->setStyleSheet("background: #FFFFFF;");
    footerWidget->setFixedHeight(80);

    buttonsLayout = new QHBoxLayout(footerWidget);
    buttonsLayout->setContentsMargins(12, 8, 12, 8);
    buttonsLayout->setSpacing(10);

    // Back button (blue outline)
    backButton->setText(tr("back"));
    backButton->setIcon(SvgHelper::iconFromSvg(SvgHelper::SVG_ARROW_LEFT,
                                               SvgHelper::Colors::BTN_OUTLINE_BLUE, QSize(24, 24)));

    // Delete button (red outline)
    deleteButton->setText(tr("delete"));
    deleteButton->setIcon(SvgHelper::iconFromSvg(SvgHelper::SVG_TRASH,
                                                 SvgHelper::Colors::BTN_OUTLINE_RED, QSize(24, 24)));

    // Confirm button (green outline)
    confirmButton->setText(tr("confirm"));
    confirmButton->setIcon(SvgHelper::iconFromSvg(SvgHelper::SVG_CHECK,
                                                  SvgHelper::Colors::BTN_OUTLINE_GREEN, QSize(24, 24)));

    buttonsLayout->addWidget(backButton);
    buttonsLayout->addWidget(deleteButton);
    buttonsLayout->addWidget(confirmButton);

    mainLayout->addWidget(footerWidget);

    // ==================== Connections ====================
    QObject::connect(searchField, &QLineEdit::textChanged, this, [this](const QString& text) {
        filterModel->setFilterFixedString(text);
        limitModel->resetLimit();
        updateShowMoreVisibility();
    });

    QObject::connect(mainView, &QListView::doubleClicked,
                     this, &DocumentSelectionWidget::handleConfirm);
    QObject::connect(backButton, &OutlineActionButton::clicked,
                     this, &DocumentSelectionWidget::backRequired);
    QObject::connect(confirmButton, &OutlineActionButton::clicked,
                     this, &DocumentSelectionWidget::handleConfirm);
    QObject::connect(deleteButton, &OutlineActionButton::clicked,
                     this, &DocumentSelectionWidget::handleDelete);
    QObject::connect(showMoreButton, &QPushButton::clicked,
                     this, &DocumentSelectionWidget::onShowMore);

    // Initial visibility check
    updateShowMoreVisibility();

    // Allow deselection by tapping outside items
    headerWidget->installEventFilter(this);
    searchContainer->installEventFilter(this);
    mainView->viewport()->installEventFilter(this);

    // Disable prediction on android for instant search
#ifdef Q_OS_ANDROID
    searchField->setInputMethodHints(Qt::InputMethodHint::ImhNoPredictiveText);
#endif
}

void DocumentSelectionWidget::replaceDocument(Document doc)
{
    coreModel->replaceDataEntity(doc);
    updateShowMoreVisibility();
}

void DocumentSelectionWidget::handleConfirm()
{
    if (!mainView->currentIndex().isValid())
    {
        // Warning: nothing selected
        StatusDialog warning(tr("Please select a document before continuing."), this);
        warning.setTitle(tr("No selection"));
        warning.setWarningMode(true);
        warning.smartExec(this);
        return;
    }

    // Making copy of document to avoid unnecessary shadow changes
    QModelIndex sourceIndex = limitModel->mapToSource(mainView->currentIndex());
    Document temp =
        std::dynamic_pointer_cast<DocumentEntity>(
            sourceIndex.data(DataEntityListModel::DataCopyRole).value<DataEntity>()
            );
    if (temp != nullptr)
        emit DocumentSelected(temp, 1);

    searchField->clear();
    mainView->clearSelection();
}

void DocumentSelectionWidget::handleDelete()
{
    if (!mainView->currentIndex().isValid())
    {
        // Warning: nothing selected
        StatusDialog warning(tr("Please select a document to delete."), this);
        warning.setTitle(tr("No selection"));
        warning.setWarningMode(true);
        warning.smartExec(this);
        return;
    }

    // Confirm deletion with StatusDialog
    StatusDialog confirm(tr("Are you sure you want to delete this document?"), this);
    confirm.setTitle(tr("Delete document"));
    confirm.setDangerMode(true);
    confirm.setOkText(tr("Delete"));
    confirm.setCancelText(tr("Cancel"));

    if (confirm.smartExec(this) != QDialog::Accepted)
        return;

    QModelIndex proxyIndex = mainView->currentIndex();
    QModelIndex filterIndex = limitModel->mapToSource(proxyIndex);
    QModelIndex sourceIndex = filterModel->mapToSource(filterIndex);

    Document temp =
        std::dynamic_pointer_cast<DocumentEntity>(
            sourceIndex.data(DataEntityListModel::DataCopyRole).value<DataEntity>()
            );
    if (temp != nullptr)
    {
        coreModel->removeDataEntity(sourceIndex);
        limitModel->invalidate();
        emit DocumentSelected(temp, 0);
    }

    searchField->clear();
    mainView->clearSelection();
    updateShowMoreVisibility();
}

void DocumentSelectionWidget::onShowMore()
{
    limitModel->showMore();
    updateShowMoreVisibility();
}

void DocumentSelectionWidget::updateShowMoreVisibility()
{
    bool hasMore = limitModel->hasMore();
    showMoreButton->setVisible(hasMore);

    if (hasMore)
    {
        int showing = limitModel->rowCount();
        int total = limitModel->totalCount();
        showMoreButton->setText(
            tr("Show more (%1 of %2)...").arg(showing).arg(total)
            );
    }
}


bool DocumentSelectionWidget::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        // Tapping on header or search clears selection
        if (obj == headerWidget || obj == searchContainer)
        {
            mainView->clearSelection();
            mainView->setCurrentIndex(QModelIndex());
            return false;
        }

        // Tapping on empty area in list clears selection
        if (obj == mainView->viewport())
        {
            QMouseEvent* me = static_cast<QMouseEvent*>(event);
            QModelIndex index = mainView->indexAt(me->pos());
            if (!index.isValid())
            {
                mainView->clearSelection();
                mainView->setCurrentIndex(QModelIndex());
                return true;
            }
        }
    }
    return inframedWidget::eventFilter(obj, event);
}
