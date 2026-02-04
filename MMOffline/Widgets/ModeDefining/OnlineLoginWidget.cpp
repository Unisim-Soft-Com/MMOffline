#include "OnlineLoginWidget.h"
#include "Widgets/utils/GlobalAppSettings.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Widgets/utils/SvgHelper.h"
#include "Widgets/ElementWidgets/StatusDialog.h"
#include "Networking/dataupdateengine-http.h"
#include "Networking/Parsers/RequestParser.h"
#include <QGraphicsDropShadowEffect>
#include <QTimer>
#include <QApplication>
#include <QScroller>
#include <QScrollerProperties>
#include <QDebug>
#include <QInputMethod>
#include <QGuiApplication>

OnlineLoginWidget::OnlineLoginWidget(QWidget* parent)
    : inframedWidget(parent), abstractNode(), passwordVisible(false)
{
    // Main layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);

    // Setup UI sections
    setupHeader();
    setupContent();

    setupFooter();

    // Apply styles
    applyStyles();

    // Setup abstract node interfaces
    main = this;
    current = contentWidget;
    untouchable = contentWidget;

    // Setup sync menu (hidden subbranch)
    syncMenu = new SyncMenuWidget(this);
    mainLayout->addWidget(syncMenu);
    syncMenu->hide();

    // Setup network
    awaiter = new RequestAwaiter(AppSettings->timeoutint, this);

    // Fill texts
    fillTexts();

    // Set initial login value
    loginField->setText(GlobalAppSettings::instance()->localLogin);

    // Connect signals
    QObject::connect(loginField, &QLineEdit::returnPressed, passwordField, QOverload<>::of(&QLineEdit::setFocus));
    QObject::connect(passwordField, &QLineEdit::returnPressed, this, &OnlineLoginWidget::passwordConfirmed);
    QObject::connect(okButton, &FooterButton::clicked, this, &OnlineLoginWidget::passwordConfirmed);
    QObject::connect(backButton, &FooterButton::clicked, this, &OnlineLoginWidget::backRequired);
    QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &OnlineLoginWidget::processResponse);
    QObject::connect(awaiter, &RequestAwaiter::requestTimeout, this, &OnlineLoginWidget::was_timeout);
    QObject::connect(syncMenu, &SyncMenuWidget::backRequired, this, &OnlineLoginWidget::hideCurrent);
    QObject::connect(togglePasswordBtn, &QPushButton::clicked, this, &OnlineLoginWidget::togglePasswordVisibility);


}

void OnlineLoginWidget::setupHeader()
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

void OnlineLoginWidget::setupContent()
{
    // Scroll area pentru când apare tastatura
    scrollArea = new QScrollArea(this);
    scrollArea->setObjectName("scrollArea");
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setFocusPolicy(Qt::NoFocus);

#ifdef Q_OS_ANDROID
    // IMPORTANT: Pe Android, dezactivăm touch events pe viewport
    // pentru a permite focus imediat pe input fields la primul tap
    scrollArea->viewport()->setAttribute(Qt::WA_AcceptTouchEvents, false);
#else
    // Pe desktop, folosim QScroller pentru scroll cu mouse
    QScroller::grabGesture(scrollArea->viewport(), QScroller::LeftMouseButtonGesture);

    auto scroller = QScroller::scroller(scrollArea->viewport());
    QScrollerProperties props = scroller->scrollerProperties();
    props.setScrollMetric(QScrollerProperties::DragVelocitySmoothingFactor, 0.6);
    props.setScrollMetric(QScrollerProperties::DecelerationFactor, 0.05);
    scroller->setScrollerProperties(props);
#endif

    contentWidget = new inframedWidget();
    contentWidget->setObjectName("contentWidget");
    contentWidget->setFocusPolicy(Qt::ClickFocus);

    contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(20, 30, 20, 20);
    contentLayout->setSpacing(20);

    setupLogo();
    setupLoginCard();

    contentLayout->addStretch();

    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea, 1);
}

