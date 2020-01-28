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
QStringLiteral("0"),
QStringLiteral("0"),
QStringLiteral("0"),
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
			QString::number(option1),
			QString::number(option2),
			QString::number(option3),
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
		+ QStringLiteral(" , ") +
		QString::number(option1)
		+ QStringLiteral(" , ") +
		QString::number(option2)
		+ QStringLiteral(" , ") +
		QString::number(option3)
		+ QStringLiteral(", \"") +
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
		option3 = l.at(i--).toInt(&ok);
		if (!ok) break;
	case 9:
		option2 = l.at(i--).toInt(&ok);
		if (!ok) break;
	case 8:
		option1 = l.at(i--).toInt(&ok);
		if (!ok) break;
	case 7:
		quantity = l.at(i--).toDouble(&ok);
		if (!ok) break;
	case 6:
		measure = l.at(i--).toInt(&ok);
		if (!ok) break;
	case 5:
		price = l.at(i--).toDouble(&ok);
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
	productName(), price(0.0), measure(0), quantity(0.0), option1(0),
	option2(0), option3(0), comment()
{
}

DocumentEntryEntity::DocumentEntryEntity(IdInt ID)
	: abs_entity(DocumentEntries), parentDocId(0), entryId(0), productId(0),
	productName(), price(0.0), measure(0), quantity(0.0), option1(0),
	option2(0), option3(0), comment()
{
}

DocumentEntryEntity::DocumentEntryEntity(const QStringList& l)
{
	_listInit(l);
}

bool DocumentEntryEntity::higherThan(const abs_entity* another) const
{
	return entryId > another->getId();
}

bool DocumentEntryEntity::compare(abs_entity* another) const
{
	auto temp = dynamic_cast<DocumentEntryEntity*>(another);
	if (temp == nullptr)
		return false;
	return entryId == temp->entryId;
}

bool DocumentEntryEntity::isLikeString(const QRegExp& qregexp) const
{
	return productName.contains(qregexp.pattern(), Qt::CaseInsensitive);
}

IdInt DocumentEntryEntity::extractId() const
{
	return entryId;
}