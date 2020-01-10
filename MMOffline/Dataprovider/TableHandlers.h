#pragma once
#include <QtCore/QString>
#include <QtCore/QStringList>
/*
	This file is holding abstract table handler class and it's inheritors. main goal of this class
	is to provide common interface for tables allowing polymorfic access to them. So, these handlers
	are making queries filled with corresponding data - which fields must be selected, how many columns
	must exist etc. General example is using this with Scaned and Downloaded table: their queries are similar
	so you can call select all with same code but it will be expanded in different queries.

*/

enum TableNames
{
	Clients,
	Products,
	Groups,
	NamedIds,
	// inner non-received entities
	Documents,
	DocumentEntries
};
namespace query_templates
{
	enum QueryTemplates
	{
		DropTableQuery,
		NotAQuery
	};

	extern const QString QueryTemplateTable[NotAQuery];
}


class TemplatedTableHandler 
{
protected:
	QString table_definition;
	QString table_declaration;
	QStringList table_fields;
	int primaryKeyField;
	int table_type;

	const QString& assertAnotherName(const QString& another_name) const noexcept;

public:
	TemplatedTableHandler();
	TemplatedTableHandler( QString t_def, QString t_decl, QStringList tfields, int primarykey = -1);
	QString declaration() const noexcept;		//	returns declaration of the table which is used to create it in database
	QString definition(const QString another_name = QString::null) const noexcept;
	QString select_all(const QString another_name = QString::null) const noexcept;
	QString select_filtered(const QString& filter, const QString another_name = QString::null) const noexcept;
	QString select_by_primary_key(const QString pkeyvalue, const QString another_name = QString::null) const noexcept;
	QString update(const QString& values, const QString another_name = QString::null) const noexcept;
	QString replace(const QString& values, const QString& another_name = QString::null) const noexcept;
	QString delete_filtered(const QString& filter, const QString another_name = QString::null) const noexcept;
	QString delete_by_primary_key(const QString& pkeyvalue, const QString another_name = QString::null) const noexcept;
	QString drop(const QString another_name = QString::null) const noexcept;
	QString allFieldsDeclaration() const noexcept;
	QString insert( const QString& values, const QString another_name = QString::null) const noexcept;
	QString makeIndex(const QString& another_name = QString::null) const noexcept;
	TemplatedTableHandler clone(QString newName);
	int getType() const noexcept { return table_type; };	//	returns type of this table
};

