#include "SettingsScreen.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Dataprovider/SqliteDataProvider.h"
#include "Dataprovider/FileParser.h"
#include "Networking/dataupdateengine-http.h"
#include "Widgets/ElementWidgets/StatusDialog.h"
#include <QScroller>
#include <QScrollerProperties>

#include <QGraphicsDropShadowEffect>
#include <QScrollArea>

// This template is used during version showing
const QString VERSION_TEMPLATE = QStringLiteral("UNAOrders %1 %2");

QFrame* SettingsScreen::createCard()
{
    QFrame* card = new QFrame(this);
    card->setObjectName("settingsCard");
    card->setAttribute(Qt::WA_StyledBackground, true);

#ifndef Q_OS_ANDROID
    // Shadow doar pe desktop (pe Android cauzează probleme de lățime)
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(card);
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 3);
    shadow->setColor(QColor(0, 0, 0, 40));
    card->setGraphicsEffect(shadow);
#endif

    // Forțăm cardul să nu depășească lățimea părintelui
    card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    return card;
}

void SettingsScreen::applyStyles()
{
    // Main widget background
    this->setStyleSheet(R"CSS(
        SettingsScreen {
            background: #F3F4F6;
        }
    )CSS");

    // Header bar style
    headerBar->setStyleSheet(R"CSS(
        QFrame#settingsHeaderBar {
            background: #0F2D5C;
            border: 0;
        }
    )CSS");

    headerTitle->setStyleSheet(R"CSS(
        QLabel {
            color: white;
            font-weight: 800;
        }
    )CSS");

    // Content area style
    contentWidget->setStyleSheet(R"CSS(
        QWidget#settingsContent {
            background: #F3F4F6;
        }
    )CSS");

    // Cards style
    QString cardStyle = R"CSS(
        QFrame#settingsCard {
            background: #FFFFFF;
            border: 1px solid #E6E6E6;
            border-radius: 16px;
        }
    )CSS";
    cardSync->setStyleSheet(cardStyle);
    cardLocale->setStyleSheet(cardStyle);
    cardDebug->setStyleSheet(cardStyle);

    // Labels style
    QString captionStyle = "QLabel { color: #6B7280; font-weight: 600; }";
    QString valueStyle = "QLabel { color: #0F2D5C; font-weight: 800; }";

    lblSyncCaption->setStyleSheet(captionStyle);
    urlBaseInfo->setStyleSheet(captionStyle);
    lblPathCaption->setStyleSheet(captionStyle);
    lblLangCaption->setStyleSheet(captionStyle);
    timeoutInfo->setStyleSheet(captionStyle);
    langValueLabel->setStyleSheet(valueStyle);
    appInfo->setStyleSheet(valueStyle);

    // Input fields style
    QString inputStyle = R"CSS(
        QLineEdit, QComboBox, QSpinBox {
            background: #FFFFFF;
            color: #111827;
            border: 1px solid #E5E7EB;
            border-radius: 12px;
            padding: 10px 12px;
            min-height: 40px;
        }
        QComboBox::drop-down {
            border: 0;
            padding-right: 10px;
        }
        QComboBox QAbstractItemView {
            background: #FFFFFF;
            color: #111827;
            selection-background-color: #E8F0FE;
        }
    )CSS";
    urlBaseField->setStyleSheet(inputStyle);
    filepathField->setStyleSheet(inputStyle);
   // timeoutField->setStyleSheet(inputStyle);

    // Load from file button style
    makeLoadFromFile->setStyleSheet(R"CSS(
        QFrame#loadFileButton {
            background: #E8F0FE;
            border: 1px solid #BFD2FF;
            border-radius: 12px;
        }
        QFrame#loadFileButton:hover {
            background: #D1E0FF;
        }
    )CSS");

    // Language button (chip) style
    translateButton->setStyleSheet(R"CSS(
        QPushButton {
            background: #FFFFFF;
            color: #111827;
            border: 1px solid #CBD5E1;
            border-radius: 18px;
            padding: 6px 14px;
            font-weight: 700;
            font-size: 24pt;
        }
        QPushButton:hover {
            background: #F3F4F6;
        }
    )CSS");

    // Debug mode button style
    debugMode->setStyleSheet(R"CSS(
        QPushButton {
            background: #F3F4F6;
            color: #374151;
            border: 1px solid #D1D5DB;
            border-radius: 12px;
            padding: 14px 16px;
            font-weight: 700;
        }
        QPushButton:checked {
            background: #DEF7EC;
            color: #03543F;
            border: 2px solid #10B981;
        }
        QPushButton:hover {
            background: #E5E7EB;
        }
    )CSS");

    // Footer bar style
    footerBar->setStyleSheet(R"CSS(
        QFrame#settingsFooterBar {
            background: #F3F4F6;
            border-top: 1px solid #DDDFE1;
        }
    )CSS");

    // Back button style (green outline)