void OnlineLoginWidget::setupLogo()
{
    // Logo container
    logoContainer = new QWidget(contentWidget);
    QVBoxLayout* logoLayout = new QVBoxLayout(logoContainer);
    logoLayout->setAlignment(Qt::AlignCenter);
    logoLayout->setSpacing(8);

    // Logo frame (the "U" icon) - dimensiune mai mică
    logoFrame = new QFrame(logoContainer);
    logoFrame->setObjectName("logoFrame");
    logoFrame->setFixedSize(60, 60);

    QVBoxLayout* logoFrameLayout = new QVBoxLayout(logoFrame);
    logoFrameLayout->setContentsMargins(0, 0, 0, 0);
    logoFrameLayout->setAlignment(Qt::AlignCenter);

    // CREAT AICI - apoi setăm objectName
    logoIcon = new QLabel("U", logoFrame);
    logoIcon->setObjectName("logoIconSmall");  // folosim stilul mic
    logoIcon->setAlignment(Qt::AlignCenter);
    logoFrameLayout->addWidget(logoIcon);

    // App name
    appNameLabel = new QLabel("UNAOrders", logoContainer);
    appNameLabel->setObjectName("appNameLabel");
    appNameLabel->setAlignment(Qt::AlignCenter);

    // Version
    versionLabel = new QLabel(logoContainer);
    versionLabel->setObjectName("versionLabel");
    versionLabel->setAlignment(Qt::AlignCenter);

    logoLayout->addWidget(logoFrame, 0, Qt::AlignCenter);
    logoLayout->addWidget(appNameLabel);
    logoLayout->addWidget(versionLabel);

    contentLayout->addWidget(logoContainer);
}

void OnlineLoginWidget::setupLoginCard()
{
    loginCard = new QFrame(contentWidget);
    loginCard->setObjectName("loginCard");
    loginCard->setFocusPolicy(Qt::ClickFocus);

    cardLayout = new QVBoxLayout(loginCard);
    cardLayout->setContentsMargins(24, 24, 24, 24);
    cardLayout->setSpacing(16);

    // Shadow effect
#ifndef Q_OS_ANDROID
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(loginCard);
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 25));
    loginCard->setGraphicsEffect(shadow);
#endif

    // === Username field ===
    loginLabel = new QLabel(loginCard);
    loginLabel->setObjectName("inputLabel");
    cardLayout->addWidget(loginLabel);

    loginInputFrame = new QFrame(loginCard);
    loginInputFrame->setObjectName("inputFrame");
    loginInputFrame->setMinimumHeight(56);

    loginInputLayout = new QHBoxLayout(loginInputFrame);
    loginInputLayout->setContentsMargins(14, 0, 14, 0);
    loginInputLayout->setSpacing(10);

    loginIconLabel = new QLabel(loginInputFrame);
    loginIconLabel->setFixedSize(20, 20);
    loginIconLabel->setPixmap(SvgHelper::iconFromSvg(SvgHelper::SVG_USER, SvgHelper::Colors::ICON_GRAY, QSize(20, 20)).pixmap(20, 20));

    loginField = new QLineEdit(loginInputFrame);
    loginField->setObjectName("inputField");
    loginField->setFrame(false);
    loginField->setFocusPolicy(Qt::StrongFocus);

    loginInputLayout->addWidget(loginIconLabel);
    loginInputLayout->addWidget(loginField, 1);

    cardLayout->addWidget(loginInputFrame);

    // === Password field ===
    passwordLabel = new QLabel(loginCard);
    passwordLabel->setObjectName("inputLabel");
    cardLayout->addWidget(passwordLabel);

    passwordInputFrame = new QFrame(loginCard);
    passwordInputFrame->setObjectName("inputFrame");
    passwordInputFrame->setMinimumHeight(56);

    passwordInputLayout = new QHBoxLayout(passwordInputFrame);
    passwordInputLayout->setContentsMargins(14, 0, 14, 0);
    passwordInputLayout->setSpacing(10);

    passwordIconLabel = new QLabel(passwordInputFrame);
    passwordIconLabel->setFixedSize(20, 20);
    passwordIconLabel->setPixmap(SvgHelper::iconFromSvg(SvgHelper::SVG_LOCK, SvgHelper::Colors::ICON_GRAY, QSize(20, 20)).pixmap(20, 20));

    passwordField = new QLineEdit(passwordInputFrame);
    passwordField->setObjectName("inputField");
    passwordField->setFrame(false);
    passwordField->setEchoMode(QLineEdit::Password);
    passwordField->setFocusPolicy(Qt::StrongFocus);

    connect(qApp, &QApplication::focusChanged, this, [this](QWidget* old, QWidget* now) {
        // Ignoră dacă widget-ul tocmai s-a deschis (old e nullptr sau nu e din acest widget)
        if (old == nullptr && !this->isActiveWindow()) {
            return;
        }

        if (now == passwordField || now == loginField) {
            logoContainer->hide();

#ifdef Q_OS_ANDROID
            // IMPORTANT: Forțăm afișarea tastaturii pe Android
            QInputMethod* inputMethod = QGuiApplication::inputMethod();
            if (inputMethod) {
                inputMethod->show();
            }
#endif

            QTimer::singleShot(100, this, [this, now]() {
                if (now == passwordField)
                    scrollArea->ensureWidgetVisible(passwordInputFrame, 0, 20);
                else if (now == loginField)
                    scrollArea->ensureWidgetVisible(loginInputFrame, 0, 20);
            });
        } else if (old == passwordField || old == loginField) {
            // Arată logo-ul doar când ieșim din câmpurile noastre
            logoContainer->show();

#ifdef Q_OS_ANDROID
            // Ascundem tastatura când ieșim din câmpurile de input
            QInputMethod* inputMethod = QGuiApplication::inputMethod();
            if (inputMethod) {
                inputMethod->hide();
            }
#endif
        }
    });

    togglePasswordBtn = new QPushButton(passwordInputFrame);
    togglePasswordBtn->setObjectName("togglePasswordBtn");
    togglePasswordBtn->setFixedSize(32, 32);
    togglePasswordBtn->setCursor(Qt::PointingHandCursor);
    togglePasswordBtn->setFlat(true);
    updatePasswordToggleIcon();

    passwordInputLayout->addWidget(passwordIconLabel);
    passwordInputLayout->addWidget(passwordField, 1);
    passwordInputLayout->addWidget(togglePasswordBtn);

    cardLayout->addWidget(passwordInputFrame);

    contentLayout->addWidget(loginCard);
}

