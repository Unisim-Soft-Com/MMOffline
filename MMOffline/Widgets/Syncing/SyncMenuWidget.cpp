#include "SyncMenuWidget.h"
#include "Widgets/utils/GlobalAppSettings.h"
#include "Widgets/utils/ElementsStyles.h"
#include "Dataprovider/SqliteDataProvider.h"
#include "Networking/dataupdateengine-http.h"
#include "Networking/Parsers/RequestParser.h"
#include <QMessageBox>
#ifdef DEBUG
#include "debugtrace.h"
#endif

using namespace RequestParser;

// this format is used for sending date \ datetimes through url to avoid url corruption
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
			//	after upload must be download
			return;
		case SyncInfoWidget::downloadEnd:
			synchrostep = 0;
			isFullSync = false;
			emit syncCompleted();
			// after download must be emitted signal
			return;
		}
	}
	else
	{
		//normal operation end - unlock buttons
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
	switch (synchrostep)
	{
	case -1:
		//	-1 is error code, it must stop syncing and unlock buttons
		synchrostep = 0;
		return false;
	case SyncInfoWidget::PostingDocument:
		return _post_document();
	case SyncInfoWidget::PostingEntry:
		return _post_entry();
	case SyncInfoWidget::GettingClients:
		AppNetwork->execQueryByTemplate(
			GetClients,
			"0",
			"100",
			"",
			awaiter
		);
		return false;
	case SyncInfoWidget::GettingGroups:
		AppNetwork->execQueryByTemplate(
			GetGroups,
			awaiter
		);
		return false;
	case SyncInfoWidget::GettingTips:
		AppNetwork->execQueryByAutofillTemplate(GetTips, awaiter);
		return false;
	case SyncInfoWidget::GettingDepozits:
		AppNetwork->execQueryByAutofillTemplate(GetDepozits, awaiter);
		return false;
	case SyncInfoWidget::GettingMeasures:
		AppNetwork->execQueryByAutofillTemplate(GetMeasures, awaiter);
		return false;
	case SyncInfoWidget::GettingOptions:
		AppNetwork->execQueryByAutofillTemplate(GetOptions, awaiter);
		return false;
	case SyncInfoWidget::GettingProducts:
		AppNetwork->execQueryByTemplate(GetProducts,
			AppNetwork->getUserID(),
			QString::number(entriesFrom),
			QString::number(entriesTo),
			awaiter);
		return true;
	}
	return false;
}

