#include "ModernSelectionDialog.h"
#include "qdebug.h"
#include <QPainter>
#include <QTimer>

ModernSelectionDialog::ModernSelectionDialog(const QString& title,
                                             const QStringList& items,
                                             int currentIndex,
                                             QWidget* parent)
    : QDialog(parent), m_items(items), m_selectedIndex(currentIndex)
{
    setupUI(title);
    populateList();
}

void ModernSelectionDialog::setupUI(const QString& title)
{
    // Full-screen frameless dialog
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setModal(true);

    // Fill the entire top-level window
    QWidget* topLevel = parentWidget();
    while (topLevel && topLevel->parentWidget())
        topLevel = topLevel->parentWidget();

    if (topLevel) {
        setFixedSize(topLevel->size());
        move(topLevel->mapToGlobal(QPoint(0, 0)));
    }

    // === White card (no layout manager — positioned manually) ===
    cardFrame = new QFrame(this);
    cardFrame->setObjectName("modernSelCard");
    cardFrame->setStyleSheet(
        "#modernSelCard {"
        "  background-color: #FFFFFF;"
        "  border-radius: 16px;"
        "}"
        );

    cardLayout = new QVBoxLayout(cardFrame);
    cardLayout->setContentsMargins(0, 0, 0, 0);
    cardLayout->setSpacing(0);

    // === Header ===
    headerFrame = new QFrame(cardFrame);
    headerFrame->setMinimumHeight(54);
    headerFrame->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "  stop:0 #1976D2, stop:1 #1565C0);"
        "border-top-left-radius: 16px;"
        "border-top-right-radius: 16px;"
        );

    QHBoxLayout* headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(20, 14, 14, 14);
    headerLayout->setSpacing(10);

    titleLabel = new QLabel(title, headerFrame);
    titleLabel->setStyleSheet(
        "background: transparent;"
        "color: #FFFFFF;"
        "font-size: 17px;"
        "font-weight: 600;"
        );

    closeButton = new QPushButton(headerFrame);
    closeButton->setText(QString::fromUtf8("\xC3\x97")); // ×
    closeButton->setFixedSize(32, 32);
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setStyleSheet(
        "QPushButton {"
        "  background: rgba(255, 255, 255, 50);"
        "  border: none;"
        "  border-radius: 16px;"
        "  color: #FFFFFF;"
        "  font-size: 18px;"
        "  font-weight: 600;"
        "}"
        "QPushButton:pressed {"
        "  background: rgba(255, 255, 255, 90);"
        "}"
        );
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);

    headerLayout->addWidget(titleLabel, 1);
    headerLayout->addWidget(closeButton);
    cardLayout->addWidget(headerFrame);

    // === Search bar ===
    QFrame* searchFrame = new QFrame(cardFrame);
    searchFrame->setStyleSheet(
        "background-color: #FFFFFF;"
        "border-bottom: 1px solid #E5E7EB;"
        );

    QHBoxLayout* searchLayout = new QHBoxLayout(searchFrame);
    searchLayout->setContentsMargins(16, 12, 16, 12);

    searchField = new QLineEdit(searchFrame);
    searchField->setPlaceholderText(tr("Search..."));
    searchField->setClearButtonEnabled(true);
    searchField->setMinimumHeight(42);
    searchField->setStyleSheet(
        "QLineEdit {"
        "  background-color: #F8FAFC;"
        "  border: 2px solid #E2E8F0;"
        "  border-radius: 10px;"
        "  padding: 8px 14px;"
        "  font-size: 14px;"
        "  color: #1F2937;"
        "}"
        "QLineEdit:focus {"
        "  border-color: #1976D2;"
        "  background-color: #FFFFFF;"
        "}"
        );
    connect(searchField, &QLineEdit::textChanged,
            this, &ModernSelectionDialog::onSearchTextChanged);

    searchLayout->addWidget(searchField);
    cardLayout->addWidget(searchFrame);

    // === List ===
    listWidget = new QListWidget(cardFrame);
    listWidget->setFrameShape(QFrame::NoFrame);
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    listWidget->setFocusPolicy(Qt::NoFocus);
    listWidget->setSelectionMode(QAbstractItemView::NoSelection);
    listWidget->setStyleSheet(
        "QListWidget {"
        "  background-color: #FFFFFF;"
        "  border-bottom-left-radius: 16px;"
        "  border-bottom-right-radius: 16px;"
        "  outline: none;"
        "}"
        "QListWidget::item {"
        "  padding: 14px 20px;"
        "  border-bottom: 1px solid #F3F4F6;"
        "  color: #1F2937;"
        "  font-size: 15px;"
        "}"
        "QListWidget::item:hover {"
        "  background-color: transparent;"
        "}"
        "QListWidget::item:selected {"
        "  background-color: transparent;"
        "  color: #1F2937;"
        "}"
            "QListWidget QScrollBar:vertical {"
                "background: transparent;"
                "width: 4px;"
                "margin: 8px 2px;"
            "}"
            "QListWidget QScrollBar::handle:vertical {"
                "background: #CBD5E1;"
                "border-radius: 2px;"
                "min-height: 30px;"
            "}"
        "QListWidget QScrollBar::add-line:vertical,"
        "QListWidget QScrollBar::sub-line:vertical,"
        "QListWidget QScrollBar::add-page:vertical,"
        "QListWidget QScrollBar::sub-page:vertical {"
            "height: 0px;"
            "background: transparent;"
        "}"
        );

    connect(listWidget, &QListWidget::itemClicked,
            this, &ModernSelectionDialog::onItemClicked);

    // Touch scrolling — disable QListWidget's own kinetic scroll
    listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

