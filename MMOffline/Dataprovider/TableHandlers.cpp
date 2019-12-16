#include "TableHandlers.h"

namespace query_templates
{
	const QString QueryTemplateTable[NotAQuery]
	{
		QStringLiteral("drop table %1")
	};
}




TemplatedTableHandler::TemplatedTableHandler()
{
}

TemplatedTableHandler::TemplatedTableHandler(QString t_decl, QString t_def, QStringList tfields)
	: table_definition(t_def), table_declaration(t_decl), table_fields(tfields)
{
}

QString TemplatedTableHandler::definition() const noexcept
{
	return  QStringLiteral("create table ") + table_declaration + QStringLiteral(" ") + table_definition;
}

QString TemplatedTableHandler::declaration() const noexcept
{
	return  table_declaration;
}

QString TemplatedTableHandler::definition(const QString another_name) const noexcept
{
	return QStringLiteral("create table ") + another_name + QStringLiteral(" ") + table_definition;
}

QString TemplatedTableHandler::select_all() const noexcept
{
	return QStringLiteral("select ") + allFieldsDeclaration() + QStringLiteral(" from ") + table_declaration;
}

QString TemplatedTableHandler::select_all(const QString another_name) const noexcept
{
	return QStringLiteral("select ") + allFieldsDeclaration() + QStringLiteral(" from ") + another_name;
}

QString TemplatedTableHandler::select_filtered(const QString& str) const noexcept
{
	return select_all() + QStringLiteral(" where ") + str;
}

QString TemplatedTableHandler::select_filtered(const QString& filter, const QString another_name) const noexcept
{
	return select_all(another_name) + QStringLiteral(" where ") + filter;
}

QString TemplatedTableHandler::update(const QString& str) const noexcept
{
	return QStringLiteral("update ") + table_declaration + QStringLiteral(" ") + str;
}

QString TemplatedTableHandler::update(const QString& values, const QString another_name) const noexcept
{
	return QStringLiteral("update ") + another_name + QStringLiteral(" ") + values;
}

QString TemplatedTableHandler::delete_filtered(const QString& str) const noexcept
{
	return QStringLiteral("delete from ") + table_declaration + QStringLiteral(" ") + str;
}

QString TemplatedTableHandler::delete_filtered(const QString& filter, const QString another_name) const noexcept
{
	return QStringLiteral("delete from ") + another_name + QStringLiteral(" ") + filter;
}

QString TemplatedTableHandler::drop() const noexcept
{
	return query_templates::QueryTemplateTable[query_templates::DropTableQuery].arg(table_declaration);
}

QString TemplatedTableHandler::drop(const QString another_name) const noexcept
{
	return query_templates::QueryTemplateTable[query_templates::DropTableQuery].arg(another_name);
}

QString TemplatedTableHandler::allFieldsDeclaration() const noexcept
{
	return table_fields.join(" , ");
}

QString TemplatedTableHandler::insert(const QString& values) const noexcept
{
	return QStringLiteral("insert into ") + table_declaration + QStringLiteral(" (")
		+ allFieldsDeclaration() + QStringLiteral(") values ") + values;
}

QString TemplatedTableHandler::insert(const QString another_name, const QString & values) const noexcept
{
	return QStringLiteral("insert into ") + table_declaration + QStringLiteral(" (")
		+ allFieldsDeclaration() + QStringLiteral(") values ") + values;
}

TemplatedTableHandler TemplatedTableHandler::clone(QString newName)
{
	return TemplatedTableHandler(newName, table_declaration, table_fields);
}
