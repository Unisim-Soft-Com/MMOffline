#include "LinearParser.h"
#include <QtCore/QJsonArray>



RequestParser::LinearParser::LinearParser( const QString& restext, const QString& errtext)
	: abs_parser(restext, errtext)
{
	uniresult->nameOf = "itemlist";
	success =  doParsing();
}

bool RequestParser::LinearParser::_parseThis()
{
	if (isErrorMessage)
		return true;
	if (!innerObject.keys().contains(QLatin1String("items")))
	{
		return false;
	}
	QJsonArray jarray = innerObject.value(QLatin1String("items")).toArray();
	if (jarray.count() == 0)
		return true;
	QJsonObject tobj;
	for (int i = 0; i < jarray.count(); ++i)
	{
		tobj = jarray.at(i).toObject();
		uniresult->queryResult.push_back(uniform_json_object_representation(tobj));
	}
	return true;
}
