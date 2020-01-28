#pragma once
#include <QNetworkRequest>
#include <QNetworkReply>
#include "Networking/RequestAwaiter.h"
#include "Networking/queryTemplates.h"

/*
	This file contains engine that is used for requesting data from provided url.
	It stores system values and query templates.
	Singletone pattern
*/

#define AppNetwork HttpUpdateEngine::instanse()

class HttpUpdateEngine : protected QObject
	// Sends requests by templates
{
	Q_OBJECT
private:
	QString url;		//	base url. Queries are attached to this url
	uint nextQueryId;	//	next id of the query
	QString sessionId;	//	session id used for most requests
	QString user_id;	//	id of logged user
	QNetworkAccessManager netManager;	//	manager for inner usage

	QMap<int, QString> queryTemplates;	//	templates of queries

	static HttpUpdateEngine* _instanse;
public:
	HttpUpdateEngine(QString& Url, QObject* parent);
	// getters and setters
	QString getUrl();
	void setUrl(QString& url);
	const QString& getUserID();
	bool sessionReady();
	// sends query using url and connects reply object to awaiter
	void sendQuery
	(
		const QString& urlpath,
		RequestAwaiter* awaiter
	);
public slots:
	void initConnection();
public:
	// Inherited via DataUpdateEngine
	void initiateSession(QString login, QString password, RequestAwaiter* awaiter = nullptr);
	void execQueryByTemplate(queryIDs id, RequestAwaiter* awaiter);
	void execQueryByAutofillTemplate(queryIDs id, RequestAwaiter* awaiter);
	void execQueryByTemplate(queryIDs id, QString arg1, RequestAwaiter* awaiter);
	void execQueryByTemplate(queryIDs ud, QString arg1, QString arg2, RequestAwaiter* awaiter);
	void execQueryByTemplate(queryIDs, QString arg1, QString arg2, QString arg3, RequestAwaiter* awaiter);
	void execQueryByTemplate(queryIDs, int argc, QStringList argv, RequestAwaiter* awaiter);
	void setSession(QString& session, QString& uid);

	static HttpUpdateEngine* instanse();
};
