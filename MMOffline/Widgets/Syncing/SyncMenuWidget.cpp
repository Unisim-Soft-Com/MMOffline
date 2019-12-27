#include "SyncMenuWidget.h"
#include "Widgets/utils/GlobalAppSettings.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
#include "Networking/Parsers/RequestParser.h"

#ifdef DEBUG
#include "debugtrace.h"
#endif

using namespace RequestParser;

bool SyncMenuWidget::_send_data_request()
{

#ifdef DEBUG
	detrace_METHCALL("_send_data_request");
#endif
	switch (synchrostep)
	{
	case -1:
		synchrostep = 0;
		return false;
	case SyncInfoWidget::GettingClients:
			AppWorkset->networkingEngine->execQueryByTemplate(
				GetClients,
				"0",
				"100",
				"",
			awaiter
			);
			return false;
	case SyncInfoWidget::GettingGroups:
		AppWorkset->networkingEngine->execQueryByTemplate(
			GetGroups,
			awaiter
		);
		return false;
	case SyncInfoWidget::GettingTips:
		AppWorkset->networkingEngine->execQueryByAutofillTemplate(GetTips, awaiter);
		return false;
	case SyncInfoWidget::GettingDepozits:
		AppWorkset->networkingEngine->execQueryByAutofillTemplate(GetDepozits, awaiter);
		return false;
	case SyncInfoWidget::GettingMeasures:
		AppWorkset->networkingEngine->execQueryByAutofillTemplate(GetMeasures, awaiter);
		return false;
	case SyncInfoWidget::GettingOptions:
		AppWorkset->networkingEngine->execQueryByAutofillTemplate(GetOptions, awaiter);
		return false;
	case SyncInfoWidget::GettingProducts:
		AppWorkset->networkingEngine->execQueryByTemplate(GetProducts, 
				AppWorkset->networkingEngine->getUserID(),
				QString::number(entriesFrom),
				QString::number(entriesTo),
			awaiter);
		return true;
	}
	return false;
}

bool SyncMenuWidget::_process_clients_response()
{
#ifdef DEBUG
	detrace_METHCALL("_process_clients_response");
#endif
	qApp->processEvents();
	ListedEntitiesResponse resp;
	switch (synchrostep)
	{
	case -1:
		synchrostep = 0;
		break;
	case SyncInfoWidget::GettingClients:
		resp = parseAndInterpretListAs<ClientEntity>(awaiter->restext, awaiter->errtext);
		if (_assertError(resp.isError, resp.error)) return false;
		dClients = upcastEntityVector<ClientEntity>(resp.data);
		currentClientId = 0;
		AppWorkset->dataprovider.pushData(resp.data);
		return true;
	case SyncInfoWidget::GettingMeasures:
	case SyncInfoWidget::GettingDepozits:
	case SyncInfoWidget::GettingOptions:
	case SyncInfoWidget::GettingTips:
		resp = parseAndInterpretListAs<NamedIdEntity>(awaiter->restext, awaiter->errtext);
		if (_assertError(resp.isError, resp.error)) return false;
		AppWorkset->dataprovider.pushData(resp.data, _subdictionaryNameConversion());
		return true;
	case SyncInfoWidget::GettingGroups:
		resp = parseAndInterpretListAs<GroupEntity>(awaiter->restext, awaiter->errtext);
		if (_assertError(resp.isError, resp.error)) return false;
		AppWorkset->dataprovider.pushData(resp.data);
		return true;
	case SyncInfoWidget::GettingProducts:
		resp = parseAndInterpretListAs<ProductEntity>( awaiter->restext, awaiter->errtext);
		if (_assertError(resp.isError, resp.error)) return false;
		AppWorkset->dataprovider.pushData(resp.data);
		entriesFrom += 50;
		entriesTo += 50;
		return !resp.data.isEmpty();
	}
	return false;
}
bool SyncMenuWidget::_next_step()
{
	if (synchrostep == -1)
		return false;
	switch (synchrostep)
	{
	case SyncInfoWidget::downloadStart:
		synchrostep = SyncInfoWidget::GettingClients;
		break;
	case SyncInfoWidget::GettingClients:
		synchrostep = SyncInfoWidget::GettingGroups;
		break;
	case SyncInfoWidget::GettingGroups:
		synchrostep = SyncInfoWidget::GettingTips;
		break;
	case SyncInfoWidget::GettingTips:
	case SyncInfoWidget::GettingDepozits:
	case SyncInfoWidget::GettingMeasures:
		synchrostep += 5;
		break;
	case SyncInfoWidget::GettingOptions:
		synchrostep = SyncInfoWidget::GettingProducts;
		break;
	case SyncInfoWidget::GettingProducts:
		
		synchrostep = SyncInfoWidget::downloadEnd;
		break;
	default:
		return false;
	}
	syncInfo->setProgress(synchrostep);
	if (synchrostep == SyncInfoWidget::downloadEnd)
		_download_complete();
	return true;
}
void SyncMenuWidget::_download_complete()
{
	AppSettings->lastSyncDate = QDate::currentDate();
	AppWorkset->dataprovider.forcedCommit();
	synchrostep = 0;
}
bool SyncMenuWidget::_assertError(bool isErr, QString& errtext)
{
	if (isErr && (!errtext.isEmpty()))
	{
		syncInfo->setErrorLog(errtext);
		synchrostep = -1;
		return true;
	}
	return false;
}
QString SyncMenuWidget::_subdictionaryNameConversion()
{
	switch (synchrostep)
	{
	case SyncInfoWidget::GettingTips:
		return QStringLiteral("Tips");
	case SyncInfoWidget::GettingDepozits:
		return QStringLiteral("Depozits");
	case SyncInfoWidget::GettingMeasures:
		return QStringLiteral("Measures");
	case SyncInfoWidget::GettingOptions:
		return QStringLiteral("Options");
	default:
		return QString();
	}

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
	currentClientId = -1;
	entriesFrom = 0;
	entriesTo = 50;
	AppWorkset->dataprovider.dropEverything();
	AppWorkset->dataprovider.recreateTable<DocumentEntity>();
	AppWorkset->dataprovider.recreateTable<DocumentEntryEntity>();
	AppWorkset->dataprovider.forcedCommit();
	_next_step();
	repeatingQuery = _send_data_request();
}

void SyncMenuWidget::makeFullSync()
{
	if (synchrostep != 0)
		return;
	sendData();
	synchrostep = SyncInfoWidget::downloadStart;
	receiveData();
}

void SyncMenuWidget::operate_on_response()
{
	synchrostep;
	if (_process_clients_response())
	{
		qApp->processEvents();
		if (repeatingQuery)
		{
			repeatingQuery = _send_data_request();
			return;
		}
		else
		{
			if (!_next_step())
				return;
			else
				repeatingQuery = _send_data_request();
		}
	}
	else
	{
		qApp->processEvents();
		if (!_next_step())
			return;
		else
			repeatingQuery = _send_data_request();
	}
}

void SyncMenuWidget::was_timeout()
{
	synchrostep = 0;
	syncInfo->setErrorLog(tr("Timeout: ") + QString::number(AppSettings->timeoutint));
}