void OnlineLoginWidget::setupFooter()
{
    footerWidget = new QWidget(this);
    footerWidget->setObjectName("footerWidget");

    footerLayout = new QHBoxLayout(footerWidget);
    footerLayout->setContentsMargins(12, 12, 12, 12);
    footerLayout->setSpacing(12);

    // Back button
    backButton = new FooterButton(FooterButton::Secondary, footerWidget);
    backButton->setIcon(SvgHelper::iconFromSvg(SvgHelper::SVG_ARROW_LEFT, SvgHelper::Colors::ICON_BLUE, QSize(28, 28)));

    // OK button
    okButton = new FooterButton(FooterButton::Success, footerWidget);
    okButton->setIcon(SvgHelper::iconFromSvg(SvgHelper::SVG_CHECK, SvgHelper::Colors::ICON_WHITE, QSize(28, 28)));

    footerLayout->addWidget(backButton);
    footerLayout->addWidget(okButton);

    mainLayout->addWidget(footerWidget);
}

void OnlineLoginWidget::applyStyles()
{
    setStyleSheet(R"(
        /* Main background */
        OnlineLoginWidget {
            background-color: #F3F6FB;
        }

        /* Header */
        #headerFrame {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #1a3a5c, stop:1 #2c5282);
        }
        #headerTitle {
            color: #FFFFFF;
            font-size: 20px;
            font-weight: 600;
        }

        /* Content area */
        #contentWidget {
            background-color: #F3F6FB;
        }

        /* Logo */
        #logoFrame {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #1a3a5c, stop:1 #2c5282);
            border-radius: 15px;
        }
        #logoIcon {
            color: #FFFFFF;
            font-size: 36px;
            font-weight: bold;
        }
        #logoIconSmall {
            color: #FFFFFF;
            font-size: 28px;
            font-weight: bold;
        }
        #appNameLabel {
            color: #1a3a5c;
            font-size: 18px;
            font-weight: 600;
        }
        #versionLabel {
            color: #64748B;
            font-size: 13px;
        }

        /* Login card */
        #loginCard {
            background-color: #FFFFFF;
            border-radius: 16px;
            border: 1px solid #E2E8F0;
        }

        /* Input labels */
        #inputLabel {
            color: #4A5568;
            font-size: 14px;
            font-weight: 500;
        }

        /* Input frames */
        #inputFrame {
            background-color: #F8FAFC;
            border: 2px solid #E2E8F0;
            border-radius: 12px;
        }
        #inputFrame:focus-within {
            border-color: #3182CE;
            background-color: #FFFFFF;
        }

        /* Input fields */
        #inputField {
            background-color: transparent;
            border: none;
            color: #2D3748;
            font-size: 16px;
            padding: 8px 0px;
        }
        #inputField::placeholder {
            color: #A0AEC0;
        }

        /* Toggle password button */
        #togglePasswordBtn {
            background-color: transparent;
            border: none;
        }

        /* Footer */
        #footerWidget {
            background-color: #FFFFFF;
            border-top: 1px solid #E2E8F0;
        }

        /* Scroll area */
        #scrollArea {
            background-color: #F3F6FB;
            border: none;
        }
    )");
}

