#include "DocumentCreationScreen.h"
#include "Dataprovider/SqliteDataProvider.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Widgets/utils/SvgHelper.h"
#include <QGraphicsDropShadowEffect>
#include <QScroller>
#include <QScrollerProperties>
#include "Widgets/utils/ModernSelectionDialog.h"
#include "Widgets/utils/ElidedComboBox.h"
#include "Widgets/utils/ModernDatePickerDialog.h"
#include <QEvent>

DocumentCreationScreen::DocumentCreationScreen(QWidget* parent)
    : inframedWidget(parent), currentDocument(nullptr)
{
    // Main layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);

    // Build UI sections
    setupHeader();
    setupCustomerBanner();
    setupContent();
    setupFooter();

    // Apply all styles
    applyStyles();

    this->setMaximumWidth(16777215); // QWIDGETSIZE_MAX
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    // Load data from database
    depozits = AppData->loadEntities<NamedIdEntity>(QString::null, "Depozits");
    tips = AppData->loadEntities<NamedIdEntity>(QString::null, "Tips");

    for (NamedId dep : depozits)
        depozitField->addItem(dep->name);

    for (NamedId tip : tips)
        tipField->addItem(tip->name);

    // Fill translatable texts
    fillTexts();

    // Intercept combo popups with modern dialog
    depozitField->installEventFilter(this);
    tipField->installEventFilter(this);

    // Connect signals
    QObject::connect(okButton, &OutlineActionButton::clicked, this, &DocumentCreationScreen::okPressed);
    QObject::connect(backButton, &OutlineActionButton::clicked, this, &DocumentCreationScreen::backRequired);
}

// ==================== Header ====================

void DocumentCreationScreen::setupHeader()
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

// ==================== Customer Banner ====================

void DocumentCreationScreen::setupCustomerBanner()
{
    customerBanner = new QFrame(this);
    customerBanner->setObjectName("customerBanner");
    customerBanner->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

    QHBoxLayout* bannerLayout = new QHBoxLayout(customerBanner);
    bannerLayout->setContentsMargins(12, 8, 12, 8);
    bannerLayout->setSpacing(10);

    // Avatar circle
    customerAvatar = new QLabel(customerBanner);
    customerAvatar->setObjectName("customerAvatar");
    customerAvatar->setFixedSize(38, 38);
    customerAvatar->setAlignment(Qt::AlignCenter);

    // Name + Address column
    QWidget* detailsWidget = new QWidget(customerBanner);
    detailsWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    QVBoxLayout* detailsLayout = new QVBoxLayout(detailsWidget);
    detailsLayout->setContentsMargins(0, 0, 0, 0);
    detailsLayout->setSpacing(2);

    customerName = new QLabel(detailsWidget);
    customerName->setObjectName("customerName");
    customerName->setWordWrap(false);
    customerName->setMinimumWidth(0);
    customerName->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);

    customerAddress = new QLabel(detailsWidget);
    customerAddress->setObjectName("customerAddress");
    customerAddress->setWordWrap(false);
    customerAddress->setMinimumWidth(0);
    customerAddress->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);

    detailsLayout->addWidget(customerName);
    detailsLayout->addWidget(customerAddress);

    // Code badge
    customerCode = new QLabel(customerBanner);
    customerCode->setObjectName("customerCode");
    customerCode->setAlignment(Qt::AlignCenter);
    customerCode->setMinimumWidth(44);

    bannerLayout->addWidget(customerAvatar);
    bannerLayout->addWidget(detailsWidget, 1);
    bannerLayout->addWidget(customerCode);

    mainLayout->addWidget(customerBanner);
}

// ==================== Content ====================

void DocumentCreationScreen::setupContent()
{
    // Scroll area
    scrollArea = new QScrollArea(this);
    scrollArea->setObjectName("scrollArea");
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setFocusPolicy(Qt::NoFocus);

    // Dezactivăm scroll-ul orizontal complet
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

#ifdef Q_OS_ANDROID
    scrollArea->viewport()->setAttribute(Qt::WA_AcceptTouchEvents, true);
#endif

    // Activăm touch scrolling optimizat (ca în SettingsScreen)
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
    setupDeliveryCard();
    contentLayout->addSpacing(20);
    setupPaymentCard();
    contentLayout->addStretch();
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea, 1);
}

