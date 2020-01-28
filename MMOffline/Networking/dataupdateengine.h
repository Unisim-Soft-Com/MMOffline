#pragma once
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include "RequestAwaiter.h"
#include "queryTemplates.h"

/*
	This abstract class of data loading engine is renundant, because it's highly likely that no other 
	protocol than TCP will be used.
*/

class DataUpdateEngine : public QObject
	// defines interfaces for future use in web requests
{
	Q_OBJECT
protected:
	int class_id;	//	defines which object of update engine will be used
	// send query with all available context and connect reply to awaiter
	virtual void sendQuery(const QString& urlpath, RequestAwaiter* awaiter) = 0;
public:
	explicit DataUpdateEngine(QObject* parent = 0);
	int myType() { return class_id; }
	//setters
	virtual void setUrl(QString& url) = 0;
	virtual void setSession(QString& session, QString& uid) = 0;
	// send query by it's id with inserting system values
	virtual void execQueryByAutofillTemplate(queryIDs id, RequestAwaiter* awaiter) = 0;
	// send query by id without any arguments
	virtual void execQueryByTemplate(queryIDs id, RequestAwaiter* awaiter = nullptr) = 0;
	// send query with system args and 1 additional argument
	virtual void execQueryByTemplate(queryIDs id, QString arg1, RequestAwaiter* awaiter = nullptr) = 0;
	// send query with system args and 2 additional arguments
	virtual void execQueryByTemplate(queryIDs ud, QString arg1, QString arg2, RequestAwaiter* awaiter = nullptr) = 0;
	// send query with system args and 3 additional arguments
	virtual void execQueryByTemplate(queryIDs, QString arg1, QString arg2, QString arg3, RequestAwaiter* awaiter = nullptr) = 0;
	// send query with system args and N additional arguments
	virtual void execQueryByTemplate(queryIDs, int argc, QStringList argv, RequestAwaiter* awaiter = nullptr) = 0;
	// send login query
	virtual void initiateSession(QString login, QString password, RequestAwaiter* awaiter = nullptr) = 0;
	// getter for userId
	virtual const QString& getUserID() = 0;
signals:
	// renundant signal, to delete
	void responseArrived(QString res, QString err, long long int receiverInfo);
};
