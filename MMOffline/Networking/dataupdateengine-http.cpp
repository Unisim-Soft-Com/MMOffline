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
	sendQuery("ping",nullptr, None);
}

void HttpUpdateEngine::sendQuery(const QString& urlpath, RequestAwaiter* awaiter, AttributesToParse attrs = AttributesToParse::None)
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
	long long int i = (long long int)awaiter;
	QVariant v(i);
	r.setAttribute(static_cast<QNetworkRequest::Attribute>(DestinationObject), QVariant((long long int)awaiter));
	auto f = r.attribute(static_cast<QNetworkRequest::Attribute>(DestinationObject));
	switch (attrs)
	{
	case SystemValues:
		r.setAttribute(static_cast<QNetworkRequest::Attribute>(SystemValues), QVariant(true));
	case None:
	default:
		break;
	}
	QNetworkReply* reply = netManager.get(r);
	switch (attrs)
	{
	case SystemValues:
		connect(reply, SIGNAL(finished()), SLOT(onSystemRequestFinished()));
		if (!(awaiter == nullptr))
		{
			QObject::connect(this, &HttpUpdateEngine::responseArrived, awaiter, &RequestAwaiter::receivePostparsedData);
			awaiter->run();
		}
		return;
	case None:
	default:
		break;
	}
	if (!(awaiter == nullptr))
		awaiter->setReplyToAwait(reply);
}

void HttpUpdateEngine::requestFinish(QNetworkReply* reply)
{
	QString res;
	QTextDecoder td(QTextCodec::codecForName("CP1251"));
	if (!reply->error())
		res = td.toUnicode(reply->readAll());
	QString errText;
	if (reply->error() == QNetworkReply::NoError) {
			if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200)
				errText =
				 reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString()
				+ " "
				+ reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
	}
	else 
	{
		errText = reply->errorString();
	}
	if (res.startsWith("({session:"))
	{
		auto temp = res.split(":");
		if (temp.count() > 0)
		{
			temp = temp.at(1).split(",");
			if (temp.count() > 0)
			{
				sessionId = temp.at(0);
			}
		}
	}
	if (GlobalAppSettings::instance()-> packetTracing)
	{
		detrace_METHDATAS("requestFinish", "session, res, err", << sessionId << res << errText);
	}
	emit responseArrived(res, errText, reply->request().attribute(static_cast<QNetworkRequest::Attribute>(DestinationObject)).toLongLong());
}

void HttpUpdateEngine::onReplyError(QNetworkReply::NetworkError)
{
#ifdef DEBUG
	//detrace_METHCALL("HttpUpdateEngine::onReplyError");
#endif
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply) {
		requestFinish(reply);
	}
}

void HttpUpdateEngine::onSystemRequestFinished()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	if (reply) {
		requestFinish(reply);
	}
}

void HttpUpdateEngine::initiateSession(QString login, QString password, RequestAwaiter* awaiter)
{
	sendQuery(
		queryTemplates.value(Login).arg(nextQueryId++).arg(login).arg(password),
		awaiter, SystemValues);
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
		for (int i = 0; i < argc; ++i)
		{
			result.arg(argv.at(i));
		}
		sendQuery(
			result, awaiter
		);
	}
}


void HttpUpdateEngine::setUrl(QString& Url)
{
	url = Url;
	initConnection();
}



