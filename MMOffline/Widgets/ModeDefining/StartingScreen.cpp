#include "StartingScreen.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Widgets/utils/SvgHelper.h"
#include "Widgets/utils/GlobalAppSettings.h"
#include <QPainter>
#include <QSizePolicy>
#include <QApplication>

StartingScreen::StartingScreen(QWidget* parent)
    : inframedWidget(parent), abstractNode(),
    mainLayout(new QVBoxLayout(this)),
    innerWidget(new inframedWidget(this)),
    innerLayout(new QVBoxLayout(innerWidget)),
    // Header
    headerLayout(new QHBoxLayout()),
    logoLabel(new QLabel(innerWidget)),
    versionLabel(new QLabel(innerWidget)),
    appNameLabel(new QLabel(innerWidget)),
    // User info
    infoLabel(new QLabel(innerWidget)),
    // Buttons container
    buttonsContainer(new QWidget(innerWidget)),
    buttonsLayout(new QVBoxLayout(buttonsContainer)),
    // Card buttons
    createDocumentButton(new CardMenuButton(innerWidget)),
    onlineLoginButton(new CardMenuButton(innerWidget)),
    logsButton(new CardMenuButton(innerWidget)),
    // Footer
    footerLayout(new QHBoxLayout()),
    quitButton(new FooterButton(FooterButton::Danger, innerWidget)),
    settingsButton(new FooterButton(FooterButton::Primary, innerWidget)),
    // Sub-screens
    onlineLogin(new OnlineLoginWidget(this)),
    settings(new SettingsScreen(this))
{
    // ===================== Main Layout Setup =====================
    this->setLayout(mainLayout);
    mainLayout->addWidget(innerWidget);
    mainLayout->addWidget(onlineLogin);
    mainLayout->addWidget(settings);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ===================== Inner Widget Setup =====================
    innerWidget->setLayout(innerLayout);
    innerLayout->setContentsMargins(0, 0, 0, 0);
    innerLayout->setSpacing(0);

    // Background color for inner widget
    innerWidget->setStyleSheet("background-color: #F3F6FB;");

    // ===================== Header Section =====================
    innerLayout->addLayout(headerLayout);
    headerLayout->setContentsMargins(16, 12, 16, 12);
    headerLayout->setSpacing(8);

    // Logo - UNAOrders icon
    logoLabel->setFixedSize(28, 28);
    logoLabel->setStyleSheet("background: transparent;");
    QPixmap logoPixmap(":/res/UNAOrders.png");
    logoLabel->setPixmap(logoPixmap.scaled(28, 28, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Version label
    QString versionText = QString::number(VERSION, 'f', 2) + " " + SUFFIX;
    versionLabel->setText(versionText);
    versionLabel->setStyleSheet(
        "QLabel {"
        "  color: #0F2D5C;"
        "  font-size: 14px;"
        "  font-weight: 500;"
        "  background: transparent;"
        "}"
        );

    // App name "UNAOrders"
    appNameLabel->setText("UNAOrders");
    appNameLabel->setStyleSheet(
        "QLabel {"
        "  color: #0F2D5C;"
        "  font-size: 14px;"
        "  font-weight: 500;"
        "  background: transparent;"
        "}"
        );

    // Header layout assembly
    headerLayout->addWidget(logoLabel);
    headerLayout->addWidget(versionLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(appNameLabel);

    // ===================== User Info Section =====================
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet(
        "QLabel {"
        "  color: #0F2D5C;"
        "  font-size: 18px;"
        "  font-weight: 600;"
        "  background: transparent;"
        "  padding: 20px 16px;"
        "}"
        );
    innerLayout->addWidget(infoLabel);

    // ===================== Buttons Container =====================
    buttonsContainer->setLayout(buttonsLayout);
    buttonsContainer->setStyleSheet("background: transparent;");
    buttonsLayout->setContentsMargins(16, 8, 16, 8);
    buttonsLayout->setSpacing(12);

    // Add card buttons to container
    buttonsLayout->addWidget(createDocumentButton);
    buttonsLayout->addWidget(onlineLoginButton);
    buttonsLayout->addWidget(logsButton);

    innerLayout->addWidget(buttonsContainer);
    innerLayout->addStretch(); // Push footer to bottom

    // ===================== Footer Section =====================
    innerLayout->addLayout(footerLayout);
    footerLayout->setContentsMargins(16, 12, 16, 24);
    footerLayout->setSpacing(12);

    footerLayout->addWidget(quitButton);
    footerLayout->addWidget(settingsButton);

    // ===================== Abstract Node Setup =====================
    current = innerWidget;
    untouchable = innerWidget;
    main = this;
    settings->hide();
    onlineLogin->hide();

    // ===================== Button Icons (SVG) =====================
    const QSize cardIconSize(40, 40);
    const QSize footerIconSize(28, 28);

    // Card buttons icons
    createDocumentButton->setIcon(
        SvgHelper::iconFromSvg(SvgHelper::SVG_DOCUMENT, SvgHelper::Colors::ICON_BLUE, cardIconSize)
        );
    createDocumentButton->setIconSize(cardIconSize);

    onlineLoginButton->setIcon(
        SvgHelper::iconFromSvg(SvgHelper::SVG_LOGIN, SvgHelper::Colors::ICON_BLUE, cardIconSize)
        );
    onlineLoginButton->setIconSize(cardIconSize);

    logsButton->setIcon(
        SvgHelper::iconFromSvg(SvgHelper::SVG_LOGS, SvgHelper::Colors::ICON_BLUE, cardIconSize)
        );
    logsButton->setIconSize(cardIconSize);

    // Footer buttons icons
    quitButton->setIcon(
        SvgHelper::iconFromSvg(SvgHelper::SVG_EXIT, SvgHelper::Colors::ICON_RED, footerIconSize)
        );
    quitButton->setIconSize(footerIconSize);

    settingsButton->setIcon(
        SvgHelper::iconFromSvg(SvgHelper::SVG_SETTINGS, SvgHelper::Colors::ICON_WHITE, footerIconSize)
        );
    settingsButton->setIconSize(footerIconSize);

    // ===================== Texts =====================
    fillTexts();

    // ===================== Connections =====================
    QObject::connect(createDocumentButton, &CardMenuButton::clicked, this, &StartingScreen::documentCreationInitiated);
    QObject::connect(onlineLoginButton, &CardMenuButton::clicked, this, &StartingScreen::toOnlineLogin);
    QObject::connect(logsButton, &CardMenuButton::clicked, this, &StartingScreen::logsRequired);
    QObject::connect(settingsButton, &FooterButton::clicked, this, &StartingScreen::toSettings);
    QObject::connect(onlineLogin, &OnlineLoginWidget::backRequired, this, &StartingScreen::hideCurrent);
    QObject::connect(settings, &SettingsScreen::backRequired, this, &StartingScreen::hideCurrent);
    QObject::connect(onlineLogin, &OnlineLoginWidget::loginReady, this, &StartingScreen::userLoggedIn);
    QObject::connect(quitButton, &FooterButton::clicked, qApp, QApplication::quit);
    QObject::connect(settings, &SettingsScreen::translating, this, &StartingScreen::translationHappened);
}

void StartingScreen::fillTexts()
{
    createDocumentButton->setText(tr("New document"));
    onlineLoginButton->setText(tr("Login"));
    logsButton->setText(tr("Logs"));
    settingsButton->setText(tr("Settings"));
    quitButton->setText(tr("Quit"));
    setInfoLabel();
}

void StartingScreen::toOnlineLogin()
{
    _hideAny(onlineLogin);
}

void StartingScreen::toSettings()
{
    _hideAny(settings);
}

void StartingScreen::hideCurrent()
{
    if (!_hideCurrent(innerWidget))
        qApp->quit();
}

void StartingScreen::userLoggedIn(QString login)
{
    setInfoLabel();
}

void StartingScreen::translationHappened(int)
{
    fillTexts();
    onlineLogin->fillTexts();
}

void StartingScreen::setInfoLabel()
{
    if (GlobalAppSettings::instance()->localLogin.isEmpty())
    {
        infoLabel->setText(tr("Currently no offline user is registered"));
    }
    else
    {
        infoLabel->setText(tr("Currently used offline user: ") + GlobalAppSettings::instance()->localLogin);
    }
}
