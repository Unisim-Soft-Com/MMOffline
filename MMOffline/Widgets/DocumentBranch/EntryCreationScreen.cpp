#include "EntryCreationScreen.h"
#include "Dataprovider/SqliteDataProvider.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Widgets/utils/SvgHelper.h"
#include "Widgets/utils/ModernSelectionDialog.h"
#include "Widgets/utils/ElidedComboBox.h"
#include <QGraphicsDropShadowEffect>
#include <QScroller>
#include <QScrollerProperties>
#include <QEvent>

// This query is appended to select_filtered to check if product is unique
const QString entryLoadQuery
    = QStringLiteral(" parentDocId = %1 and productId = %2");

EntryCreationScreen::EntryCreationScreen(QWidget* parent)
    : inframedWidget(parent),
    currentSOptionIndex(0), currentTOptionIndex(0)
{
    // Main layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);

    // Build UI sections
    setupHeader();
    setupProductBanner();
    setupContent();
    setupFooter();

    // Apply all styles
    applyStyles();

    this->setMaximumWidth(16777215);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    // Load data from database
    measures = AppData->loadEntities<NamedIdEntity>(QString::null, "Measures");
    options = AppData->loadEntities<NamedIdEntity>(QString::null, "Options");

    for (NamedId& m : measures)
        measureField->addItem(m->name);

    for (NamedId& o : options)
        optionField->addItem(o->name);

    // Fill translatable texts
    fillTexts();

    // Intercept combo popups with modern dialog
    measureField->installEventFilter(this);
    optionField->installEventFilter(this);

    // Connect signals
    QObject::connect(okButton, &OutlineActionButton::clicked, this, &EntryCreationScreen::confirmed);
    QObject::connect(backButton, &OutlineActionButton::clicked, this, &EntryCreationScreen::backRequired);
}

// ==================== Header ====================

void EntryCreationScreen::setupHeader()
{
    headerFrame = new QFrame(this);
    headerFrame->setObjectName("headerFrame");
    headerFrame->setMinimumHeight(calculateAdaptiveButtonHeight(0.08));

    QHBoxLayout* headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(20, 12, 20, 12);

    headerTitle = new QLabel(headerFrame);
    headerTitle->setObjectName("headerTitle");
    headerTitle->setAlignment(Qt::AlignCenter);

    headerLayout->addWidget(headerTitle);
    mainLayout->addWidget(headerFrame);
}

// ==================== Product Banner ====================

void EntryCreationScreen::setupProductBanner()
{
    productBanner = new QFrame(this);
    productBanner->setObjectName("productBanner");
    productBanner->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

    QHBoxLayout* bannerLayout = new QHBoxLayout(productBanner);
    bannerLayout->setContentsMargins(12, 8, 12, 8);
    bannerLayout->setSpacing(10);

    // Avatar circle (first letter)
    productAvatar = new QLabel(productBanner);
    productAvatar->setObjectName("productAvatar");
    productAvatar->setFixedSize(38, 38);
    productAvatar->setAlignment(Qt::AlignCenter);

    // Name + Code column
    QWidget* detailsWidget = new QWidget(productBanner);
    detailsWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    QVBoxLayout* detailsLayout = new QVBoxLayout(detailsWidget);
    detailsLayout->setContentsMargins(0, 0, 0, 0);
    detailsLayout->setSpacing(2);

    productNameLabel = new QLabel(detailsWidget);
    productNameLabel->setObjectName("productName");
    productNameLabel->setWordWrap(false);
    productNameLabel->setMinimumWidth(0);
    productNameLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);

    productCodeLabel = new QLabel(detailsWidget);
    productCodeLabel->setObjectName("productCode");
    productCodeLabel->setWordWrap(false);
    productCodeLabel->setMinimumWidth(0);
    productCodeLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);

    detailsLayout->addWidget(productNameLabel);
    detailsLayout->addWidget(productCodeLabel);

    // Price badge
    productPriceBadge = new QLabel(productBanner);
    productPriceBadge->setObjectName("productPriceBadge");
    productPriceBadge->setAlignment(Qt::AlignCenter);
    productPriceBadge->setMinimumWidth(44);

    bannerLayout->addWidget(productAvatar);
    bannerLayout->addWidget(detailsWidget, 1);
    bannerLayout->addWidget(productPriceBadge);

    mainLayout->addWidget(productBanner);
}