void DocumentCreationScreen::setupDeliveryCard()
{
    deliveryCard = new QFrame(contentWidget);
    deliveryCard->setObjectName("formCard");
    deliveryCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

#ifndef Q_OS_ANDROID
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(deliveryCard);
    shadow->setBlurRadius(16);
    shadow->setOffset(0, 3);
    shadow->setColor(QColor(0, 0, 0, 20));
    deliveryCard->setGraphicsEffect(shadow);
#endif

    QVBoxLayout* cardLayout = new QVBoxLayout(deliveryCard);
    cardLayout->setContentsMargins(16, 16, 16, 16);
    cardLayout->setSpacing(14);

    // Card title
    QLabel* cardTitle = new QLabel(deliveryCard);
    cardTitle->setObjectName("cardTitle");
    QIcon packageIcon = SvgHelper::iconFromSvg(SvgHelper::SVG_PACKAGE, SvgHelper::Colors::ICON_BLUE, QSize(18, 18));
    // Title with icon via layout
    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->setSpacing(8);
    QLabel* titleIcon = new QLabel(deliveryCard);
    titleIcon->setPixmap(packageIcon.pixmap(18, 18));
    titleIcon->setFixedSize(18, 18);
    titleLayout->addWidget(titleIcon);
    titleLayout->addWidget(cardTitle, 1);
    cardLayout->addLayout(titleLayout);

    // Date field — clickable, opens ModernDatePickerDialog
    QLabel* dateLabelTitle = new QLabel(deliveryCard);
    dateLabelTitle->setObjectName("fieldLabel");
    dateLabelTitle->setText(tr("Shipping date"));
    cardLayout->addWidget(dateLabelTitle);

    QFrame* dateFrame = new QFrame(deliveryCard);
    dateFrame->setObjectName("inputFrame");
    dateFrame->setMinimumHeight(48);
    dateFrame->setCursor(Qt::PointingHandCursor);
    QHBoxLayout* dateLayout = new QHBoxLayout(dateFrame);
    dateLayout->setContentsMargins(14, 0, 14, 0);
    dateLayout->setSpacing(10);

    QLabel* dateIcon = new QLabel(dateFrame);
    dateIcon->setPixmap(SvgHelper::iconFromSvg(SvgHelper::SVG_CALENDAR,
                                               SvgHelper::Colors::ICON_GRAY, QSize(20, 20)).pixmap(20, 20));
    dateIcon->setFixedSize(20, 20);

    // Left side: date + subtitle
    QWidget* dateTextWidget = new QWidget(dateFrame);
    QVBoxLayout* dateTextLayout = new QVBoxLayout(dateTextWidget);
    dateTextLayout->setContentsMargins(0, 6, 0, 6);
    dateTextLayout->setSpacing(2);

    dateDisplayLabel = new QLabel(dateTextWidget);
    dateDisplayLabel->setStyleSheet(
        "color: #1F2937; font-size: 16px; font-weight: 600;"
        "background: transparent; border: none;"
        );

    dateSubtitleLabel = new QLabel(dateTextWidget);
    dateSubtitleLabel->setStyleSheet(
        "color: #6B7280; font-size: 11px;"
        "background: transparent; border: none;"
        );

    dateTextLayout->addWidget(dateDisplayLabel);
    dateTextLayout->addWidget(dateSubtitleLabel);

    // Right side: badge
    dateBadgeLabel = new QLabel(dateFrame);
    dateBadgeLabel->setAlignment(Qt::AlignCenter);
    dateBadgeLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    dateBadgeLabel->setFixedHeight(22);

    m_currentDate = QDate::currentDate().addDays(1);
    updateDateDisplay();

    dateLayout->addWidget(dateIcon);
    dateLayout->addWidget(dateTextWidget, 1);
    dateLayout->addWidget(dateBadgeLabel);
    cardLayout->addWidget(dateFrame);

    dateFrame->installEventFilter(this);

    // Depozit + Tip in two columns
    QHBoxLayout* twoColLayout = new QHBoxLayout();
    twoColLayout->setSpacing(10);

    // Depozit
    QVBoxLayout* depCol = new QVBoxLayout();
    depCol->setSpacing(6);
    QLabel* depLabel = new QLabel(deliveryCard);
    depLabel->setObjectName("fieldLabel");
    depLabel->setText(tr("Deposit"));
    depozitField = new ElidedComboBox(deliveryCard);
    depozitField->setObjectName("modernCombo");
    depozitField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    depozitField->setMinimumHeight(44);
    depozitField->setMinimumWidth(0);
    depozitField->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    depCol->addWidget(depLabel);
    depCol->addWidget(depozitField);

    // Tip document
    QVBoxLayout* tipCol = new QVBoxLayout();
    tipCol->setSpacing(6);
    QLabel* tipLabel = new QLabel(deliveryCard);
    tipLabel->setObjectName("fieldLabel");
    tipLabel->setText(tr("Document type"));
    tipField = new ElidedComboBox(deliveryCard);
    tipField->setObjectName("modernCombo");
    tipField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    tipField->setMinimumHeight(44);
    tipField->setMinimumWidth(0);
    tipField->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    tipCol->addWidget(tipLabel);
    tipCol->addWidget(tipField);

    twoColLayout->addLayout(depCol, 1);
    twoColLayout->addLayout(tipCol, 1);
    cardLayout->addLayout(twoColLayout);

    contentLayout->addWidget(deliveryCard);
}

