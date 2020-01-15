#include "abs_entity.h"

abs_entity::abs_entity(int ClassId)
	: class_id(ClassId)
{
}

uniform_json_object_representation abs_entity::toUniJson() const
{
	return toJsonRepresentation();
}

bool abs_entity::fromUniJson(const uniform_json_object_representation& unijs)
{
	return fromJsonRepr(unijs);
}

bool abs_entity::fromSqlQuery(QueryPtr q)
{
	return fromSql(q);
}

const TemplatedTableHandler* const abs_entity::getAssociatedTable() const
{
	return getAssocTable();
}

TemplatedTableHandler abs_entity::getRenamedTable(const QString& tname) const
{
	return getAssocTable()->clone(tname);
}

TemplatedTableHandler abs_entity::getRenamedTable(TableNames tname) const
{
	return getAssocTable()->clone(predefinedDBNames.at(tname));
}

IdInt abs_entity::getId() const
{
	return extractId();
}

QString abs_entity::insertToDBHeader() const
{
	return getAssocTable()->allFieldsDeclaration();
}

QString abs_entity::insertToDBValues() const
{
	return getContentsForDb();
}

QString abs_entity::insertionQuery() const
{
	return  getAssocTable()->insert(getContentsForDb());
}

QString abs_entity::insertionQuery(const QString another_table) const
{
	return  getAssocTable()->insert(getContentsForDb(), another_table);
}

bool abs_entity::deepCompare(abs_entity* another) const
{
	if (another->class_id == this->class_id)
	{
		return compare(another);
	}
	return false;
}

int abs_entity::myType() const
{
	return class_id;
}

bool abs_entity::filter(const QRegExp& qrexp) const
{
	return isLikeString(qrexp);
}

abs_entity* abs_entity::clone() const
{
	return fabricate();
}

const QString dateSerializationFormat =
"dd.MM.yyyy";
const QString dateTimeSerializationFormat =
"dd.MM.yyyy HH.mm.ss";