// ==================== Content ====================

void EntryCreationScreen::setupContent()
{
    // Scroll area
    scrollArea = new QScrollArea(this);
    scrollArea->setObjectName("scrollArea");
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setFocusPolicy(Qt::NoFocus);

    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

#ifdef Q_OS_ANDROID
    scrollArea->viewport()->setAttribute(Qt::WA_AcceptTouchEvents, true);
#endif

    QScroller::grabGesture(scrollArea->viewport(), QScroller::TouchGesture);
    QScrollerProperties props = QScroller::scroller(scrollArea->viewport())->scrollerProperties();
    props.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    props.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    props.setScrollMetric(QScrollerProperties::DragStartDistance, 0.002);
    QScroller::scroller(scrollArea->viewport())->setScrollerProperties(props);

    contentWidget = new QWidget();
    contentWidget->setObjectName("contentWidget");
    contentWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    contentWidget->setMinimumWidth(0);
    contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(12, 14, 12, 14);
    contentLayout->setSpacing(12);

    setupDetailsCard();
    contentLayout->addSpacing(20);
    setupQuantityCard();
    contentLayout->addSpacing(20);
    setupCommentCard();
    contentLayout->addStretch();

    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea, 1);
}

// ==================== Details Card (Measure + Option) ====================

void EntryCreationScreen::setupDetailsCard()
{
    detailsCard = new QFrame(contentWidget);
    detailsCard->setObjectName("formCard");
    detailsCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

#ifndef Q_OS_ANDROID
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(detailsCard);
    shadow->setBlurRadius(16);
    shadow->setOffset(0, 3);
    shadow->setColor(QColor(0, 0, 0, 20));
    detailsCard->setGraphicsEffect(shadow);
#endif

    QVBoxLayout* cardLayout = new QVBoxLayout(detailsCard);
    cardLayout->setContentsMargins(16, 16, 16, 16);
    cardLayout->setSpacing(14);

    // Card title with icon
    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->setSpacing(8);
    QLabel* titleIcon = new QLabel(detailsCard);
    titleIcon->setPixmap(SvgHelper::iconFromSvg(SvgHelper::SVG_PACKAGE,
                                                SvgHelper::Colors::ICON_BLUE, QSize(18, 18)).pixmap(18, 18));
    titleIcon->setFixedSize(18, 18);
    QLabel* cardTitle = new QLabel(detailsCard);
    cardTitle->setObjectName("cardTitle");
    cardTitle->setText(tr("Product details"));
    titleLayout->addWidget(titleIcon);
    titleLayout->addWidget(cardTitle, 1);
    cardLayout->addLayout(titleLayout);

    // Measure + Option in two columns
    QHBoxLayout* twoColLayout = new QHBoxLayout();
    twoColLayout->setSpacing(10);

    // Measure
    QVBoxLayout* measureCol = new QVBoxLayout();
    measureCol->setSpacing(6);
    QLabel* measureLabel = new QLabel(detailsCard);
    measureLabel->setObjectName("fieldLabel");
    measureLabel->setText(tr("Measure"));
    measureField = new ElidedComboBox(detailsCard);
    measureField->setObjectName("modernCombo");
    measureField->setMinimumHeight(44);
    measureField->setMinimumWidth(0);
    measureField->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    measureCol->addWidget(measureLabel);
    measureCol->addWidget(measureField);

    // Option
    QVBoxLayout* optionCol = new QVBoxLayout();
    optionCol->setSpacing(6);
    QLabel* optionLabel = new QLabel(detailsCard);
    optionLabel->setObjectName("fieldLabel");
    optionLabel->setText(tr("Option"));
    optionField = new ElidedComboBox(detailsCard);
    optionField->setObjectName("modernCombo");
    optionField->setMinimumHeight(44);
    optionField->setMinimumWidth(0);
    optionField->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    optionCol->addWidget(optionLabel);
    optionCol->addWidget(optionField);

    twoColLayout->addLayout(measureCol, 1);
    twoColLayout->addLayout(optionCol, 1);
    cardLayout->addLayout(twoColLayout);

    contentLayout->addWidget(detailsCard);
}