void DocumentCreationScreen::setupPaymentCard()
{
    paymentCard = new QFrame(contentWidget);
    paymentCard->setObjectName("formCard");
    paymentCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

#ifndef Q_OS_ANDROID
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(paymentCard);
    shadow->setBlurRadius(16);
    shadow->setOffset(0, 3);
    shadow->setColor(QColor(0, 0, 0, 20));
    paymentCard->setGraphicsEffect(shadow);
#endif

    QVBoxLayout* cardLayout = new QVBoxLayout(paymentCard);
    cardLayout->setContentsMargins(16, 10, 16, 10);
    cardLayout->setSpacing(8);

    // Card title
    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->setSpacing(8);
    // QLabel* titleIcon = new QLabel(paymentCard);
    // titleIcon->setPixmap(SvgHelper::iconFromSvg(SvgHelper::SVG_MONEY,
    //                                             SvgHelper::Colors::ICON_BLUE, QSize(18, 18)).pixmap(18, 18));
    // titleIcon->setFixedSize(18, 18);
    QLabel* cardTitle = new QLabel(paymentCard);
    cardTitle->setObjectName("cardTitle");
    cardTitle->setText(tr("Amount paid"));
    //titleLayout->addWidget(titleIcon);
    titleLayout->addWidget(cardTitle, 1);
    cardLayout->addLayout(titleLayout);

    // Amount stepper
    summPaidField = new ModernAmountStepper(paymentCard);
    summPaidField->setMinimum(0);
    summPaidField->setMaximum(50000);
    summPaidField->setPrecision(2);
    summPaidField->setStep(1.0);
    cardLayout->addWidget(summPaidField);

    contentLayout->addWidget(paymentCard);
}

// ==================== Footer ====================

void DocumentCreationScreen::setupFooter()
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

void DocumentCreationScreen::applyStyles()
{
    setStyleSheet(R"(
        /* Main background */
        DocumentCreationScreen {
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

        /* Customer banner */
        #customerBanner {
            background-color: #EFF6FF;
            border-bottom: 1px solid #DBEAFE;
        }
        #customerAvatar {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #1976D2, stop:1 #1565C0);
            border-radius: 19px;
            color: #FFFFFF;
            font-size: 15px;
            font-weight: 700;
        }
        #customerName {
            color: #1F2937;
            font-size: 14px;
            font-weight: 600;
        }
        #customerAddress {
            color: #6B7280;
            font-size: 11px;
        }
        #customerCode {
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

        /* Input frames */
        #inputFrame {
            background-color: #FAFAFA;
            border: 2px solid #E2E8F0;
            border-radius: 10px;
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

        /* Footer */
        #footerWidget {
            background-color: #FFFFFF;
            border-top: 1px solid #E2E8F0;
        }
    )");
}

// ==================== Date Display ====================