#ifdef Q_OS_ANDROID
    backButton->setStyleSheet(R"CSS(
        QPushButton {
            background: #EAF7EE;
            border: 1px solid #16A34A;
            border-radius: 18px;
            color: #166534;
            font-size: 20pt;
            font-weight: 800;
            padding: 10px 12px;
        }
        QPushButton:pressed {
            background: #C2CCC4;
        }
    )CSS");
#else
    backButton->setStyleSheet(R"CSS(
        QPushButton {
            background: #EAF7EE;
            border: 1px solid #16A34A;
            border-radius: 14px;
            color: #166534;
            font-size: 14pt;
            font-weight: 700;
            padding: 6px 10px;
        }
        QPushButton:pressed {
            background: #C2CCC4;
        }
    )CSS");
#endif

    // Save button style (blue solid)
#ifdef Q_OS_ANDROID
    okButton->setStyleSheet(R"CSS(
        QPushButton {
            background: #2A7BE4;
            color: white;
            border: 1px solid #1C5399;
            border-radius: 18px;
            font-size: 20pt;
            font-weight: 800;
            padding: 10px 12px;
        }
        QPushButton:pressed {
            background: #1F5BB8;
        }
    )CSS");
#else
    okButton->setStyleSheet(R"CSS(
        QPushButton {
            background: #2A7BE4;
            color: white;
            border: 1px solid #1C5399;
            border-radius: 14px;
            font-size: 14pt;
            font-weight: 700;
            padding: 6px 10px;
        }
        QPushButton:pressed {
            background: #1F5BB8;
        }
    )CSS");
#endif
}

void SettingsScreen::fillTexts()
{
    headerTitle->setText(tr("Settings"));
    urlBaseInfo->setText(tr("Address for synchronization"));
    lblPathCaption->setText(tr("File path"));
    loadFileText->setText(tr("Load from file"));
    lblLangCaption->setText(tr("Language"));
    timeoutInfo->setText(tr("Timeout interval"));
    debugMode->setText(QString::fromUtf8("🔧  ") + tr("DEBUG MODE"));

    // Update language display
    langValueLabel->setText(langDisplayNames[currentLanguage]);
    translateButton->setText(langFlags[currentLanguage]);

    // Footer buttons
    backButton->setText(QString::fromUtf8("←  ") + tr("Back"));
    okButton->setText(tr("Save") + QString::fromUtf8("  ✓"));
}