// ==================== Quantity Card ====================

void EntryCreationScreen::setupQuantityCard()
{
    quantityCard = new QFrame(contentWidget);
    quantityCard->setObjectName("formCard");
    quantityCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

#ifndef Q_OS_ANDROID
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(quantityCard);
    shadow->setBlurRadius(16);
    shadow->setOffset(0, 3);
    shadow->setColor(QColor(0, 0, 0, 20));
    quantityCard->setGraphicsEffect(shadow);
#endif

    QVBoxLayout* cardLayout = new QVBoxLayout(quantityCard);
    cardLayout->setContentsMargins(16, 10, 16, 10);
    cardLayout->setSpacing(8);

    // Card title with icon
    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->setSpacing(8);
    QLabel* titleIcon = new QLabel(quantityCard);
    titleIcon->setPixmap(SvgHelper::iconFromSvg(SvgHelper::SVG_HASH,
                                                SvgHelper::Colors::ICON_BLUE, QSize(18, 18)).pixmap(18, 18));
    titleIcon->setFixedSize(18, 18);
    QLabel* cardTitle = new QLabel(quantityCard);
    cardTitle->setObjectName("cardTitle");
    cardTitle->setText(tr("Quantity"));
    titleLayout->addWidget(titleIcon);
    titleLayout->addWidget(cardTitle, 1);
    cardLayout->addLayout(titleLayout);

    // Amount stepper
    quantityStepper = new ModernAmountStepper(quantityCard);
    quantityStepper->setMinimum(0);
    quantityStepper->setMaximum(1000000);
    quantityStepper->setPrecision(3);
    quantityStepper->setStep(1.0);
    cardLayout->addWidget(quantityStepper);

    contentLayout->addWidget(quantityCard);
}

// ==================== Comment Card ====================

void EntryCreationScreen::setupCommentCard()
{
    commentCard = new QFrame(contentWidget);
    commentCard->setObjectName("formCard");
    commentCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

#ifndef Q_OS_ANDROID
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(commentCard);
    shadow->setBlurRadius(16);
    shadow->setOffset(0, 3);
    shadow->setColor(QColor(0, 0, 0, 20));
    commentCard->setGraphicsEffect(shadow);
#endif

    QVBoxLayout* cardLayout = new QVBoxLayout(commentCard);
    cardLayout->setContentsMargins(16, 10, 16, 10);
    cardLayout->setSpacing(8);

    // Card title with icon
    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->setSpacing(8);
    QLabel* titleIcon = new QLabel(commentCard);
    titleIcon->setPixmap(SvgHelper::iconFromSvg(SvgHelper::SVG_COMMENT,
                                                SvgHelper::Colors::ICON_BLUE, QSize(18, 18)).pixmap(18, 18));
    titleIcon->setFixedSize(18, 18);
    QLabel* cardTitle = new QLabel(commentCard);
    cardTitle->setObjectName("cardTitle");
    cardTitle->setText(tr("Comment"));
    titleLayout->addWidget(titleIcon);
    titleLayout->addWidget(cardTitle, 1);
    cardLayout->addLayout(titleLayout);

    // Comment input
    commentField = new QLineEdit(commentCard);
    commentField->setObjectName("commentInput");
    commentField->setMinimumHeight(44);
    cardLayout->addWidget(commentField);

    contentLayout->addWidget(commentCard);
}

// ==================== Footer ====================

void EntryCreationScreen::setupFooter()
{
    footerWidget = new QWidget(this);
    footerWidget->setObjectName("footerWidget");

    footerLayout = new QHBoxLayout(footerWidget);
    footerLayout->setContentsMargins(8, 10, 8, 10);
    footerLayout->setSpacing(12);

    backButton = new OutlineActionButton(OutlineActionButton::Blue, footerWidget);
    backButton->setIcon(SvgHelper::iconFromSvg(SvgHelper::SVG_ARROW_LEFT,
                                               SvgHelper::Colors::BTN_OUTLINE_BLUE, QSize(24, 24)));

    okButton = new OutlineActionButton(OutlineActionButton::Green, footerWidget);
    okButton->setIcon(SvgHelper::iconFromSvg(SvgHelper::SVG_CHECK,
                                             SvgHelper::Colors::BTN_OUTLINE_GREEN, QSize(24, 24)));

    footerLayout->addWidget(backButton);
    footerLayout->addWidget(okButton);

    mainLayout->addWidget(footerWidget);
}

