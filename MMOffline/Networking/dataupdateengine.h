#pragma once
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include "RequestAwaiter.h"
#include "queryTemplates.h"

class DataUpdateEngine : public QObject
{
	Q_OBJECT
protected:
	int class_id;
	virtual void sendQuery(const QString& urlpath, RequestAwaiter* awaiter) = 0;
public:
	explicit DataUpdateEngine(QObject* parent = 0);
	int myType() { return class_id; }
	virtual void setUrl(QString& url) = 0;
	virtual void setSession(QString& session, QString& uid) = 0;
	virtual void execQueryByAutofillTemplate(queryIDs id, RequestAwaiter* awaiter) = 0;
	virtual void execQueryByTemplate(queryIDs id, RequestAwaiter* awaiter = nullptr) = 0;
	virtual void execQueryByTemplate(queryIDs id, QString arg1, RequestAwaiter* awaiter = nullptr) = 0;
	virtual void execQueryByTemplate(queryIDs ud, QString arg1, QString arg2, RequestAwaiter* awaiter = nullptr) = 0;
	virtual void execQueryByTemplate(queryIDs, QString arg1, QString arg2, QString arg3, RequestAwaiter* awaiter = nullptr) = 0;
	virtual void execQueryByTemplate(queryIDs, int argc, QStringList argv, RequestAwaiter* awaiter = nullptr) = 0;
	virtual void initiateSession(QString login, QString password, RequestAwaiter* awaiter = nullptr) = 0;
	virtual const QString& getUserID() = 0;
signals:
	void responseArrived(QString res, QString err, long long int receiverInfo);
};
