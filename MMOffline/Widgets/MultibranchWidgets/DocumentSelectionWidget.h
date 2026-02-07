#pragma once
#include "Widgets/parents/inframedWidget.h"
#include "Dataprovider/DataEntities.h"
#include "Widgets/ElementWidgets/OutlineActionButton.h"
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QListView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QPushButton>


/*
    This widget is used for selecting document. It is branch independent.
    It allows to change document representation in it without affecting database.
    It allows user to select action with document, but does not performs it.
    It does not needed to be primed.

    Modernized version with:
    - Blue gradient header
    - Card-based document display (ModernDocumentsDelegate)
    - Styled search field with pagination (15 items at a time)
    - Three outline buttons: Back, Delete, Confirm
    - StatusDialog for confirmations and warnings

    Affected tables:
    V       Documents
*/

// Simple proxy model to limit visible rows for pagination
class LimitedProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit LimitedProxyModel(QObject* parent = nullptr)
        : QSortFilterProxyModel(parent), m_limit(ITEMS_PER_PAGE) {}

    int rowCount(const QModelIndex& parent = QModelIndex()) const override
    {
        int actual = QSortFilterProxyModel::rowCount(parent);
        return qMin(actual, m_limit);
    }

    int totalCount() const { return QSortFilterProxyModel::rowCount(); }
    bool hasMore() const { return totalCount() > m_limit; }
    int currentLimit() const { return m_limit; }

    void showMore()
    {
        beginResetModel();
        m_limit += ITEMS_PER_PAGE;
        endResetModel();
    }

    void resetLimit()
    {
        beginResetModel();
        m_limit = ITEMS_PER_PAGE;
        endResetModel();
    }

    static const int ITEMS_PER_PAGE = 15;



private:
    int m_limit;
};

class DocumentSelectionWidget : public inframedWidget
{
    Q_OBJECT
protected:
    // Layout
    QVBoxLayout* mainLayout;
    QWidget* headerWidget;
    QLabel* headerLabel;
    QWidget* searchContainer;
    QLineEdit* searchField;

    // List
    QListView* mainView;
    DataEntityListModel* coreModel;
    DataEntityFilterModel* filterModel;
    LimitedProxyModel* limitModel;


    // "Show more" button
    QPushButton* showMoreButton;

    // Footer buttons
    QHBoxLayout* buttonsLayout;
    OutlineActionButton* backButton;
    OutlineActionButton* deleteButton;
    OutlineActionButton* confirmButton;

public:
    DocumentSelectionWidget(QWidget* parent = nullptr);

    void replaceDocument(Document doc);

    bool eventFilter(QObject* obj, QEvent* event) override;

protected slots:
    void handleConfirm();
    void handleDelete();
    void onShowMore();
    void updateShowMoreVisibility();

signals:
    void DocumentSelected(Document doc, int action);
};
