#include "DataEntities.h"
#include <QtCore/QVariant>
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif

QStringList predefinedDBNames
{
	"Clients",
};

TemplatedTableHandler predefinedTables[PREDEFINED_TABLES_QUANTITY]
{
	TemplatedTableHandler(
		predefinedDBNames.at(Clients),
		QStringLiteral("( id number, name TEXT )"),
		QStringList
		{
			QStringLiteral("id"),
			QStringLiteral("name")
		}
		)
};
const abs_entity *  const associateTableWithData[PREDEFINED_TABLES_QUANTITY]
{
	new ClientEntity()
};



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

const TemplatedTableHandler * const abs_entity::getAssociatedTable() const
{
	return getAssocTable();
}

TemplatedTableHandler abs_entity::getRenamedTable(const QString & tname) const
{
	return getAssocTable()->clone(tname);
}

TemplatedTableHandler abs_entity::getRenamedTable(TableNames tname) const
{
	return getAssocTable()->clone(predefinedDBNames.at(tname));
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
	return  getAssocTable()->insert(another_table, getContentsForDb());
}

int abs_entity::myType() const
{
	return class_id;
}

abs_entity* abs_entity::clone() const
{
	return fabricate();
}

uniform_json_object_representation ClientEntity::toJsonRepresentation() const
{
	return uniform_json_object_representation(
		QStringList { "id", "name"},
		QStringList { QString::number(id), name}
	);
}

bool ClientEntity::fromJsonRepr(const uniform_json_object_representation& o)
{
	QString t = o.value(QLatin1String("id"));
	bool ok;
	int tmp = t.toInt(&ok);
	if (ok)
	{
		id = tmp;
	}
	else
		return false;
	name = o.value(QLatin1String("name"));
	if (name.isNull())
		return false;
	return true;
}

TemplatedTableHandler* ClientEntity::getAssocTable() const
{
	return predefinedTables + Clients;
}


QString ClientEntity::getContentsForDb() const
{
	return QStringLiteral("( ") + QString::number(id) + QStringLiteral(" , \"") + name + QStringLiteral("\" )");
}

abs_entity* ClientEntity::fabricate() const
{
	return new ClientEntity(id, name);
}

ClientEntity::ClientEntity()
	:abs_entity(Clients)
{
}

ClientEntity::ClientEntity(int Id, QString Name)
	: abs_entity(Clients), id(Id), name(Name)
{

}


bool ClientEntity::fromSql(QueryPtr q)
{
	if (!q->next())
		return false;
	bool ok;
	QVariant temp(q->value(0));
	if (!temp.isValid())
		return false;
	id = temp.toInt(&ok);
	if (!ok)
		return false;
	temp = q->value(1);
	if (!temp.isValid())
		return false;
	name = temp.toString();
	return true;
}
