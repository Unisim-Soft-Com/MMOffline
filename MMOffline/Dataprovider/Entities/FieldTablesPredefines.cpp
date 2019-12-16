#include "FieldTablesPredefines.h"




QStringList predefinedDBNames
{
	"Clients",
	"Products",


	// These tables belongs to inner data entities
	"Documents"
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
		"expires TEXT, group number, color TEXT, groupInDictionary number, groupName text, "
		"groupCounterparty number, column number, groupgr2 number )"),
		QStringList
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
			QStringLiteral("group"),
			QStringLiteral("color"),
			QStringLiteral("groupInDictionary"),
			QStringLiteral("groupName"),
			QStringLiteral("groupCounterparty"),
			QStringLiteral("column"),
			QStringLiteral("groupgr2")
		}),


	// These tables are for inner objects
	TemplatedTableHandler(
		predefinedDBNames.at(Documents),
		QStringLiteral("( documentId number, dateWhenCreated TEXT, shippingDate TEXT, clientId number, clientName TEXT,"
			" warehouseId number, warehouseName TEXT, documentType number, documentTypeName number, "
			"alreadyPaid TEXT )"),
		QStringList
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
			}
	)
};
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
QStringLiteral("group"),
QStringLiteral("color"),
QStringLiteral("groupInDictionary"),
QStringLiteral("groupName"),
QStringLiteral("groupCounterparty"),
QStringLiteral("column"),
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