#ifdef Q_OS_ANDROID
    listWidget->setAttribute(Qt::WA_AcceptTouchEvents, true);
#endif
    QScroller::grabGesture(listWidget->viewport(), QScroller::TouchGesture);
    QScrollerProperties props = QScroller::scroller(listWidget->viewport())->scrollerProperties();
    props.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy,
                          QScrollerProperties::OvershootAlwaysOff);
    props.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy,
                          QScrollerProperties::OvershootAlwaysOff);
    props.setScrollMetric(QScrollerProperties::DragStartDistance, 0.008);
    props.setScrollMetric(QScrollerProperties::DragVelocitySmoothingFactor, 0.8);
    props.setScrollMetric(QScrollerProperties::MaximumVelocity, 0.15);
    props.setScrollMetric(QScrollerProperties::MinimumVelocity, 0.01);
    props.setScrollMetric(QScrollerProperties::DecelerationFactor, 0.2);
    props.setScrollMetric(QScrollerProperties::AcceleratingFlickMaximumTime, 0.1);
    props.setScrollMetric(QScrollerProperties::AcceleratingFlickSpeedupFactor, 1.0);
    props.setScrollMetric(QScrollerProperties::AxisLockThreshold, 1.0);
    QScroller::scroller(listWidget->viewport())->setScrollerProperties(props);

    // Track when scrolling stops
    connect(QScroller::scroller(listWidget->viewport()), &QScroller::stateChanged,
            this, [this](QScroller::State newState) {
                static QScroller::State prevState = QScroller::Inactive;
                if (newState == QScroller::Inactive &&
                    (prevState == QScroller::Scrolling || prevState == QScroller::Dragging)) {
                    m_scrollTimer.restart();
                }
                prevState = newState;
            });
    m_scrollTimer.start();

    m_wasScrolling = false;
    listWidget->viewport()->installEventFilter(this);

    cardLayout->addWidget(listWidget, 1);

    positionCard();
}

void ModernSelectionDialog::positionCard()
{
    int w = width();
    int h = height();

    // Card: 86% width, max 70% height, centered
    int cardW = qMin((int)(w * 0.86), 380);
    int cardH = qMin((int)(h * 0.70), 520);
    int cardX = (w - cardW) / 2;
    int cardY = (h - cardH) / 2;

    cardFrame->setGeometry(cardX, cardY, cardW, cardH);
}

void ModernSelectionDialog::populateList()
{
    listWidget->clear();
    m_loadedCount = 0;
    loadMoreItems();

    // Scroll to selected if visible
    if (m_selectedIndex >= 0 && m_selectedIndex < listWidget->count()) {
        listWidget->scrollToItem(listWidget->item(m_selectedIndex),
                                 QAbstractItemView::PositionAtCenter);
    }
}