void DocumentCreationScreen::updateDateDisplay()
{
    QString dayName;
    switch (m_currentDate.dayOfWeek()) {
    case 1: dayName = tr("Monday"); break;
    case 2: dayName = tr("Tuesday"); break;
    case 3: dayName = tr("Wednesday"); break;
    case 4: dayName = tr("Thursday"); break;
    case 5: dayName = tr("Friday"); break;
    case 6: dayName = tr("Saturday"); break;
    case 7: dayName = tr("Sunday"); break;
    }
    dateDisplayLabel->setText(m_currentDate.toString("dd.MM.yyyy"));

    int daysUntil = QDate::currentDate().daysTo(m_currentDate);

    if (daysUntil == 1)
        dateSubtitleLabel->setText(dayName + ", " + tr("tomorrow"));
    else if (daysUntil > 1)
        dateSubtitleLabel->setText(dayName + ", " + tr("in %1 days").arg(daysUntil));
    else
        dateSubtitleLabel->setText(dayName);

    // Badge color: yellow if tomorrow, blue otherwise
    if (daysUntil <= 1) {
        dateBadgeLabel->setText(tr("Tomorrow"));
        dateBadgeLabel->setStyleSheet(
            "background: #FEF3C7; color: #92400E;"
            "border-radius: 10px; padding: 2px 10px;"
            "font-size: 11px; font-weight: 600; border: none;"
            );
    } else {
        dateBadgeLabel->setText(tr("%1 days").arg(daysUntil));
        dateBadgeLabel->setStyleSheet(
            "background: #DBEAFE; color: #1E40AF;"
            "border-radius: 10px; padding: 2px 10px;"
            "font-size: 11px; font-weight: 600; border: none;"
            );
    }
}

// ==================== Fill Texts ====================

void DocumentCreationScreen::fillTexts()
{
    headerTitle->setText(tr("Order Details"));
    backButton->setText(tr("Back"));
    okButton->setText(tr("OK"));
}

// ==================== Public Methods ====================

void DocumentCreationScreen::primeCreation(Client client)
{
    if (client == nullptr)
    {
        customerName->setText(tr("No client"));
        customerAddress->setText("");
        customerAvatar->setText("?");
        customerCode->setText("---");
    }
    else
    {
        customerName->setText(client->name);

        // Extract address if available (after '|' in name or from other field)
        // The original showed "Cumparator TEST| or. Chisinau str.N.Dimo 22"
        // If client has address info, show it; otherwise show client name
        customerAddress->setText(tr("Client ID: %1").arg(client->id));

        // Avatar: first letter of client name
        QString firstLetter = client->name.left(1).toUpper();
        customerAvatar->setText(firstLetter);

        // Code badge
        customerCode->setText(QString::number(client->id));

        // Create document
        currentDocument.reset(new DocumentEntity(IdGenerator::generateId()));
        currentDocument->clientId = client->id;
        currentDocument->clientName = client->name;
        currentDocument->dateWhenCreated = QDateTime::currentDateTime();
        m_currentDate = currentDocument->dateWhenCreated.addDays(1).date();
        updateDateDisplay();
    }
}

void DocumentCreationScreen::show()
{
    summPaidField->spinBox()->setFocus();
    inframedWidget::show();
}

void DocumentCreationScreen::okPressed()
{
    if (currentDocument == nullptr)
    {
        customerName->setText(tr("Can not create doc without client"));
    }
    else
    {
        currentDocument->shippingDate = m_currentDate;
        int index = findNamedId(depozitField->currentText(), depozits);
        if (index != -1)
        {
            currentDocument->warehouseId = depozits.at(index)->id;
            currentDocument->warehouseName = depozits.at(index)->name;
        }
        index = findNamedId(tipField->currentText(), tips);
        if (index != -1)
        {
            currentDocument->documentType = tips.at(index)->id;
            currentDocument->documentTypeName = tips.at(index)->name;
        }
        currentDocument->alreadyPaid = summPaidField->value();
        summPaidField->setValue(0);
        emit documentCreated(currentDocument);
    }
}

bool DocumentCreationScreen::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::TouchBegin)
    {
        // Date picker dialog
        QFrame* frame = qobject_cast<QFrame*>(obj);
        if (frame && frame->cursor().shape() == Qt::PointingHandCursor) {
            ModernDatePickerDialog dlg(tr("Select delivery date"), m_currentDate, this);
            dlg.setMinimumDate(QDate::currentDate());
            if (dlg.exec() == QDialog::Accepted) {
                m_currentDate = dlg.selectedDate();
                updateDateDisplay();
            }
            return true;
        }

        QComboBox* combo = qobject_cast<QComboBox*>(obj);
        if (combo && (combo == depozitField || combo == tipField))
        {
            // Build items list
            QStringList items;
            for (int i = 0; i < combo->count(); ++i)
                items << combo->itemText(i);

            // Determine title
            QString title = (combo == depozitField)
                                ? tr("Select Deposit")
                                : tr("Select Document Type");

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
