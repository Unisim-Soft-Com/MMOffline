#include "ErrorParser.h"


namespace RequestParser
{
	bool containsError(QJsonObject* doc)
	{
		return doc->contains(QStringLiteral("error"));
	}

	parsedError makeError(QJsonObject* doc)
	{
		parsedError err;
		err.displayable = doc->value(QStringLiteral("error")).toString();
		err.full = doc->value(QStringLiteral("error_full")).toString();
		err.trace = doc->value(QStringLiteral("trace")).toString();
		err.isError = !err.displayable.isEmpty();
		return err;
	}
}