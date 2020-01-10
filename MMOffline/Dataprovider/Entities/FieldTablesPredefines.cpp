#include "FieldTablesPredefines.h"

namespace fieldPredefinitions
{

	const QStringList fieldsOfProductEntity
	{
	QStringLiteral("id"),
	QStringLiteral("name"),
	QStringLiteral("price"),
	QStringLiteral("measure"),
	QStringLiteral("groupId"),
	QStringLiteral("clientIds")
	};
	const QStringList documentFields
	{
	QStringLiteral("documentId"),
	QStringLiteral("dateWhenCreated"),
	QStringLiteral("shippingDate"),
	QStringLiteral("clientId"),
	QStringLiteral("clientName"),
	QStringLiteral("warehouseId"),
	QStringLiteral("warehouseName"),
	QStringLiteral("documentType"),
	QStringLiteral("documentTypeName"),
	QStringLiteral("alreadyPaid")
	};
	const QStringList documentEntryFields
	{
	QStringLiteral("parentDocId"),
	QStringLiteral("entryId"),
	QStringLiteral("productId"),
	QStringLiteral("productName"),
	QStringLiteral("price"),
	QStringLiteral("measure"),
	QStringLiteral("quantity"),
	QStringLiteral("option1"),
	QStringLiteral("option2"),
	QStringLiteral("option3"),
	QStringLiteral("comment")
	};
	const QStringList fieldsOfGroupEntity
	{
		QStringLiteral("name"),
		QStringLiteral("id"),
		QStringLiteral("parent_id")
	};
	const QStringList fieldsOfNamedIdEntity
	{
		QStringLiteral("name"),
		QStringLiteral("id")
	};

}
using namespace fieldPredefinitions;
	QStringList predefinedDBNames
	{
		QStringLiteral("Clients"),
		QStringLiteral("Products"),
		QStringLiteral("Groups"),
		QStringLiteral("NamedIds"),
		// These tables belongs to inner data entities
		QStringLiteral("Documents"),
		QStringLiteral("Entries")
	};

	TemplatedTableHandler predefinedTables[PREDEFINED_TABLES_QUANTITY]
	{
		TemplatedTableHandler(
			predefinedDBNames.at(Clients),
			QStringLiteral("( id INTEGER PRIMARY KEY NOT NULL, name TEXT )"),
			QStringList
			{
				QStringLiteral("id"),
				QStringLiteral("name")
			}, 0),
		TemplatedTableHandler(
			predefinedDBNames.at(Products),
			QStringLiteral("( id INTEGER PRIMARY KEY NOT NULL, name TEXT, price number,"
			" measure number,"
			"groupId INTEGER, "
			"clientIds TEXT )"),
			fieldsOfProductEntity, 0),

		TemplatedTableHandler(
			predefinedDBNames.at(Groups),
			QStringLiteral("( name TEXT, id INTEGER PRIMARY KEY NOT NULL, parent_id INTEGER )"),
			fieldsOfGroupEntity, 1),
		TemplatedTableHandler(
			predefinedDBNames.at(NamedIds),
			QStringLiteral("( name TEXT, id INTEGER PRIMARY KEY NOT NULL)"),
			fieldsOfNamedIdEntity, 1
	),
		// These tables are for inner objects
		TemplatedTableHandler(
			predefinedDBNames.at(Documents),
			QStringLiteral("( documentId INTEGER PRIMARY KEY NOT NULL, dateWhenCreated TEXT, shippingDate TEXT, clientId INTEGER, clientName TEXT,"
				" warehouseId INTEGER, warehouseName TEXT, documentType number, documentTypeName TEXT, "
				"alreadyPaid NUMBER )"),
			documentFields, 0
		),
		TemplatedTableHandler(
			predefinedDBNames.at(DocumentEntries),
			QStringLiteral("( parentDocId INTEGER, entryId INTEGER PRIMARY KEY NOT NULL, productId INTEGER, productName TEXT,"
			"price number, measure INTEGER, quantity number, option1 INTEGER, option2 INTEGER, option3 INTEGER, comment TEXT )"),
			documentEntryFields, 1
	)
	};
	namespace ComplexFilters
	{
		const QString ProductQuantityLinking =
			QStringLiteral("select DISTINCT a.productid, ifnull(a.quantity,0) quantity from ( select c.id cod_client, g.id cod_group, p.id productid "
				", (select sum(e.quantity) from Entries e, Documents d where d.clientId = c.id and e.parentDocId = d.documentId  "
				"and e.productId = p.id) quantity  "
				"from Clients c, Products p, Groups g where c.id = %1 and g.id = %2 and g.id = p.groupId and p.clientIds like '%'||c.id||'%') a"
				);
		const QString ClientQuantityLinking = 
			QStringLiteral("select c.id cod_client, (select count(documentId) from Documents d where c.id = d.clientId) cnt from Clients c;");
	}