void ModernSelectionDialog::loadMoreItems()
{
    // Remove "More..." item if exists
    if (listWidget->count() > 0) {
        QListWidgetItem* lastItem = listWidget->item(listWidget->count() - 1);
        if (lastItem->data(Qt::UserRole).toInt() == -1) {
            delete listWidget->takeItem(listWidget->count() - 1);
        }
    }

    int end = qMin(m_loadedCount + BATCH_SIZE, m_items.size());

    for (int i = m_loadedCount; i < end; ++i) {
        QString text = m_items.at(i);

        if (i == m_selectedIndex)
            text = QString::fromUtf8("\xe2\x9c\x93  ") + text;

        QListWidgetItem* item = new QListWidgetItem(text);
        item->setData(Qt::UserRole, i);
        item->setSizeHint(QSize(0, 50));
        listWidget->addItem(item);
    }

    m_loadedCount = end;

    // Add "More..." if there are remaining items
    if (m_loadedCount < m_items.size()) {
        int remaining = m_items.size() - m_loadedCount;
        QListWidgetItem* moreItem = new QListWidgetItem(
            QString("▼  More... (%1 remaining)").arg(remaining));
        moreItem->setData(Qt::UserRole, -1);  // Special marker
        moreItem->setSizeHint(QSize(0, 50));
        moreItem->setTextAlignment(Qt::AlignCenter);
        listWidget->addItem(moreItem);
    }
}

void ModernSelectionDialog::onSearchTextChanged(const QString& text)
{
    if (!text.isEmpty() && m_loadedCount < m_items.size()) {
        // Load all items for search
        m_loadedCount = 0;
        listWidget->clear();
        for (int i = 0; i < m_items.size(); ++i) {
            QString itemText = m_items.at(i);
            if (i == m_selectedIndex)
                itemText = QString::fromUtf8("\xe2\x9c\x93  ") + itemText;
            QListWidgetItem* item = new QListWidgetItem(itemText);
            item->setData(Qt::UserRole, i);
            item->setSizeHint(QSize(0, 50));
            listWidget->addItem(item);
        }
        m_loadedCount = m_items.size();
    }

    for (int i = 0; i < listWidget->count(); ++i) {
        QListWidgetItem* item = listWidget->item(i);
        int idx = item->data(Qt::UserRole).toInt();
        if (idx == -1) { item->setHidden(true); continue; }
        bool matches = m_items.at(idx).contains(text, Qt::CaseInsensitive);
        item->setHidden(!matches);
    }
}

void ModernSelectionDialog::onItemClicked(QListWidgetItem* item)
{
    if (m_wasScrolling) {
        m_wasScrolling = false;
        return;
    }

    // "More..." clicked — load next batch
    if (item->data(Qt::UserRole).toInt() == -1) {
        loadMoreItems();
        return;
    }

    item->setBackground(QColor("#EFF6FF"));
    item->setForeground(QColor("#1976D2"));

    m_selectedIndex = item->data(Qt::UserRole).toInt();

    QTimer::singleShot(150, this, [this]() {
        accept();
    });
}

void ModernSelectionDialog::resizeEvent(QResizeEvent* event)
{
    QDialog::resizeEvent(event);
    positionCard();
}

bool ModernSelectionDialog::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == listWidget->viewport()) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* me = static_cast<QMouseEvent*>(event);
            m_pressPos = me->pos();
            m_wasScrolling = false;
        }
        else if (event->type() == QEvent::MouseMove) {
            QMouseEvent* me = static_cast<QMouseEvent*>(event);
            if ((me->pos() - m_pressPos).manhattanLength() > 10) {
                m_wasScrolling = true;
            }
        }
    }
    return QDialog::eventFilter(obj, event);
}

void ModernSelectionDialog::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    // Draw semi-transparent dark overlay
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0, 0, 0, 140));
}

void ModernSelectionDialog::mousePressEvent(QMouseEvent* event)
{
    // Click outside card = close
    if (!cardFrame->geometry().contains(event->pos())) {
        reject();
        return;
    }
    QDialog::mousePressEvent(event);
}

int ModernSelectionDialog::selectedIndex() const
{
    return m_selectedIndex;
}

QString ModernSelectionDialog::selectedText() const
{
    if (m_selectedIndex >= 0 && m_selectedIndex < m_items.size())
        return m_items.at(m_selectedIndex);
    return QString();
}
