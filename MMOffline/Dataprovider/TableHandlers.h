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
	Clients
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
	int table_type;
public:
	TemplatedTableHandler();
	TemplatedTableHandler( QString t_def, QString t_decl, QStringList tfields);
	QString definition() const noexcept;		//	returns table name
	QString declaration() const noexcept;		//	returns declaration of the table which is used to create it in database
	QString definition(const QString another_name) const noexcept;
	QString select_all() const noexcept;		//	returns standard select * query
	QString select_all(const QString another_name) const noexcept;
	QString select_filtered(const QString& filter) const noexcept;	//	"select where " + your input
	QString select_filtered(const QString& filter, const QString another_name) const noexcept;
	QString update(const QString& values) const noexcept;			//	"update table " + your input
	QString update(const QString& values, const QString another_name) const noexcept;
	QString delete_filtered(const QString& filter) const noexcept;	//	"delete from table " + your input
	QString delete_filtered(const QString& filter, const QString another_name) const noexcept;
	QString drop() const noexcept;							//	returns drop query for this table
	QString drop(const QString another_name) const noexcept;
	QString allFieldsDeclaration() const noexcept;
	QString insert(const QString& values) const noexcept;
	QString insert(const QString another_name, const QString& values) const noexcept;
	TemplatedTableHandler clone(QString newName);
	int getType() const noexcept { return table_type; };	//	returns type of this table
};

