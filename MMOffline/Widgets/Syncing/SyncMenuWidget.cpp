#include "SyncMenuWidget.h"
#include "Widgets/utils/GlobalAppSettings.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
#include "Networking/Parsers/RequestParser.h"
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif

using namespace RequestParser;

void SyncMenuWidget::_send_data_request()
{

#ifdef DEBUG
	detrace_METHCALL("_send_data_request");
#endif

	switch (synchrostep)
	{
	case -1:
		synchrostep = 0;
		return;
	case SyncInfoWidget::GettingClients:
			AppWorkset->networkingEngine->execQueryByTemplate(
				GetClients,
				QString::number(entriesFrom),
				QString::number(entriesTo),
				"",
				awaiter
			);
			break;

	}
}

bool SyncMenuWidget::_process_clients_response()
{

#ifdef DEBUG
	detrace_METHCALL("_process_clients_response");
#endif

	ListedEntitiesResponse resp = parseAndInterpretListAs<ClientEntity>(awaiter->restext, awaiter->errtext);
#ifdef DEBUG
	detrace_METHEXPL(" response: " << showLEResponse(&resp));
#endif
	if (resp.isError && (!resp.error.isEmpty()))
	{
		syncInfo->setErrorLog(resp.error);
		synchrostep = -1;
		return false;
	}
	if (resp.data.isEmpty())
	{
		return false;
	}
	AppWorkset->dataprovider.pushData(resp.data);
	entriesFrom += 50;
	entriesTo += 50;
	return true;
}
bool SyncMenuWidget::_next_step()
{
	if (synchrostep == -1)
		return false;
	switch (synchrostep)
	{
	case SyncInfoWidget::downloadStart:
		synchrostep = SyncInfoWidget::GettingClients;
		entriesFrom = 0;
		entriesTo = 50;
		break;
	case SyncInfoWidget::GettingClients:
		synchrostep = SyncInfoWidget::downloadEnd;
	}
	syncInfo->setProgress(synchrostep);
	if (synchrostep == SyncInfoWidget::downloadEnd)
		_download_complete();
	return true;
}
void SyncMenuWidget::_download_complete()
{
	AppSettings->lastSyncDate = QDate::currentDate();
	synchrostep = 0;
}
SyncMenuWidget::SyncMenuWidget(QWidget* parent)
	:inframedWidget(parent),
	mainLayout(new QVBoxLayout(this)),
	syncInfo(new SyncInfoWidget(this)),
	partialActionsLayout(new QHBoxLayout(this)),
	sendButton(new MegaIconButton(this)),
	receiveButton(new MegaIconButton(this)),
	syncButton(new MegaIconButton(this)),
	backButton(new MegaIconButton(this)),
	awaiter(new RequestAwaiter(AppSettings->timeoutint, this)),
	synchrostep(0)
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(syncInfo);
	mainLayout->addLayout(partialActionsLayout);
	mainLayout->addWidget(syncButton);
	mainLayout->addWidget(backButton);
	mainLayout->setContentsMargins(1, 1, 1, 1);
	mainLayout->setSpacing(0);
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
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &SyncMenuWidget::operate_on_response);
}

void SyncMenuWidget::setLogin(QString& nlogin)
{
	syncInfo->setInfopack(nlogin, QStringLiteral("0"));
}

void SyncMenuWidget::sendData()
{
	if (synchrostep != 0)
		return;
	synchrostep = SyncInfoWidget::uploadStart;
	syncInfo->setProgress(synchrostep);
	synchrostep = SyncInfoWidget::uploadEnd;
	syncInfo->setProgress(synchrostep);
}

void SyncMenuWidget::receiveData()
{
	if (synchrostep != 0 && synchrostep != SyncInfoWidget::downloadStart)
	{
		return;
	}
	synchrostep = SyncInfoWidget::downloadStart;
	_next_step();
	_send_data_request();
}

void SyncMenuWidget::makeFullSync()
{
	if (synchrostep != 0)
		return;
	sendData();
	receiveData();
}

void SyncMenuWidget::operate_on_response()
{
#ifdef DEBUG
	detrace_METHCALL("_process_clients_response");
#endif
	switch (synchrostep)
	{
	case SyncInfoWidget::GettingClients:
		if (_process_clients_response())
		{
			_send_data_request();
			return;
		}
		if (!_next_step())
			return;
		break;
	}
}

void SyncMenuWidget::was_timeout()
{
}
