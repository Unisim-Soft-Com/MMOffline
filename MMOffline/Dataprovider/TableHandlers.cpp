#include "TableHandlers.h"

namespace query_templates
{
	const QString QueryTemplateTable[NotAQuery]
	{
		QStringLiteral("drop table %1")
	};
}




const QString& TemplatedTableHandler::assertAnotherName(const QString& another_name) const noexcept
{
	return (another_name.isNull()) ? table_declaration : another_name;
}

TemplatedTableHandler::TemplatedTableHandler()
	: table_definition(), table_declaration(), table_fields(), primaryKeyField(-1), table_type(0)
{
}

TemplatedTableHandler::TemplatedTableHandler(QString t_decl, QString t_def, QStringList tfields, int primarykey)
	: table_definition(t_def), table_declaration(t_decl), table_fields(tfields), primaryKeyField(primarykey), 
	table_type(1)
{
}

QString TemplatedTableHandler::declaration() const noexcept
{
	return  table_declaration;
}

QString TemplatedTableHandler::definition(const QString another_name) const noexcept
{
	const QString& name = assertAnotherName(another_name);
	return QStringLiteral("create table ") + name + QStringLiteral(" ") + table_definition;
}

QString TemplatedTableHandler::select_all(const QString another_name) const noexcept
{
	const QString& name = assertAnotherName(another_name);
	return QStringLiteral("select ") + allFieldsDeclaration() + QStringLiteral(" from ") + name;
}

QString TemplatedTableHandler::select_filtered(const QString& filter, const QString another_name) const noexcept
{
	const QString& name = assertAnotherName(another_name);
	return select_all(name) + QStringLiteral(" where ") + filter;
}

QString TemplatedTableHandler::select_by_primary_key(const QString pkeyvalue, const QString another_name) const noexcept
{
	if (primaryKeyField == -1)
		return QString::null;
	const QString& name = assertAnotherName(another_name);
	return select_all(name) + QStringLiteral(" where ") + table_fields.at(primaryKeyField) 
		+ QStringLiteral(" = ") + pkeyvalue;
}


QString TemplatedTableHandler::update(const QString& values, const QString another_name) const noexcept
{
	const QString& name = assertAnotherName(another_name);
	return QStringLiteral("update ") + name + QStringLiteral(" ") + values;
}

QString TemplatedTableHandler::replace(const QString& values, const QString& another_name) const noexcept
{
	const QString& name = (another_name.isNull()) ? table_declaration : another_name;
	return QStringLiteral("REPLACE INTO ") + name + QStringLiteral(" ( ") 
		+ allFieldsDeclaration() + QStringLiteral(" ) VALUES ") + values;
}

QString TemplatedTableHandler::delete_filtered(const QString& filter, const QString another_name) const noexcept
{
	const QString& name = assertAnotherName(another_name);
	return QStringLiteral("delete from ") + name + QStringLiteral(" where ") + filter;
}

QString TemplatedTableHandler::delete_by_primary_key(const QString& pkeyvalue, const QString another_name) const noexcept
{
	if (primaryKeyField == -1)
		return QString::null;
	const QString& name = assertAnotherName(another_name);
	return QStringLiteral("DELETE FROM ") + name + QStringLiteral(" where ")
		+ table_fields.at(primaryKeyField) + " = " + pkeyvalue;
}

QString TemplatedTableHandler::drop(const QString another_name) const noexcept
{
	const QString& name = assertAnotherName(another_name);
	return query_templates::QueryTemplateTable[query_templates::DropTableQuery].arg(another_name);
}

QString TemplatedTableHandler::allFieldsDeclaration() const noexcept
{
	return table_fields.join(" , ");
}

QString TemplatedTableHandler::insert( const QString & values, const QString another_name) const noexcept
{
	const QString& name = assertAnotherName(another_name);
	return QStringLiteral("insert into ") + name + QStringLiteral(" (")
		+ allFieldsDeclaration() + QStringLiteral(") values ") + values;
}

QString TemplatedTableHandler::makeIndex(const QString& another_name) const noexcept
{
	if (primaryKeyField == -1)
		return QString::null;
	const QString& name = assertAnotherName(another_name);
	return QStringLiteral("CREATE INDEX ") 	+ name + QStringLiteral("_index ON ") 
		+ name + "(" + table_fields.at(primaryKeyField) + ")";
}

TemplatedTableHandler TemplatedTableHandler::clone(QString newName)
{
	return TemplatedTableHandler(newName, table_declaration, table_fields, primaryKeyField);
}
