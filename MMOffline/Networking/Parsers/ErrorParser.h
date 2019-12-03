#pragma once
#include <QtCore/QJsonArray>
#include "Networking/queryTemplates.h"





class parsedError
{
	bool isError;
	QString displayable;
	QString full;
	QString trace;
};


parsedError makeError(QJsonArray*)

