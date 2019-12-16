#pragma once

#include "JsonUniresult.h"
#include "Dataprovider/DataEntities.h"
#include <QtCore/QVector>
#include "Networking/Parsers/LinearParser.h"

namespace RequestParser
{
	template <class DataEnt>
	DataEnt interpretAs(uniform_json_object_representation& o)
	{
		DataEnt newObj;
		if (newObj.fromUniJson(o))
		{
			return newObj;
		}
		return DataEnt();
	};

	template <class DataEnt>
	QVector<DataEntity> interpretAs(const QVector<uniform_json_object_representation>& vo)
	{
		DataEntity temp(new DataEnt());
		QVector<DataEntity> toreturn;
		auto start = vo.constBegin();
		while (start != vo.constEnd())
		{
			if (temp->fromUniJson(*start++))
			{
				toreturn.push_back(DataEntity(temp->clone()));
			}
		}
		return toreturn;
	}

	struct ListedEntitiesResponse
	{
		QVector<DataEntity> data;
		QString error;
		bool isError;
	};
	inline QString showLEResponse(ListedEntitiesResponse* resp)
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
	template <class DataEnt>
	ListedEntitiesResponse parseAndInterpretListAs(const QString& res, const QString& err)
	{
		ListedEntitiesResponse toreturn;
		toreturn.isError = false;
		LinearParser parser(res, err);
		auto result = parser.read();
		if (parser.hasError() || (! parser.isSuccesfull()))
		{
			toreturn.error = parser.getErrors();
			toreturn.isError = true;
			return toreturn;
		}
		else
		{
			toreturn.data = interpretAs<DataEnt>(result->queryResult);
			if (toreturn.data.count() == 0 && result->queryResult.count() > 0)
			{
				toreturn.error = QStringLiteral("Interpretation error: possible template argument mismatch!");
				toreturn.isError = true;
			}
			return toreturn;
		}
	}
	struct LoginResult
	{
		QString session;
		QString uid;
		QString error;
		bool isError;
		LoginResult(QString s, QString u, QString e, bool i) : session(s), uid(u), error(e), isError(i) {};
		LoginResult(QString e) : session(), uid(), error(e), isError(true) {};
	};
	static LoginResult getLoginResult(const QString& res, const QString& err)
	{
		ConfirmationResponseParser parser(res, err);
		auto result = parser.read();
		if (parser.isSuccesfull())
		{
			return LoginResult(result->queryResult.first().at(0), result->queryResult.first().at(1), QString(), false);
		}
		else
		{
			return LoginResult(parser.getErrors());
		}
	}

}