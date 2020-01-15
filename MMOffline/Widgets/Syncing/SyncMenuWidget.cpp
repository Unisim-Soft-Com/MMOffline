#include "SyncMenuWidget.h"
#include "Widgets/utils/GlobalAppSettings.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
#include "Networking/Parsers/RequestParser.h"
#include <QMessageBox>
#ifdef DEBUG
#include "debugtrace.h"
#endif

using namespace RequestParser;
const QString dateSendFormat = QStringLiteral("dd_MM_yyyy");
const QString dateTimeSendFormat = QStringLiteral("dd_MM_yyyy_HH_mm_ss");
void SyncMenuWidget::_assertEnd()
{
	if (isFullSync)
	{
		switch (synchrostep)
		{
		case SyncInfoWidget::uploadEnd:
			synchrostep = 0;
			receiveData();
			return;
		case SyncInfoWidget::downloadEnd:
			synchrostep = 0;
			isFullSync = false;
			return;
		}
	}
	else
	{
		synchrostep = 0;
	}
}
int SyncMenuWidget::_count_upload_progress()
{
	if (docsToUpload.isEmpty())
		return 100;
	return (int)((float)currentDoc + 1.0 / ((float)docsToUpload.count() + 1.0 / 100.0));
}

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
	case SyncInfoWidget::PostingDocument:
	{
		QStringList buffer;
		Document d = docsToUpload.at(currentDoc);
		buffer << QString::number(d->documentId) << d->dateWhenCreated.toString(dateTimeSendFormat)
			<< d->shippingDate.toString(dateSendFormat) << QString::number(d->clientId)
			<< QString::number(d->warehouseId) << QString::number(d->documentType) << QString::number(d->alreadyPaid);
		AppWorkset->networkingEngine->execQueryByTemplate(PostDocument, 7, buffer, awaiter);
		return false;
	}
	case SyncInfoWidget::PostingEntry:
	{
		QStringList buffer;
		DocumentEntry e = docsToUpload.at(currentDoc)->linkedEntries.at(currentEntry);
		buffer << QString::number(e->parentDocId) << QString::number(e->entryId)
			<< QString::number(e->productId) << QString::number(e->price)
			<< QString::number(e->measure) << QString::number(e->quantity) <<
			QString::number(e->option1) << QString::number(e->option2) <<
			QString::number(e->option3) << e->comment;
		AppWorkset->networkingEngine->execQueryByTemplate(PostEntry, 10, buffer, awaiter);
		return false;
	}

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
	case SyncInfoWidget::PostingDocument:
		if (awaiter->restext.toLongLong() == docsToUpload.at(currentDoc)->documentId)
		{
			syncInfo->setProgress(_count_upload_progress());
			return true;
		}
		return false;
	case SyncInfoWidget::PostingEntry:
		if (awaiter->restext.toLongLong() == docsToUpload.at(currentDoc)->linkedEntries.at(currentEntry)->entryId)
			return true;
		return false;
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
		resp = parseAndInterpretListAs<ProductEntity>(awaiter->restext, awaiter->errtext);
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
	case SyncInfoWidget::PostingDocument:
	{
		if (docsToUpload.count() > currentDoc)
		{
			currentEntry = 0;
			synchrostep = SyncInfoWidget::PostingEntry;
			return true;
		}
		return false;
	}
	case SyncInfoWidget::PostingEntry:
	{
		++currentEntry;
		if (docsToUpload.at(currentDoc)->linkedEntries.count() > currentEntry)
		{
			return true;
		}
		else
		{
			synchrostep = SyncInfoWidget::PostingDocument;
			++currentDoc;
			if (docsToUpload.count() <= currentDoc)
			{
				synchrostep = SyncInfoWidget::uploadEnd;
				_upload_complete();
				return false;
			}
			return true;
		}
	}
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
	_assertEnd();
}
void SyncMenuWidget::_upload_complete()
{
	if (synchrostep != SyncInfoWidget::uploadEnd)
		return;
	syncInfo->setProgress(99);
	AppWorkset->dataprovider.recreateTable<DocumentEntity>();
	AppWorkset->dataprovider.recreateTable<DocumentEntryEntity>();
	AppSettings->lastSyncDate = QDate::currentDate();
	syncInfo->setInfopack(AppSettings->localLogin, 0);
	syncInfo->setProgress(SyncInfoWidget::uploadEnd);
	_assertEnd();
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
	synchrostep(0), entriesFrom(0), entriesTo(100), dClients(),
	currentClientId(0), repeatingQuery(false), docsToUpload(), currentDoc(0), currentEntry(0), isFullSync(false)
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

	receiveButton->setIcon(QIcon(":/res/netDownload.png"));
	receiveButton->setStyleSheet(DOWNLOAD_BUTTON_STYLESHEET);

	sendButton->setIcon(QIcon(":/res/netUpload.png"));
	sendButton->setStyleSheet(UPLOAD_BUTTON_STYLESHEET);

	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	backButton->setIcon(QIcon(":/res/back.png"));

	QObject::connect(syncButton, &MegaIconButton::clicked, this, &SyncMenuWidget::makeFullSync);
	QObject::connect(receiveButton, &MegaIconButton::clicked, this, &SyncMenuWidget::receiveData);
	QObject::connect(sendButton, &MegaIconButton::clicked, this, &SyncMenuWidget::sendData);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &SyncMenuWidget::backRequired);
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &SyncMenuWidget::operate_on_response);
}

void SyncMenuWidget::setLogin(QString& nlogin)
{
	syncInfo->setInfopack(nlogin,
		QString::number(AppWorkset->dataprovider.countData(predefinedDBNames[Documents])));
}

void SyncMenuWidget::fillTexts()
{
	syncButton->setText(tr("Synchronize"));
	backButton->setText(tr("back"));
	receiveButton->setText(tr("Get data"));
	sendButton->setText(tr("Send data"));
	syncInfo->fillTexts();
}

void SyncMenuWidget::sendData()
{
	if (synchrostep != 0)
		return;
	synchrostep = SyncInfoWidget::uploadStart;
	syncInfo->setProgress(synchrostep);
	docsToUpload = AppWorkset->dataprovider.loadEntities<DocumentEntity>();
	if (docsToUpload.isEmpty())
	{
		syncInfo->setProgress(100);
		synchrostep = 0;
		if (isFullSync)
			receiveData();
		return;
	}
	DocEntryList entries;
	for (Document d : docsToUpload)
	{
		entries = AppWorkset->dataprovider.loadEntities<DocumentEntryEntity>("parentDocId = " + QString::number(d->documentId));
		for (DocumentEntry e : entries)
		{
			d->linkEntry(e);
		}
	}
	synchrostep = SyncInfoWidget::PostingDocument;
	currentDoc = 0;
	currentEntry = 0;
	_send_data_request();
}

void SyncMenuWidget::receiveData()
{
	if (synchrostep != 0 && synchrostep != SyncInfoWidget::downloadStart)
	{
		return;
	}
	if (!isFullSync)
	{
		int res = QMessageBox::warning(this, tr("Warning"), tr("Receiving data warning"), QMessageBox::Ok, QMessageBox::Cancel);
		if (res == QMessageBox::Cancel)
		{
			return;
		}
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
	isFullSync = true;
	sendData();
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
		if (synchrostep < SyncInfoWidget::downloadStart)
		{
			_send_data_request();
			return;
		}
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