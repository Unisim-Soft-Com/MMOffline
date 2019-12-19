#include "FieldTablesPredefines.h"


const QStringList fieldsOfProductEntity
{
QStringLiteral("id"),
QStringLiteral("name"),
QStringLiteral("shortName"),
QStringLiteral("price"),
QStringLiteral("priceWithTaxes"),
QStringLiteral("measure"),
QStringLiteral("quantity"),
QStringLiteral("quantityInPieces"),
QStringLiteral("rest"),
QStringLiteral("expires"),
QStringLiteral("groupId"),
QStringLiteral("color"),
QStringLiteral("groupInDictionary"),
QStringLiteral("groupName"),
QStringLiteral("groupCounterparty"),
QStringLiteral("columnBE"),
QStringLiteral("groupgr2")
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
QStringLiteral("parendDocId"),
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

QStringList predefinedDBNames
{
	QStringLiteral("Clients"),
	QStringLiteral("Products"),
	QStringLiteral("Groups"),

	// These tables belongs to inner data entities
	QStringLiteral("Documents"),
	QStringLiteral("DocumentEntries")
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
		}),
	TemplatedTableHandler(
		predefinedDBNames.at(Products),
		QStringLiteral("( id number, name TEXT, shortName TEXT, price number, priceWithTaxes number,"
		" measure number, quantity number, quantityInPieces number, rest number, "
		"expires TEXT, groupId number, color TEXT, groupInDictionary number, groupName text, "
		"groupCounterparty number, columnBE number, groupgr2 number )"),
		fieldsOfProductEntity),

	TemplatedTableHandler(
		predefinedDBNames.at(Groups),
		QStringLiteral("( name TEXT, id number, parent_id number )"),
		fieldsOfGroupEntity),
	TemplatedTableHandler(
		predefinedDBNames.at(NamedIds),
		QStringLiteral("( name TEXT, id number )"),
		fieldsOfNamedIdEntity
),
	// These tables are for inner objects
	TemplatedTableHandler(
		predefinedDBNames.at(Documents),
		QStringLiteral("( documentId number, dateWhenCreated TEXT, shippingDate TEXT, clientId number, clientName TEXT,"
			" warehouseId number, warehouseName TEXT, documentType number, documentTypeName number, "
			"alreadyPaid TEXT )"),
		documentFields
	),
	TemplatedTableHandler(
		predefinedDBNames.at(DocumentEntries),
		QStringLiteral("( parentDocId number, entryId number, productId number, productName TEXT,"
		"price number, measure number, quantity number, option1 TEXT, option2 TEXT, option3 TEXT, comment TEXT )"),
		documentEntryFields
)
};
