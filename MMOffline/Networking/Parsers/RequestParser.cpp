#include "RequestParser.h"
namespace RequestParser {
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