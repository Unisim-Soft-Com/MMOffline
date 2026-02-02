#include "StatusDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QStyle>
#include <QApplication>
#include <QPainterPath>
#include <QEventLoop>
#include <QScreen>
#include <QTimer>

StatusDialog::StatusDialog(const QString& message, QWidget* parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::CustomizeWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setModal(true);

    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::transparent);
    setPalette(pal);

    buildUi(message);
    applyStyle();

    adjustSize();
    QTimer::singleShot(0, this, [this]{
        const QRect ar = QGuiApplication::primaryScreen()->availableGeometry();
        move(ar.center() - QPoint(width()/2, height()/2));
    });
}

void StatusDialog::setTitle(const QString& t)      { if (titleLbl)  titleLbl->setText(t); }
void StatusDialog::setOkText(const QString& t)     { if (okBtn)     okBtn->setText(t); }
void StatusDialog::setCancelText(const QString& t) { if (cancelBtn) cancelBtn->setText(t); }
void StatusDialog::setIconText(const QString& t)   { if (iconLbl)   iconLbl->setText(t); }

void StatusDialog::setDangerMode(bool on)
{
    if (!okBtn) return;
    okBtn->setProperty("danger", on);
    okBtn->style()->unpolish(okBtn);
    okBtn->style()->polish(okBtn);
    okBtn->update();
}

void StatusDialog::setWarningMode(bool on)
{
    if (!iconLbl || !cancelBtn || !okBtn) return;

    if (on) {
        iconLbl->setText(QString::fromUtf8("⚠"));
        iconLbl->setStyleSheet("background: #F59E0B; color: #FFFFFF; font-size: 22pt; font-weight: 900; border-radius: 12px;");
        cancelBtn->hide();
        okBtn->setText(tr("OK"));
    }
}

void StatusDialog::setSuccessMode(bool on)
{
    if (!iconLbl || !cancelBtn || !okBtn) return;

    if (on) {
        iconLbl->setText(QString::fromUtf8("✓"));
        iconLbl->setStyleSheet("background: #1FB455; color: #FFFFFF; font-size: 22pt; font-weight: 900; border-radius: 12px;");
        cancelBtn->hide();
        okBtn->setText(tr("OK"));
    }
}

void StatusDialog::hideCancelButton()
{
    if (cancelBtn) cancelBtn->hide();
}

void StatusDialog::buildUi(const QString& message)
{
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // CARD
    card = new QFrame(this);
    card->setObjectName("card");
    card->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    auto* cardLay = new QVBoxLayout(card);
    cardLay->setContentsMargins(16, 16, 16, 16);
    cardLay->setSpacing(12);

    QSize scr = QGuiApplication::primaryScreen()->availableGeometry().size();
    int shorterSide = qMin(scr.width(), scr.height());
    int cardW = qRound(shorterSide * 0.85);
    card->setMinimumWidth(cardW);
    card->setMaximumWidth(cardW);

#ifndef Q_OS_ANDROID
    auto* shadow = new QGraphicsDropShadowEffect(card);
    shadow->setBlurRadius(28);
    shadow->setOffset(0, 10);
    shadow->setColor(QColor(0, 0, 0, 140));
    card->setGraphicsEffect(shadow);
#endif

    auto* header = new QHBoxLayout();
    header->setSpacing(12);

    iconLbl = new QLabel(card);
    iconLbl->setObjectName("icon");
    iconLbl->setText(QString::fromUtf8("!"));
    iconLbl->setAlignment(Qt::AlignCenter);
    iconLbl->setFixedSize(48, 48);

    titleLbl = new QLabel(tr("Status"), card);
    titleLbl->setObjectName("title");
    titleLbl->setWordWrap(false);

    header->addWidget(iconLbl);
    header->addWidget(titleLbl, 1, Qt::AlignVCenter);

    msgLbl = new QLabel(message, card);
    msgLbl->setObjectName("message");
    msgLbl->setWordWrap(true);

    auto* btnRow = new QHBoxLayout();
    btnRow->setSpacing(10);

    cancelBtn = new QPushButton(tr("Cancel"), card);
    cancelBtn->setObjectName("cancelBtn");
    okBtn = new QPushButton(tr("OK"), card);
    okBtn->setObjectName("okBtn");

    btnRow->addStretch(1);
    btnRow->addWidget(cancelBtn);
    btnRow->addWidget(okBtn);

    cardLay->addLayout(header);
    cardLay->addWidget(msgLbl);
    cardLay->addLayout(btnRow);

    root->addWidget(card, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    root->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
}

void StatusDialog::applyStyle()
{
    setStyleSheet(R"(
        QDialog { background: transparent; }
        #card {
            background: #E8E8E8;
            border-radius: 16px;
            border: 1px solid #B0B0B0;
        }
        #icon {
            background: #EF4444;
            color: #FFFFFF;
            font-size: 16pt;
            font-weight: 900;
            border-radius: 10px;
        }
        #title {
            color: #0F2D5C;
            font-size: 18pt;
            font-weight: 800;
        }
        #message {
            color: #1F2A44;
            font-size: 14pt;
        }
        QPushButton {
            min-height: 44px;
            padding: 0 16px;
            border-radius: 12px;
            font-size: 14pt;
            font-weight: 700;
        }
        #cancelBtn {
            color: #0F2D5C;
            background: #FFFFFF;
            border: 1px solid #D0D0D0;
        }
        #cancelBtn:pressed { background: #E0E0E0; }
        #okBtn {
            color: #FFFFFF;
            background: #1FB455;
            border: 1px solid #16A34A;
            min-width: 40px;
            min-height: 50px;
        }
        #okBtn:pressed { background: #169544; }
        QPushButton[danger="true"] {
            color: #FFFFFF;
            background: #EF4444;
            border: 1px solid #DC2626;
        }
        QPushButton[danger="true"]:pressed { background: #DC2626; }
    )");
}

void StatusDialog::resizeEvent(QResizeEvent* e)
{
    QDialog::resizeEvent(e);
    if (backdrop) backdrop->setGeometry(rect());
}

int StatusDialog::smartExec(QWidget* host)
{
#ifndef Q_OS_ANDROID
    return exec();
#else
    if (!host) host = parentWidget();
    QWidget* topLevel = host->window();
    setParent(topLevel);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Widget);
    setWindowModality(Qt::NonModal);
    setAttribute(Qt::WA_TranslucentBackground, true);

    setGeometry(topLevel->rect());
    show();
    raise();
    activateWindow();

    QEventLoop loop;
    int rc = QDialog::Rejected;
    connect(cancelBtn, &QPushButton::clicked, this, [&]{ rc = QDialog::Rejected; close(); });
    connect(okBtn, &QPushButton::clicked, this, [&]{ rc = QDialog::Accepted; close(); });
    connect(this, &QDialog::finished, &loop, &QEventLoop::quit);
    loop.exec();
    return rc;
#endif
}