SettingsScreen::SettingsScreen(QWidget* parent)
    : inframedWidget(parent),
    mainLayout(new QVBoxLayout(this)),
    headerBar(new QFrame(this)),
    headerTitle(new QLabel(this)),
    contentWidget(new QWidget(this)),
    contentLayout(new QVBoxLayout(contentWidget)),
    cardSync(nullptr),
    cardLocale(nullptr),
    cardDebug(nullptr),
    lblSyncCaption(new QLabel(this)),
    urlBaseInfo(new QLabel(this)),
    urlBaseField(new QComboBox(this)),
    lblPathCaption(new QLabel(this)),
    filepathField(new QLineEdit(this)),
    makeLoadFromFile(new QFrame(this)),
    loadFileIcon(new QLabel(this)),
    loadFileText(new QLabel(this)),
    lblLangCaption(new QLabel(this)),
    langValueLabel(new QLabel(this)),
    translateButton(new QPushButton(this)),
    timeoutInfo(new QLabel(this)),
    timeoutField(new QSpinBox(this)),
    debugMode(new QPushButton(this)),
    errorLog(new AssertionLabel(this)),
    footerBar(new QFrame(this)),
    footerLayout(new QHBoxLayout()),
    backButton(new QPushButton(this)),
    okButton(new QPushButton(this)),
    appInfo(new QLabel(this)),
    langIcons{ QIcon(":/res/eng.png"), QIcon(":/res/ro.png"), QIcon(":/res/ru.png") },
    currentLanguage(Translations::langFromString(AppSettings->language)),
    langDisplayNames{ "English", "Română", "Русский" },
    langFlags{ QString::fromUtf8("🇬🇧"), QString::fromUtf8("🇷🇴"), QString::fromUtf8("🇷🇺") }
{
    // === MAIN LAYOUT SETUP ===
    this->setLayout(mainLayout);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // === HEADER BAR ===
    headerBar->setObjectName("settingsHeaderBar");
    QGridLayout* headerLayout = new QGridLayout(headerBar);
    headerLayout->setContentsMargins(16, 14, 16, 14);
    headerLayout->setHorizontalSpacing(0);
    headerLayout->setVerticalSpacing(0);

    headerLayout->setColumnStretch(0, 1);
    headerLayout->setColumnStretch(1, 0);
    headerLayout->setColumnStretch(2, 1);


    // Version info (small, left side)
    appInfo->setText(VERSION_TEMPLATE.arg(VERSION).arg(SUFFIX));
    QFont versionFont = appInfo->font();
#ifdef Q_OS_ANDROID
    versionFont.setPointSize(10);
#else
    versionFont.setPointSize(8);
#endif
    appInfo->setFont(versionFont);
    appInfo->setStyleSheet("color: rgba(255,255,255,0.7);");

    // Title (center)
    QFont headerFont = headerTitle->font();
#ifdef Q_OS_ANDROID
    headerFont.setPointSize(24);
#else
    headerFont.setPointSize(16);
#endif
    headerFont.setBold(true);
    headerTitle->setFont(headerFont);
    headerTitle->setAlignment(Qt::AlignCenter);

    headerLayout->addWidget(appInfo, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
    headerLayout->addWidget(headerTitle, 0, 1, Qt::AlignCenter);

    QWidget* rightSpacer = new QWidget(headerBar);
    rightSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    headerLayout->addWidget(rightSpacer, 0, 2);

    mainLayout->addWidget(headerBar);

    // === CONTENT AREA ===
    contentWidget->setObjectName("settingsContent");
    contentLayout->setContentsMargins(8, 12, 8, 12);
    contentLayout->setSpacing(12);

    // --- CARD 1: Sync Settings ---
    cardSync = createCard();
    QVBoxLayout* syncLayout = new QVBoxLayout(cardSync);
    syncLayout->setContentsMargins(14, 12, 14, 12);
    syncLayout->setSpacing(8);

    // Version info at top of card
    QLabel* versionLabel = new QLabel(cardSync);
    versionLabel->setText(VERSION_TEMPLATE.arg(VERSION).arg(SUFFIX));
    versionLabel->setAlignment(Qt::AlignCenter);
    versionLabel->setStyleSheet("color: #0F2D5C; font-size: 16pt; font-weight: 800; padding: 4px 0 12px 0;");
    syncLayout->addWidget(versionLabel);

    // URL field
    syncLayout->addWidget(urlBaseInfo);
    urlBaseField->setEditable(true);
    urlBaseField->setInsertPolicy(QComboBox::InsertAtTop);
    if (!GlobalAppSettings::instance()->HttpUrl.isEmpty())
        urlBaseField->addItem(GlobalAppSettings::instance()->HttpUrl);
    if (GlobalAppSettings::instance()->alternativeUrls.count() > 0)
        urlBaseField->addItems(GlobalAppSettings::instance()->alternativeUrls);
    urlBaseField->setCurrentIndex(0);
    urlBaseField->setMinimumHeight(48);
    urlBaseField->setMinimumWidth(0);
    syncLayout->addWidget(urlBaseField);

    syncLayout->addSpacing(6);

    // File path field
    syncLayout->addWidget(lblPathCaption);
    filepathField->setText(AppSettings->filePath);
    filepathField->setMinimumHeight(48);
    filepathField->setMinimumWidth(0);
    syncLayout->addWidget(filepathField);

    syncLayout->addSpacing(6);

    // Load from file button
    // Load from file button (emoji + text separat)
    makeLoadFromFile->setObjectName("loadFileButton");
    makeLoadFromFile->setMinimumHeight(52);
    makeLoadFromFile->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    makeLoadFromFile->setCursor(Qt::PointingHandCursor);

    QHBoxLayout* loadFileLayout = new QHBoxLayout(makeLoadFromFile);
    loadFileLayout->setContentsMargins(16, 8, 16, 8);
    loadFileLayout->setSpacing(8);

    loadFileIcon->setText(QString::fromUtf8("📂"));
    loadFileIcon->setStyleSheet("font-size: 20pt; background: transparent;");

    loadFileText->setStyleSheet("font-weight: 700; color: #0F2D5C; background: transparent;");

    loadFileLayout->addStretch();
    loadFileLayout->addWidget(loadFileIcon);
    loadFileLayout->addWidget(loadFileText);
    loadFileLayout->addStretch();

    syncLayout->addWidget(makeLoadFromFile);

    contentLayout->addWidget(cardSync);

    // --- CARD 2: Language & Timeout ---
    cardLocale = createCard();
    QVBoxLayout* localeLayout = new QVBoxLayout(cardLocale);
    localeLayout->setContentsMargins(14, 12, 14, 12);
    localeLayout->setSpacing(10);

    // Language row
    localeLayout->addWidget(lblLangCaption);

    QHBoxLayout* langRow = new QHBoxLayout();
    langRow->setContentsMargins(0, 0, 0, 0);
    langValueLabel->setMinimumHeight(40);
    langValueLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    langRow->addWidget(langValueLabel);

    translateButton->setMinimumHeight(48);
    translateButton->setFixedWidth(80);
    langRow->addWidget(translateButton);

    localeLayout->addLayout(langRow);

    localeLayout->addSpacing(8);

    // Timeout row
    localeLayout->addWidget(timeoutInfo);
    timeoutField->setMinimum(0);
    timeoutField->setMaximum(10000000);
    timeoutField->setValue(AppSettings->timeoutint);
    timeoutField->setMinimumHeight(48);
    timeoutField->setMinimumWidth(0);
    localeLayout->addWidget(timeoutField);

    contentLayout->addWidget(cardLocale);

    // --- CARD 3: Debug Mode ---
    cardDebug = createCard();
    QVBoxLayout* debugLayout = new QVBoxLayout(cardDebug);
    debugLayout->setContentsMargins(14, 12, 14, 12);
    debugLayout->setSpacing(8);

    debugMode->setCheckable(true);
    debugMode->setChecked(AppSettings->dbTracing);
    debugMode->setMinimumHeight(56);
    debugLayout->addWidget(debugMode);

    contentLayout->addWidget(cardDebug);

    // Error log (if needed)
    errorLog->hide();

    // Add stretch to push content up
    contentLayout->addStretch(1);

    // Wrap content in scroll area for small screens
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidget(contentWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("QScrollArea { background: #F3F4F6; border: 0; }");

    // Dezactivăm scroll-ul orizontal complet
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Activăm touch scrolling optimizat
    QScroller::grabGesture(scrollArea->viewport(), QScroller::TouchGesture);
    QScrollerProperties props = QScroller::scroller(scrollArea->viewport())->scrollerProperties();
    props.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    props.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    props.setScrollMetric(QScrollerProperties::DragStartDistance, 0.002);
    QScroller::scroller(scrollArea->viewport())->setScrollerProperties(props);

    mainLayout->addWidget(scrollArea, 1);

    // === FOOTER BAR ===
    footerBar->setObjectName("settingsFooterBar");
    footerBar->setLayout(footerLayout);
    footerLayout->setContentsMargins(12, 10, 12, 12);
    footerLayout->setSpacing(12);

    backButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    okButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

#ifdef Q_OS_ANDROID
    backButton->setMinimumHeight(70);
    okButton->setMinimumHeight(70);
#else
    backButton->setMinimumHeight(48);
    okButton->setMinimumHeight(48);
#endif

    footerLayout->addWidget(backButton);
    footerLayout->addWidget(okButton);

    mainLayout->addWidget(footerBar);

    // === APPLY STYLES ===
    applyStyles();

    // === APPLY TRANSLATIONS ===
    fillTexts();

    // === CONNECT SIGNALS ===
    QObject::connect(okButton, &QPushButton::clicked, this, &SettingsScreen::allConfirmed);
    QObject::connect(backButton, &QPushButton::clicked, this, &SettingsScreen::backRequired);
    QObject::connect(urlBaseField, QOverload<const QString&>::of(&QComboBox::activated), this, &SettingsScreen::urlPicked);
    QObject::connect(filepathField, &QLineEdit::returnPressed, this, &SettingsScreen::loadFromFile);
    makeLoadFromFile->installEventFilter(this);
    QObject::connect(translateButton, &QPushButton::clicked, this, &SettingsScreen::translateAll);
}

void SettingsScreen::allConfirmed()
{
    GlobalAppSettings::instance()->HttpUrl = (urlBaseField->currentText().isEmpty()) ? GlobalAppSettings::instance()->HttpUrl : urlBaseField->currentText();
    if (!GlobalAppSettings::instance()->alternativeUrls.contains(GlobalAppSettings::instance()->HttpUrl))
    {
        if (!GlobalAppSettings::instance()->HttpUrl.isEmpty())
            GlobalAppSettings::instance()->alternativeUrls << GlobalAppSettings::instance()->HttpUrl;
        GlobalAppSettings::instance()->dump();
        urlBaseField->addItem(GlobalAppSettings::instance()->HttpUrl);
    }
    AppSettings->filePath = filepathField->text();
    AppSettings->timeoutint = timeoutField->value();
    AppSettings->packetTracing = AppSettings->dbTracing = debugMode->isChecked();
    AppNetwork->setUrl(GlobalAppSettings::instance()->HttpUrl);
    AppSettings->dump();
    emit backRequired();
}

void SettingsScreen::loadFromFile()
{
    if (filepathField->text().isEmpty())
    {
        StatusDialog dlg(tr("Please enter a file path"), this);
        dlg.setTitle(tr("Warning"));
        dlg.setWarningMode(true);
        dlg.smartExec(this);
        return;
    }

    if (filepathField->text() == LOCAL_LOAD_PATH)
    {
        QDir dir(LOCAL_LOAD_PATH);
        if (!dir.exists())
            dir.mkdir(LOCAL_LOAD_PATH);
    }

    CsvFileParser parser(filepathField->text());
    parser.doParsing();

    QStringList errors = parser.errorList();

    if (errors.isEmpty())
    {
        // Succes
        StatusDialog dlg(tr("File loaded successfully"), this);
        dlg.setTitle(tr("Success"));
        dlg.setSuccessMode(true);
        dlg.smartExec(this);
    }
    else
    {
        // Eroare
        StatusDialog dlg(errors.join("\n"), this);
        dlg.setTitle(tr("Error"));
        dlg.setDangerMode(true);
        dlg.hideCancelButton();
        dlg.smartExec(this);
    }
}

void SettingsScreen::urlPicked(QString url)
{
    GlobalAppSettings::instance()->HttpUrl = url;
}


bool SettingsScreen::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == makeLoadFromFile && event->type() == QEvent::MouseButtonRelease) {
        loadFromFile();
        return true;
    }
    return inframedWidget::eventFilter(obj, event);
}

void SettingsScreen::translateAll()
{
    using namespace Translations;
    ++currentLanguage;
    AppSettings->setTranslator(currentLanguage);
    fillTexts();
    emit translating(currentLanguage);
}
