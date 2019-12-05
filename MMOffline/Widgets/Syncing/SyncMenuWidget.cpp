#include "SyncMenuWidget.h"
#include "Widgets/utils/GlobalAppSettings.h"
#include "Widgets/utils/ElementsStyles.h"
SyncMenuWidget::SyncMenuWidget(QWidget* parent)
	:inframedWidget(parent),
	mainLayout(new QVBoxLayout(this)),
	syncInfo(new SyncInfoWidget(this)),
	partialActionsLayout(new QHBoxLayout(this)),
	sendButton(new MegaIconButton(this)),
	receiveButton(new MegaIconButton(this)),
	syncButton(new MegaIconButton(this)),
	backButton(new MegaIconButton(this)),
	awaiter(new RequestAwaiter(AppSettings->timeoutint, this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(syncInfo);
	mainLayout->addLayout(partialActionsLayout);
	mainLayout->addWidget(syncButton);
	mainLayout->addWidget(backButton);
	partialActionsLayout->addWidget(sendButton);
	partialActionsLayout->addWidget(receiveButton);

	syncButton->setIcon(QIcon(":/res/upload.png"));
	syncButton->setText(tr("Synchronize"));

	receiveButton->setIcon(QIcon(":/res/netDownload.png"));
	receiveButton->setText(tr("Get data"));

	sendButton->setIcon(QIcon(":/res/netUpload.png"));
	sendButton->setText(tr("Send data"));

	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setText(tr("back"));

	QObject::connect(syncButton, &MegaIconButton::clicked, this, &SyncMenuWidget::makeFullSync);
	QObject::connect(receiveButton, &MegaIconButton::clicked, this, &SyncMenuWidget::receiveData);
	QObject::connect(sendButton, &MegaIconButton::clicked, this, &SyncMenuWidget::sendData);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &SyncMenuWidget::backRequired);
}

void SyncMenuWidget::setLogin(QString& nlogin)
{
	syncInfo->setInfopack(nlogin, QStringLiteral("0"));
}

void SyncMenuWidget::sendData()
{
}

void SyncMenuWidget::receiveData()
{
}

void SyncMenuWidget::makeFullSync()
{
	sendData();
	receiveData();
}
