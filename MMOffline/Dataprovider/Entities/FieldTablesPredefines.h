#pragma once
#include "Dataprovider/TableHandlers.h"

/*
	This file is containing predefenitions of all tables used for entities. It is used for centralized access
	to table definitions. Do not forget to update it after changes in entities.
	Also here are stored complex queries.
*/

namespace fieldPredefinitions
	// Here are stored fields of tables, except the Clients table (it was made before this file)
{
	extern const QStringList documentFields;
	extern const QStringList documentEntryFields;
	extern const QStringList fieldsOfProductEntity;
	extern const QStringList fieldsOfGroupEntity;
	extern const QStringList fieldsOfNamedIdEntity;
}
// This counter used to show how much tables we have
const int PREDEFINED_TABLES_QUANTITY = DocumentEntries + 1;
// Here are stored predefined tables used in ->associatedTable() methods of entities
extern TemplatedTableHandler predefinedTables[PREDEFINED_TABLES_QUANTITY];
// Here are stored names of all tables
extern QStringList predefinedDBNames;
namespace ComplexFilters
	//Here are stored filters which are too complex for simple ->where clause.
{
	// selects quantity of products for client %1 and group %2 from entries
	extern const QString ProductQuantityLinking;
	// selects quantity of documents bound to clients, ordered the same as client select
	extern const QString ClientQuantityLinking;
	// selects all groups where most bottom groups are containing products
	extern const QString TruncateGroupsWithoutProducts;
}