bool SyncMenuWidget::_process_clients_response()
{
	// refreshing mainloop
	qApp->processEvents();
	ListedEntitiesResponse resp;
	switch (synchrostep)
	{
	case -1:
		// error code, must stop syncing
		synchrostep = 0;
		break;
	case SyncInfoWidget::PostingDocument:
		// checking if result text has returned document id
		if (awaiter->restext.toLongLong() == docsToUpload.at(currentDoc)->documentId)
		{
			// if yes, continue 
			syncInfo->setProgress(_count_upload_progress());
			return true;
		}
		return false;
	case SyncInfoWidget::PostingEntry:
		// checking if result text has returned entry id, if yes - continue
		if (awaiter->restext.toLongLong() == docsToUpload.at(currentDoc)->linkedEntries.at(currentEntry)->entryId)
			return true;
		return false;
	case SyncInfoWidget::GettingClients:
		resp = parseAndInterpretListAs(awaiter->restext, awaiter->errtext, DataEntity(new ClientEntity()));
		if (_assertError(resp.isError, resp.error)) return false;
		dClients = upcastEntityVector<ClientEntity>(resp.data);
		currentClientId = 0;
		AppData->pushData(resp.data);
		return true;
	case SyncInfoWidget::GettingMeasures:
	case SyncInfoWidget::GettingDepozits:
	case SyncInfoWidget::GettingOptions:
	case SyncInfoWidget::GettingTips:
		resp = parseAndInterpretListAs(awaiter->restext, awaiter->errtext, DataEntity(new NamedIdEntity()));
		if (_assertError(resp.isError, resp.error)) return false;
		AppData->pushData(resp.data, _subdictionaryNameConversion());
		return true;
	case SyncInfoWidget::GettingGroups:
		resp = parseAndInterpretListAs(awaiter->restext, awaiter->errtext, DataEntity(new GroupEntity()));
		if (_assertError(resp.isError, resp.error)) return false;
		AppData->pushData(resp.data);
		return true;
	case SyncInfoWidget::GettingProducts:
		resp = parseAndInterpretListAs(awaiter->restext, awaiter->errtext, DataEntity(new ProductEntity()));
		if (_assertError(resp.isError, resp.error)) return false;
		AppData->pushData(resp.data);
		entriesFrom += 50;
		entriesTo += 50;
		return !resp.data.isEmpty();
	}
	return false;
}
bool SyncMenuWidget::_next_step()
{
	switch (synchrostep)
	{
	case -1:
		// error code, must stop synchronization
		synchrostep = 0;
		return false;
	case SyncInfoWidget::PostingDocument:
	{
		if (docsToUpload.count() > currentDoc)
			// if there is more documents to sync - must nullify entry index and continue as PostingEntry
		{
			currentEntry = 0;
			synchrostep = SyncInfoWidget::PostingEntry;
			return true;
		}
		// else - stop
		return false;
	}
	case SyncInfoWidget::PostingEntry:
	{
		++currentEntry;
		if (docsToUpload.at(currentDoc)->linkedEntries.count() > currentEntry)
			// if there is more entries - continue sending
		{
			return true;
		}
		else
			// try to post document
		{
			synchrostep = SyncInfoWidget::PostingDocument;
			++currentDoc;
			if (docsToUpload.count() <= currentDoc)
				// if no documents to post - end upload
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
	AppData->forcedCommit();
	syncInfo->reload();
	_assertEnd();
}
void SyncMenuWidget::_upload_complete()
{
	if (synchrostep != SyncInfoWidget::uploadEnd)
		return;
	syncInfo->setProgress(99);
	AppData->recreateTable(DataEntity(new DocumentEntity()));
	AppData->recreateTable(DataEntity(new DocumentEntryEntity()));
	AppSettings->lastSyncDate = QDate::currentDate();
	syncInfo->setInfopack(AppSettings->localLogin,AppSettings->localLogin, QStringLiteral("0"));
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
bool SyncMenuWidget::_post_document()
{
	QStringList buffer;
	Document d = docsToUpload.at(currentDoc);
	buffer << QString::number(d->documentId) << d->dateWhenCreated.toString(dateTimeSendFormat)
		<< d->shippingDate.toString(dateSendFormat) << QString::number(d->clientId)
		<< QString::number(d->warehouseId) << QString::number(d->documentType) << QString::number(d->alreadyPaid);
	AppNetwork->execQueryByTemplate(PostDocument, 7, buffer, awaiter);
	return false;
}
bool SyncMenuWidget::_post_entry()
{
	QStringList buffer;
	DocumentEntry e = docsToUpload.at(currentDoc)->linkedEntries.at(currentEntry);
	buffer << QString::number(e->parentDocId) << QString::number(e->entryId)
		<< QString::number(e->productId) << QString::number(e->price)
		<< QString::number(e->measure) << QString::number(e->quantity) <<
		QString::number(e->option1) << QString::number(e->option2) <<
		QString::number(e->option3) << e->comment;
	AppNetwork->execQueryByTemplate(PostEntry, 10, buffer, awaiter);
	return false;
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
	previousLogin(),
	synchrostep(0), entriesFrom(0), entriesTo(100), dClients(),
	currentClientId(0), repeatingQuery(false), docsToUpload(), currentDoc(0), currentEntry(0), isFullSync(false)
{
	// emplacing widgets
	this->setLayout(mainLayout);
	mainLayout->addWidget(syncInfo);
	mainLayout->addLayout(partialActionsLayout);
	mainLayout->addWidget(syncButton);
	mainLayout->addWidget(backButton);
	mainLayout->setContentsMargins(1, 1, 1, 1);
	mainLayout->setSpacing(0);
	partialActionsLayout->addWidget(sendButton);
	partialActionsLayout->addWidget(receiveButton);

	// setting appearance for buttons
	syncButton->setIcon(QIcon(":/res/upload.png"));

	receiveButton->setIcon(QIcon(":/res/netDownload.png"));
	receiveButton->setStyleSheet(DOWNLOAD_BUTTON_STYLESHEET);

	sendButton->setIcon(QIcon(":/res/netUpload.png"));
	sendButton->setStyleSheet(UPLOAD_BUTTON_STYLESHEET);

	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	backButton->setIcon(QIcon(":/res/back.png"));
	fillTexts();

	// connecting slots
	QObject::connect(syncButton, &MegaIconButton::clicked, this, &SyncMenuWidget::makeFullSync);
	QObject::connect(receiveButton, &MegaIconButton::clicked, this, &SyncMenuWidget::receiveData);
	QObject::connect(sendButton, &MegaIconButton::clicked, this, &SyncMenuWidget::sendData);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &SyncMenuWidget::backRequired);
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &SyncMenuWidget::operate_on_response);
}

void SyncMenuWidget::setLogin(QString& nlogin, QString& ologin)
{
	previousLogin = ologin;
	syncInfo->setInfopack(nlogin, ologin,
		QString::number(AppData->countData(predefinedDBNames[Documents])));
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
	if (!(AppSettings->localLogin == previousLogin))
	{
		int res = QMessageBox::warning(this, tr("Warning"), tr("Non equal login warning"),
			QMessageBox::Ok, QMessageBox::Cancel);
		if (res == QMessageBox::Cancel)
		{
			synchrostep = 0;
			syncInfo->setProgress(synchrostep);
			return;
		}
	}
	docsToUpload = AppData->loadEntities<DocumentEntity>();
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
		entries = AppData->loadEntities<DocumentEntryEntity>("parentDocId = " + QString::number(d->documentId));
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
	AppData->dropEverything();
	AppData->recreateTable(DataEntity(new DocumentEntity()));
	AppData->recreateTable(DataEntity(new DocumentEntryEntity()));
	AppData->forcedCommit();
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