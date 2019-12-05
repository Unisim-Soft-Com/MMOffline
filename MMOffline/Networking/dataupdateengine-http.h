#pragma once
#include <QNetworkRequest>
#include <QNetworkReply>
#include "dataupdateengine.h"

class HttpUpdateEngine : public DataUpdateEngine
{
	Q_OBJECT
private:
	QString url;
	uint nextQueryId;
	qint64 delay;
	QString sessionId;
	QString user_id;
	QNetworkAccessManager netManager;

	QMap<int, QString> queryTemplates;
public:
	HttpUpdateEngine(QString& Url, QObject* parent);
	QString getUrl();
	virtual void setUrl(QString& url) override;
	bool sessionReady();
	virtual void sendQuery
	(
		const QString& urlpath,
		RequestAwaiter* awaiter
	);
public slots:
	void initConnection();
public:
	virtual void initiateSession(QString login, QString password, RequestAwaiter* awaiter = nullptr) override;

	// Inherited via DataUpdateEngine
	virtual void execQueryByTemplate(queryIDs id, RequestAwaiter* awaiter ) override;
	virtual void execQueryByTemplate(queryIDs id, QString arg1, RequestAwaiter* awaiter ) override;
	virtual void execQueryByTemplate(queryIDs ud, QString arg1, QString arg2, RequestAwaiter* awaiter ) override;
	virtual void execQueryByTemplate(queryIDs, QString arg1, QString arg2, QString arg3, RequestAwaiter* awaiter ) override;
	virtual void execQueryByTemplate(queryIDs, int argc, QStringList argv, RequestAwaiter* awaiter) override;
	virtual void setSession(QString& session, QString& uid) override;
};

