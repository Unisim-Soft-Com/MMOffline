#pragma once
#include "Networking/Parsers/JsonUniresult.h"
#include "TableHandlers.h"
#include <memory>
#include <QtSql/qsqlquery.h>


const int PREDEFINED_TABLES_QUANTITY = 1;

typedef std::shared_ptr<QSqlQuery> QueryPtr;

extern TemplatedTableHandler predefinedTables[PREDEFINED_TABLES_QUANTITY]; 
class abs_entity
{
protected:
	int class_id;

	virtual uniform_json_object_representation toJsonRepresentation() const = 0;
	virtual bool fromJsonRepr(const uniform_json_object_representation &) = 0;
	virtual TemplatedTableHandler* getAssocTable() const = 0;
	virtual QString getContentsForDb() const = 0;
	virtual abs_entity* fabricate() const = 0;
	virtual bool fromSql(QueryPtr q) = 0;
public:
	abs_entity(int class_id = 0);
	uniform_json_object_representation toUniJson() const;
	bool fromUniJson(const uniform_json_object_representation&);
	bool fromSqlQuery(QueryPtr q);
	const TemplatedTableHandler * const getAssociatedTable() const;
	TemplatedTableHandler getRenamedTable(const QString & tname) const;
	TemplatedTableHandler getRenamedTable(TableNames tname) const;
	QString insertToDBHeader() const;
	QString insertToDBValues() const;
	QString insertionQuery() const;
	QString insertionQuery(const QString another_table) const;
	int myType() const;
	abs_entity* clone() const;
};


extern const abs_entity * const associateTableWithData[PREDEFINED_TABLES_QUANTITY];
typedef std::shared_ptr<abs_entity> DataEntity;

class ClientEntity : public abs_entity
{
public:
	int id;
	QString name;
protected:
	// Inherited via abs_entity
	virtual uniform_json_object_representation toJsonRepresentation() const override;
	virtual bool fromJsonRepr(const uniform_json_object_representation&) override;
	virtual bool fromSql(QueryPtr q) override;
	virtual TemplatedTableHandler* getAssocTable() const override;
	virtual QString getContentsForDb() const override;
	virtual abs_entity* fabricate() const override;
public:
	ClientEntity();
	ClientEntity(int id, QString name);

	// Inherited via abs_entity
};