// ==================== Styles ====================

void EntryCreationScreen::applyStyles()
{
    setStyleSheet(R"(
        /* Main background */
        EntryCreationScreen {
            background-color: #F3F6FB;
        }

        /* Header */
        #headerFrame {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #1976D2, stop:1 #1565C0);
        }
        #headerTitle {
            color: #FFFFFF;
            font-size: 19px;
            font-weight: 600;
        }

        /* Product banner */
        #productBanner {
            background-color: #EFF6FF;
            border-bottom: 1px solid #DBEAFE;
        }
        #productAvatar {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #E3F2FD, stop:1 #BBDEFB);
            border-radius: 10px;
            color: #1565C0;
            font-size: 16px;
            font-weight: 700;
        }
        #productName {
            color: #1F2937;
            font-size: 14px;
            font-weight: 600;
        }
        #productCode {
            color: #6B7280;
            font-size: 11px;
        }
        #productPriceBadge {
            background-color: #1E40AF;
            color: #FFFFFF;
            border-radius: 6px;
            padding: 4px 10px;
            font-size: 12px;
            font-weight: 700;
        }

        /* Content area */
        #contentWidget {
            background-color: #F3F6FB;
        }
        #scrollArea {
            background-color: #F3F6FB;
            border: none;
        }

        /* Form cards */
        #formCard {
            background-color: #FFFFFF;
            border-radius: 12px;
            border: 1px solid #E2E8F0;
        }

        /* Card title */
        #cardTitle {
            color: #1a3a5c;
            font-size: 13px;
            font-weight: 600;
            text-transform: uppercase;
        }

        /* Field labels */
        #fieldLabel {
            color: #6B7280;
            font-size: 12px;
            font-weight: 500;
        }

        /* Combo boxes */
        #modernCombo {
            background-color: #FAFAFA;
            border: 2px solid #E2E8F0;
            border-radius: 10px;
            padding: 8px 12px;
            color: #1F2937;
            font-size: 14px;
        }
        #modernCombo::drop-down {
            border: none;
            width: 0px;
        }
        #modernCombo::down-arrow {
            width: 12px;
            height: 12px;
        }
        #modernCombo QAbstractItemView {
            background-color: #FFFFFF;
            border: 1px solid #E2E8F0;
            border-radius: 8px;
            selection-background-color: #EFF6FF;
            selection-color: #1F2937;
            padding: 4px;
        }

        /* Comment input */
        #commentInput {
            background-color: #FAFAFA;
            border: 2px solid #E2E8F0;
            border-radius: 10px;
            padding: 8px 12px;
            color: #1F2937;
            font-size: 14px;
        }
        #commentInput:focus {
            border-color: #1976D2;
            background-color: #FFFFFF;
        }

        /* Footer */
        #footerWidget {
            background-color: #FFFFFF;
            border-top: 1px solid #E2E8F0;
        }
    )");
}

// ==================== Fill Texts ====================

void EntryCreationScreen::fillTexts()
{
    headerTitle->setText(tr("Product editing"));
    backButton->setText(tr("Back"));
    okButton->setText(tr("OK"));
    commentField->setPlaceholderText(tr("Add a comment (optional)..."));
}

// ==================== Update Product Banner ====================

void EntryCreationScreen::updateProductBanner()
{
    if (operatedProduct == nullptr) return;

    productNameLabel->setText(operatedProduct->name);
    productCodeLabel->setText(tr("ID: %1").arg(operatedProduct->id));
    productPriceBadge->setText(QString::number(operatedProduct->price, 'f', 2));

    QString firstLetter = operatedProduct->name.trimmed().left(1).toUpper();
    productAvatar->setText(firstLetter.isEmpty() ? "?" : firstLetter);
}

// ==================== Event Filter ====================

