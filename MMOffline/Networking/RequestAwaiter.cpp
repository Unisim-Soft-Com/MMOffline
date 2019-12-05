#include "RequestAwaiter.h"
#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <QtCore/QTextDecoder>
#include "Widgets/utils/GlobalAppSettings.h"
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif
const char* RECEIVER_SLOT_NAME = "requestIncoming";


void removeRSE(QString* str)
{
	int cnt = 0;
	auto strbeg = str->begin();
	while (!(strbeg == str->end()))
	{
		if ((*strbeg++) == QChar('{'))
		{
			break;
		}
		cnt++;
	}
	str->remove(0,cnt);
	str->chop(2);


}

RequestAwaiter::RequestAwaiter(int interval, QObject* parent)
	: QObject(parent), timer(new QTimer(this)), awaiting(false), timeoutinterval(interval),awaitedReply(nullptr)
{
#ifdef DEBUG
	//detrace_METHEXPL("interval was:" << interval );
#endif
	timer->setInterval(interval);
	timer->setSingleShot(true);
	QObject::connect(timer, &QTimer::timeout, this, &RequestAwaiter::timeout);
}

void RequestAwaiter::run()
{
	timer->setInterval(timeoutinterval);
	timer->start();

#ifdef DEBUG
	//detrace_METHEXPL("now with interval " << timer->interval() << " remaining is " << timer->remainingTime());
#endif

	awaiting = true;
	wastimeout = false;
	restext.clear();
	errtext.clear();
}

bool RequestAwaiter::isAwaiting()
{
	return awaiting;
}

bool RequestAwaiter::wasTimeout()
{
	return wastimeout;
}

int RequestAwaiter::getInterval()
{
	return timeoutinterval;
}

void RequestAwaiter::setReplyToAwait(QNetworkReply* toAwait)
{
	if (awaitedReply != nullptr)
		return;
	awaitedReply = toAwait;
	QObject::connect(toAwait, &QNetworkReply::finished, this, &RequestAwaiter::requestIncoming);
	QObject::connect(toAwait, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &RequestAwaiter::replyError);
	run();
}

void RequestAwaiter::stopAwaiting()
{
	awaiting = false;
	wastimeout = false;
	timer->stop();
}

void RequestAwaiter::timeout()
{
#ifdef DEBUG
	// detrace_METHCALL("RequestAwaiter::timeout");
	 //detrace_METHEXPL(timer->remainingTime() << " msecs remaining");
#endif
	awaiting = false;
	wastimeout = true;
	if (awaitedReply != nullptr) {
		awaitedReply->deleteLater();
		awaitedReply = nullptr;
	}
	emit requestTimeout();
}

void RequestAwaiter::requestIncoming()
{
	QTextDecoder td(QTextCodec::codecForName("CP1251"));
	if (!awaitedReply->error())
		restext = td.toUnicode(awaitedReply->readAll());
	if (awaitedReply->error() == QNetworkReply::NoError) {
		if (awaitedReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200)
			errtext =
			awaitedReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString()
			+ " "
			+ awaitedReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
	}
	else
	{
		errtext = awaitedReply->errorString();
	}
	awaiting = false;
	timer->stop();
	wastimeout = false;
	if (awaitedReply != nullptr) {
		awaitedReply->deleteLater();
		awaitedReply = nullptr;
	}
	removeRSE(&restext);
	if (GlobalAppSettings::instance()->packetTracing)
	{
		detrace_NETRESPREC(restext, errtext);
	}
	emit requestSuccess(restext, errtext);
	emit requestReceived();
}

void RequestAwaiter::replyError(QNetworkReply::NetworkError)
{
}

