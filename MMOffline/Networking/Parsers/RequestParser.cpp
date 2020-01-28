#include "RequestParser.h"
namespace RequestParser {
	QVector<DataEntity> interpretAs(const QVector<uniform_json_object_representation>& vo, DataEntity prototype)
	{
		QVector<DataEntity> toreturn;
		auto start = vo.constBegin();
		while (start != vo.constEnd())
		{
			if (prototype->fromUniJson(*start++))
			{
				toreturn.push_back(DataEntity(prototype->clone()));
			}
		}
		return toreturn;
	}

	ListedEntitiesResponse parseAndInterpretListAs(const QString& res, const QString& err, DataEntity prototype)
	{
		ListedEntitiesResponse toreturn;
		toreturn.isError = false;
		LinearParser parser(res, err);
		auto result = parser.read();
		if (parser.hasError() || (!parser.isSuccesfull()))
		{
			toreturn.error = parser.getErrors();
			toreturn.isError = true;
			return toreturn;
		}
		else
		{
			toreturn.data = interpretAs(result->queryResult, prototype);
			if (toreturn.data.count() == 0 && result->queryResult.count() > 0)
			{
				toreturn.error = QStringLiteral("Interpretation error: possible template argument mismatch!");
				toreturn.isError = true;
			}
			return toreturn;
		}
	}

	LoginResult RequestParser::getLoginResult(const QString& res, const QString& err)
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