bool EntryCreationScreen::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::TouchBegin)
    {
        QComboBox* combo = qobject_cast<QComboBox*>(obj);
        if (combo && (combo == measureField || combo == optionField))
        {
            // Build items list
            QStringList items;
            for (int i = 0; i < combo->count(); ++i)
                items << combo->itemText(i);

            // Determine title
            QString title = (combo == measureField)
                                ? tr("Select Measure")
                                : tr("Select Option");

            // Show modern dialog
            ModernSelectionDialog dlg(title, items, combo->currentIndex(), this);
            if (dlg.exec() == QDialog::Accepted) {
                combo->setCurrentIndex(dlg.selectedIndex());
            }

            return true;  // Consume event - don't show native popup
        }
    }
    return inframedWidget::eventFilter(obj, event);
}

// ==================== Prime Entry (Product + Document) ====================

void EntryCreationScreen::primeEntryCreation(Product p, Document doc)
{
    if (p == nullptr || doc == nullptr)
        return;

    operatedProduct.reset(new ProductEntity(*p));
    updateProductBanner();

    // Uniqueness check
    currentEntry = AppData->loadEntityAs<DocumentEntryEntity>(
        entryLoadQuery.arg(doc->documentId).arg(p->id));

    if (currentEntry->entryId == 0)
    {
        // New entry
        currentEntry->productName = p->name;
        currentEntry->entryId = IdGenerator::generateId();
        currentEntry->parentDocId = doc->documentId;
        currentEntry->price = p->price;
        currentEntry->productId = p->id;

        // Reset fields to defaults
        measureField->setCurrentIndex(0);
        optionField->setCurrentIndex(0);
        currentSOptionIndex = 0;
        currentTOptionIndex = 0;
        quantityStepper->setValue(0);
        commentField->clear();
    }
    else
    {
        // Existing entry — load values
        int index = findNamedId(currentEntry->measure, measures);
        if (index != -1) measureField->setCurrentIndex(index);
        index = findNamedId(currentEntry->option1, options);
        if (index != -1) optionField->setCurrentIndex(index);
        index = findNamedId(currentEntry->option2, options);
        if (index != -1) currentSOptionIndex = index;
        index = findNamedId(currentEntry->option3, options);
        if (index != -1) currentTOptionIndex = index;
        quantityStepper->setValue(currentEntry->quantity);
        commentField->setText(currentEntry->comment);
    }
}

// ==================== Prime Entry (from existing DocumentEntry) ====================

void EntryCreationScreen::primeEntryCreation(DocumentEntry e)
{
    if (e == nullptr)
        return;

    operatedProduct = AppData->loadEntityById<ProductEntity>(e->productId);
    Document doc = AppData->loadEntityById<DocumentEntity>(e->parentDocId);

    if (operatedProduct == nullptr || doc == nullptr)
    {
        emit backRequired();
        return;
    }

    updateProductBanner();
    currentEntry = e;

    int index = findNamedId(e->measure, measures);
    if (index != -1) measureField->setCurrentIndex(index);
    index = findNamedId(e->option1, options);
    if (index != -1) optionField->setCurrentIndex(index);
    index = findNamedId(e->option2, options);
    if (index != -1) currentSOptionIndex = index;
    index = findNamedId(e->option3, options);
    if (index != -1) currentTOptionIndex = index;

    quantityStepper->setValue(e->quantity);
    commentField->setText(e->comment);
}

// ==================== Show ====================

void EntryCreationScreen::show()
{
    quantityStepper->spinBox()->setFocus();
    inframedWidget::show();
}

// ==================== Confirmed ====================

void EntryCreationScreen::confirmed()
{
    currentEntry->comment = commentField->text();
    currentEntry->quantity = quantityStepper->value();

    // Measure
    int index = findNamedId(measureField->currentText(), measures);
    if (index != -1)
        currentEntry->measure = measures.at(index)->id;

    // Options
    index = findNamedId(optionField->currentText(), options);
    if (index != -1)
        currentEntry->option1 = options.at(index)->id;
    if (currentSOptionIndex >= 0 && currentSOptionIndex < options.size())
        currentEntry->option2 = options.at(currentSOptionIndex)->id;
    if (currentTOptionIndex >= 0 && currentTOptionIndex < options.size())
        currentEntry->option3 = options.at(currentTOptionIndex)->id;

    // Reset UI
    quantityStepper->setValue(0);
    commentField->clear();

    emit entryCreated(currentEntry);
}
