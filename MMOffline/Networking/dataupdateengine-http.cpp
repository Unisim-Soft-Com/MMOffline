#include <QtCore/QDate>
#include <QtCore/QDateTime>
#include <QtCore/QTime>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QTimer>
#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <QtCore/QTextDecoder>
#include <QtCore/QPointer>
#include <QtCore/QDebug>
#include "dataupdateengine-http.h"
#include "debugtrace.h"
#include "Widgets/utils/GlobalAppSettings.h"
#include "queryTemplates.h"


#include <QtWidgets/QMessageBox>
#define DEBUG



HttpUpdateEngine::HttpUpdateEngine(QString& Url, QObject* parent)
	: DataUpdateEngine(parent), url(Url), nextQueryId(0), delay(0), queryTemplates(_initTemplates())
{
    class_id=1;
}


bool HttpUpdateEngine::sessionReady()
{
	return !sessionId.isEmpty();
}
QString HttpUpdateEngine::getUrl()
{
	return url;
}

void HttpUpdateEngine::initConnection()
{
#ifdef DEBUG
	//detrace_METHCALL("HttpUpdateEngine::initConnection");
#endif
	sendQuery("ping",nullptr);
}

void HttpUpdateEngine::sendQuery(const QString& urlpath, RequestAwaiter* awaiter)
{
	if (awaiter != nullptr)
	{
		if (awaiter->isAwaiting())
			return;
	}
	QString currUrl = url + urlpath;
	if (GlobalAppSettings::instance()->packetTracing)
		detrace_METHDATAS("sendQuery", "url", << currUrl);
	QNetworkRequest r;
	r.setRawHeader("Puller", "MMOffline");
	r.setUrl(QUrl::fromUserInput(currUrl));
	QNetworkReply* reply = netManager.get(r);
	if (!(awaiter == nullptr))
		awaiter->setReplyToAwait(reply);
}

void HttpUpdateEngine::initiateSession(QString login, QString password, RequestAwaiter* awaiter)
{
	sendQuery(
		queryTemplates.value(Login).arg(nextQueryId++).arg(login).arg(password),
		awaiter);
}

void HttpUpdateEngine::execQueryByTemplate(queryIDs id, RequestAwaiter* awaiter )
{
	if (_checkArgQuantity(id, 0) && !sessionId.isEmpty())
	{
		sendQuery(
			queryTemplates.value(id).arg(nextQueryId++).arg(sessionId), awaiter
		);
	}
}

void HttpUpdateEngine::execQueryByAutofillTemplate(queryIDs id, RequestAwaiter* awaiter)
{
	sendQuery(
		queryTemplates.value(id).arg(nextQueryId++).arg(sessionId).arg(id), awaiter
	);
}

void HttpUpdateEngine::execQueryByTemplate(queryIDs id, QString arg1, RequestAwaiter* awaiter)
{
	if (_checkArgQuantity(id, 1) && !sessionId.isEmpty())
	{
		sendQuery(
			queryTemplates.value(id).arg(nextQueryId++).arg(sessionId).arg(arg1), awaiter
		);
	}
}

void HttpUpdateEngine::execQueryByTemplate(queryIDs id, QString arg1, QString arg2, RequestAwaiter* awaiter )
{
	if (_checkArgQuantity(id, 2) && !sessionId.isEmpty())
	{
		sendQuery(
			queryTemplates.value(id).arg(nextQueryId++).arg(sessionId).arg(arg1).arg(arg2), awaiter
		);
	}
}

void HttpUpdateEngine::execQueryByTemplate(queryIDs id, QString arg1, QString arg2, QString arg3, RequestAwaiter* awaiter )
{
	if (_checkArgQuantity(id, 3) && !sessionId.isEmpty())
	{
		sendQuery(
			queryTemplates.value(id).arg(nextQueryId++).arg(sessionId).arg(arg1).arg(arg2).arg(arg3), awaiter
		);
	}
}

void HttpUpdateEngine::execQueryByTemplate(queryIDs id, int argc, QStringList argv, RequestAwaiter* awaiter )
{
	if (_checkArgQuantity(id, argc) && !sessionId.isEmpty())
	{
		QString result = queryTemplates.value(id);
		result = result.arg(nextQueryId++).arg(sessionId);
		for (int i = 0; i < argc; ++i)
		{
			result = result.arg(argv.at(i));
		}
		sendQuery(
			result, awaiter
		);
	}
}

void HttpUpdateEngine::setSession(QString& session, QString& uid)
{
	sessionId = session;
	user_id = uid;
}

void HttpUpdateEngine::setUrl(QString& Url)
{
	url = Url;
	initConnection();
}

const QString& HttpUpdateEngine::getUserID()
{
	return user_id;
}



