#include "DocumentEntryEntity.h"
#include <QVariant>
#include <QTime>
const QStringList fieldDefaults
{
QStringLiteral("0"),
QStringLiteral("0"),
QStringLiteral("0"),
QStringLiteral(""),
QStringLiteral("0"),
QStringLiteral("0"),
QStringLiteral("0"),
QStringLiteral(""),
QStringLiteral(""),
QStringLiteral(""),
QStringLiteral("")
};
const QString entryIdAssertionQuery =
QStringLiteral("select entryId from Entries where entryId = %1;");

using namespace fieldPredefinitions;
uniform_json_object_representation DocumentEntryEntity::toJsonRepresentation() const
{
	return uniform_json_object_representation(
		documentEntryFields,
		QStringList
		{
			QString::number(parentDocId),
			QString::number(entryId),
			QString::number(productId),
			productName,
			QString::number(price),
			QString::number(measure),
			QString::number(quantity),
			option1,
			option2,
			option3,
			comment
		}
	);
}

bool DocumentEntryEntity::fromJsonRepr(const uniform_json_object_representation& o)
{
	if (o.size() == 0)
		return false;
	_listInit(o.mapValues(documentEntryFields, fieldDefaults));
	return true;
}

TemplatedTableHandler* DocumentEntryEntity::getAssocTable() const
{
	return predefinedTables + DocumentEntries;
}

QString DocumentEntryEntity::getContentsForDb() const
{
	return	QStringLiteral("( ") +
		QString::number(parentDocId)
		+ QStringLiteral(" , ") +
		QString::number(entryId)
		+ QStringLiteral(" , ") +
		QString::number(productId)
		+ QStringLiteral(" , \"") +
		productName
		+ QStringLiteral("\" , ") +
		QString::number(price)
		+ QStringLiteral(" , ") +
		QString::number(measure)
		+ QStringLiteral(" , ") +
		QString::number(quantity)
		+ QStringLiteral(" , \"") +
		option1
		+ QStringLiteral("\" , \"") +
		option2
		+ QStringLiteral("\" , \"") +
		option3
		+ QStringLiteral("\", \"") +
		comment
		+ QStringLiteral("\" )");
}

abs_entity* DocumentEntryEntity::fabricate() const
{
	return new DocumentEntryEntity(*this);
}

bool DocumentEntryEntity::fromSql(QueryPtr q)
{
	if (!q->next())
		return false;
	QStringList values;
	for (int i = 0; i < documentEntryFields.count(); ++i)
	{
		values << q->value(i).toString();
	}
	_listInit(values);
	return true;
}

void DocumentEntryEntity::_listInit(const QStringList& l)
{
	int i = l.count() - 1;
	bool ok = true;
	switch (l.count())
	{
	case 11:
		comment = l.at(i--);
	case 10:
		option3 = l.at(i--);
	case 9:
		option2 = l.at(i--);
	case 8:
		option1 = l.at(i--);
	case 7:
		quantity = l.at(i--).toDouble(&ok);
		if (!ok) break;
	case 6:
		measure = l.at(i--).toInt(&ok);
		if (!ok) break;
	case 5:
		price  = l.at(i--).toDouble(&ok); 
		if (!ok) break;
	case 4:
		productName = l.at(i--);
	case 3:
		productId = l.at(i--).toLongLong(&ok);
		if (!ok) break;
	case 2:
		entryId = l.at(i--).toLongLong(&ok);
		if (!ok) break;
	case 1:
		parentDocId = l.at(i--).toLongLong(&ok);
		break;
	default:
		break;
	}
	if (!ok)
	{
		throw InitializationError(i);
	}
}

DocumentEntryEntity::DocumentEntryEntity()
	: abs_entity(DocumentEntries), parentDocId(0), entryId(0), productId(0),
	productName(), price(0.0), measure(0), quantity(0.0), option1(),
	option2(), option3(), comment()
{

}

DocumentEntryEntity::DocumentEntryEntity(IdInt ID)
	: abs_entity(DocumentEntries), parentDocId(0), entryId(0), productId(0),
	productName(), price(0.0), measure(0), quantity(0.0), option1(),
	option2(), option3(), comment()
{
}

DocumentEntryEntity::DocumentEntryEntity(const QStringList& l)
{
	_listInit(l);
}

bool DocumentEntryEntity::isLikeString(const QRegExp& qregexp) const
{
	return false;
}

IdInt DocumentEntryEntity::extractId() const
{
	return entryId;
}

