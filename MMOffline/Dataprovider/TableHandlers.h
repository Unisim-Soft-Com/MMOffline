#pragma once
#include <QtCore/QString>
#include <QtCore/QStringList>

/*
	This file is holding template for "normal" entity table handler. This handler can construct most of 
	common queries from provided data, allowing polymorthic table access, i.e. you can change pointer to 
	handler or recreate it instead of manual fixes in code. Main goal of this handler is to hide 
	raw SQL queries to allow global changes without multiple manual replaces. This handler
	is losing some speed compared to hardcoded queries, so consider to create variables to remember
	frequently used queries, like insertions.

*/

enum TableNames
	// contains table numbers, which are used as entity types and allows indexation in predefined arrays
{
	// outer received entities
	Clients,
	Products,
	Groups,
	NamedIds,
	// inner non-received entities
	Documents,
	DocumentEntries
};
namespace query_templates
	// this namespace stores templates for table independent query, which can be used without 
	// providing table schema information
{
	enum QueryTemplates
	{
		DropTableQuery,		//	drop table
		CountElementsQuery,		//	count (*)
		NotAQuery			//	skip execution
	};

	extern const QString QueryTemplateTable[NotAQuery];
}

class TemplatedTableHandler
	// encapsulates table, automatizing query creation
{
protected:
	QString table_definition;	//	table schema (fields and their types)
	QString table_declaration;	//	table name (can be overrided without recreating handler)
	QStringList table_fields;	//	field names
	int primaryKeyField;		//	which field is used as primary key
	int table_type;				//	renundant type, can be used again in case of additional templates

	// checks name, returns valid one
	const QString& assertAnotherName(const QString& another_name) const noexcept;

public:
	TemplatedTableHandler();
	TemplatedTableHandler(QString t_def, QString t_decl, QStringList tfields, int primarykey = -1);
	QString declaration() const noexcept;		//	returns declaration of the table which is used to create it in database
	// returns table creation query
	QString definition(const QString another_name = QString::null) const noexcept;
	// selects all using field order provided in constructor
	QString select_all(const QString another_name = QString::null) const noexcept;
	// select using filter. Keeps column order.
	QString select_filtered(const QString& filter, const QString another_name = QString::null) const noexcept;
	// select using primary key. keeps column order
	QString select_by_primary_key(const QString pkeyvalue, const QString another_name = QString::null) const noexcept;
	// updates table
	QString update(const QString& values, const QString another_name = QString::null) const noexcept;
	// replaces values using REPLACE. It guaranties that after query there will be only one value
	QString replace(const QString& values, const QString& another_name = QString::null) const noexcept;
	// deletes all non-filtered
	QString delete_filtered(const QString& filter, const QString another_name = QString::null) const noexcept;
	// deletes one element by it's primary key
	QString delete_by_primary_key(const QString& pkeyvalue, const QString another_name = QString::null) const noexcept;
	// drops table. Is a wrapper to query template
	QString drop(const QString another_name = QString::null) const noexcept;
	// returns joined names
	QString allFieldsDeclaration() const noexcept;
	// makes insertion query from provided values
	QString insert(const QString& values, const QString another_name = QString::null) const noexcept;
	// creates index for this table
	QString makeIndex(const QString& another_name = QString::null) const noexcept;
	// creates copy of this handler with another name
	TemplatedTableHandler clone(QString newName);
	// returns type of this handler. Renundant
	int getType() const noexcept { return table_type; };	//	returns type of this table
};
