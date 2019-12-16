#pragma once
#include "Networking/Parsers/JsonUniresult.h"
#include "FieldTablesPredefines.h"
#include <memory>
#include <QtSql/qsqlquery.h>
#include <exception>



typedef std::shared_ptr<QSqlQuery> QueryPtr;



class abs_entity
{
protected:
	int class_id;

	virtual uniform_json_object_representation toJsonRepresentation() const = 0;
	virtual bool fromJsonRepr(const uniform_json_object_representation&) = 0;
	virtual TemplatedTableHandler* getAssocTable() const = 0;
	virtual QString getContentsForDb() const = 0;
	virtual abs_entity* fabricate() const = 0;
	virtual bool fromSql(QueryPtr q) = 0;
public:
	abs_entity(int class_id = 0);
	uniform_json_object_representation toUniJson() const;
	bool fromUniJson(const uniform_json_object_representation&);
	bool fromSqlQuery(QueryPtr q);
	const TemplatedTableHandler* const getAssociatedTable() const;
	TemplatedTableHandler getRenamedTable(const QString& tname) const;
	TemplatedTableHandler getRenamedTable(TableNames tname) const;
	QString insertToDBHeader() const;
	QString insertToDBValues() const;
	QString insertionQuery() const;
	QString insertionQuery(const QString another_table) const;
	int myType() const;
	abs_entity* clone() const;
};


class InitializationError : public std::exception
{
private:
	std::string msg = "Error initializing entity with supportive value ";
public:
	int counter;
	InitializationError(int ctr) : counter(ctr) { msg += QString::number(ctr).toStdString(); };
	virtual const char* what() const override { return msg.c_str(); };
};

