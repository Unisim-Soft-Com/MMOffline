#pragma once

#include "JsonUniresult.h"
#include "Dataprovider/DataEntities.h"
#include <QtCore/QVector>
#include "Networking/Parsers/LinearParser.h"

/*
	This file contains set of functions allowing parsing of the result from the uniform result
*/

namespace RequestParser
{
	template <class DataEnt>
	DataEnt interpretAs(uniform_json_object_representation& o)
		// creates data entity inheritor from uniresult
	{
		DataEnt newObj;
		if (newObj.fromUniJson(o))
		{
			return newObj;
		}
		return DataEnt();
	};

	QVector<DataEntity> interpretAs(const QVector<uniform_json_object_representation>& vo, DataEntity prototype);
		// parses vector of unijson into vector of particular DataEntity
	
	struct ListedEntitiesResponse
		// this structure represents result as list of DataEntities
	{
		QVector<DataEntity> data;	//	list of polymorthic entities
		QString error;				//	contains error message
		bool isError;				//	true if response was not parsed due to error
	};
	inline QString showLEResponse(ListedEntitiesResponse* resp)
		// transforms response into string with insertion queries
	{
		QString tret;
		if (resp != nullptr)
		{
			tret = (resp->isError) ? (QStringLiteral("not valid ") + resp->error) : QStringLiteral("is valid ");
			for (DataEntity e : resp->data)
			{
				tret += e->insertionQuery();
			}
		}
		return tret;
	}

	ListedEntitiesResponse parseAndInterpretListAs(const QString& res, const QString& err, DataEntity prototype);
		// final parsing function, performs all parsing, returns response with the list of polymorthic entities
	
	struct LoginResult
		// represents login result with session and user_id
	{
		QString session;
		QString uid;
		QString error;
		bool isError;
		LoginResult(QString s, QString u, QString e, bool i) : session(s), uid(u), error(e), isError(i) {};
		LoginResult(QString e) : session(), uid(), error(e), isError(true) {};
	};
	// parses response to login request
	LoginResult getLoginResult(const QString& res, const QString& err);
}