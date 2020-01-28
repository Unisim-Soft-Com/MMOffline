#include "FileParser.h"

CsvFileParser::CsvFileParser(const QString path, QChar sep)
	: rootdir(path), errors(), isReady(false), separator(sep)
{
	if (!rootdir.exists())
	{
		errors << tr("Folder does not exists!");
	}
	else
	{
		isReady = true;
	}
}

bool CsvFileParser::doParsing()
{
	if (!isReady)
		return false;
	if (!_getEntity<ClientEntity>(QStringLiteral("clients.csv"), 2))
		return false;
	if (!_getEntity<ProductEntity>(QStringLiteral("products.csv"), fieldPredefinitions::fieldsOfProductEntity.count()))
		return false;
	if (!_getEntity<GroupEntity>(QStringLiteral("groups.csv"), fieldPredefinitions::fieldsOfGroupEntity.count()))
		return false;
	if (!_getEntity<NamedIdEntity>(QStringLiteral("measures.csv"), fieldPredefinitions::fieldsOfNamedIdEntity.count(),
		QStringLiteral("Measures")))
		return false;
	if (!_getEntity<NamedIdEntity>(QStringLiteral("types.csv"), fieldPredefinitions::fieldsOfNamedIdEntity.count(),
		QStringLiteral("Tips")))
		return false;
	if (!_getEntity<NamedIdEntity>(QStringLiteral("options.csv"), fieldPredefinitions::fieldsOfNamedIdEntity.count(),
		QStringLiteral("Options")))
		return false;
	if (!_getEntity<NamedIdEntity>(QStringLiteral("depozits.csv"), fieldPredefinitions::fieldsOfNamedIdEntity.count(),
		QStringLiteral("Depozits")))
		return false;
	return true;
}

QStringList CsvFileParser::errorList()
{
	return errors;
}

bool CsvFileParser::_assertElementsCount(QStringList* splitvalues, int required)
{
	if (splitvalues->count() != required)
	{
		errors << tr("values count mismatch: ")
			+ QString::number(splitvalues->count()) + tr(" instead of: ") +
			QString::number(required);
		return false;
	}
	return true;
}