void OnlineLoginWidget::show()
{
    inframedWidget::show();
}

void OnlineLoginWidget::fillTexts()
{
    headerTitle->setText(tr("Autentificare"));
    versionLabel->setText(tr("versiunea 0.52 beta"));
    loginLabel->setText(tr("Nume utilizator"));
    loginField->setPlaceholderText(tr("Introduceți numele"));
    passwordLabel->setText(tr("Parolă"));
    passwordField->setPlaceholderText(tr("Introduceți parola"));
    backButton->setText(tr("Înapoi"));
    okButton->setText(tr("Autentificare"));
    syncMenu->fillTexts();
}

void OnlineLoginWidget::togglePasswordVisibility()
{
    passwordVisible = !passwordVisible;
    passwordField->setEchoMode(passwordVisible ? QLineEdit::Normal : QLineEdit::Password);
    updatePasswordToggleIcon();
}

void OnlineLoginWidget::updatePasswordToggleIcon()
{
    QString svgIcon = passwordVisible ? SvgHelper::SVG_EYE_OFF : SvgHelper::SVG_EYE;
    togglePasswordBtn->setIcon(SvgHelper::iconFromSvg(svgIcon, SvgHelper::Colors::ICON_GRAY, QSize(20, 20)));
    togglePasswordBtn->setIconSize(QSize(20, 20));
}

void OnlineLoginWidget::processResponse()
{
    auto resp = RequestParser::getLoginResult(awaiter->restext, awaiter->errtext);
    if (resp.isError)
    {
        StatusDialog dlg(resp.error, this);
        dlg.setTitle(tr("Eroare autentificare"));
        dlg.setDangerMode(true);
        dlg.hideCancelButton();
        dlg.setOkText(tr("OK"));
        dlg.smartExec(this);
    }
    else
    {
        AppNetwork->setSession(resp.session, resp.uid);
        syncMenu->setLogin(pendingLogin, AppSettings->localLogin);
        AppSettings->localLogin = pendingLogin;
        AppSettings->dump();
        emit loginReady(AppSettings->localLogin);
        _hideAny(syncMenu);
        headerFrame->hide();
        footerWidget->hide();
        pendingLogin.clear();
    }
}

void OnlineLoginWidget::was_timeout()
{
    StatusDialog dlg(tr("A expirat timpul de așteptare: ") + QString::number(awaiter->getInterval()) + tr(" secunde"), this);
    dlg.setTitle(tr("Eroare conexiune"));
    dlg.setWarningMode(true);
    dlg.smartExec(this);
}

void OnlineLoginWidget::hideCurrent()
{
    headerFrame->show();
    footerWidget->show();
    _hideCurrent(contentWidget);
    emit backRequired();
}

void OnlineLoginWidget::passwordConfirmed()
{
    if (loginField->text().isEmpty() || passwordField->text().isEmpty())
    {
        StatusDialog dlg(tr("Vă rugăm să introduceți numele de utilizator și parola!"), this);
        dlg.setTitle(tr("Câmpuri incomplete"));
        dlg.setWarningMode(true);
        dlg.smartExec(this);
        return;
    }
    if (awaiter->isAwaiting()) return;
    pendingLogin = loginField->text();
    AppNetwork->initiateSession(loginField->text(), passwordField->text(), awaiter);
}
