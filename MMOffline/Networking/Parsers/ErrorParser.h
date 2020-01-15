#pragma once
#include <QtCore/QJsonObject>
#include "Networking/queryTemplates.h"

struct parsedError
{
	bool isError;
	QString displayable;
	QString full;
	QString trace;
};
namespace RequestParser {
	bool containsError(QJsonObject* doc);
	parsedError makeError(QJsonObject* doc);
}
