#pragma once
#include <QtCore/QJsonObject>
#include "Networking/queryTemplates.h"

/*
	This file contains error parsing function with it's result. 
	Usually you will want to check result text of the request before parsing it
	containsError checks document for key fields of database error
	makeError extracts information
*/

struct parsedError
	// structure representing database error
{
	bool isError;	//	true if error is real
	QString displayable;	//	shortest expanation
	QString full;			//	full explanation
	QString trace;			//	stack trace on the database
};
namespace RequestParser {
	// checks json object for database errors
	bool containsError(QJsonObject* doc);
	// tries to extract database error from json.
	parsedError makeError(QJsonObject* doc);
}
