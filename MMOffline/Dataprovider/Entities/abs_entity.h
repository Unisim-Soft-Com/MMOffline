#pragma once
#include "Networking/Parsers/JsonUniresult.h"
#include "FieldTablesPredefines.h"
#include <memory>
#include <QtSql/qsqlquery.h>
#include <exception>
#include <qregularexpression.h>
#include <qmetatype.h>
#include <QVariant>

/*
		This file contains basic entity class, which is providing interfaces for easy storing,
	loading and uploading data entities. Inheriting it will allow to use vast set of tools,
	like data models, autosearch and autosort proxies, easy conversions to json elements and 
	access to dataprovider templates.

	Used pattern: prototype
*/

// Query pointer is used during initializations from database requests, such as SELECT *
typedef std::shared_ptr<QSqlQuery> QueryPtr;

// IdInt is used for storing id's. Use this type for compatibility with IdGenerator
typedef long long int IdInt;

class abs_entity
// Basic entity class is providing common interfaces for most operations
{
protected:
	// Use this id for defining which class is this. If you skip initialization of this - you will
	// lack some non-dynamic possibilities, but you will still be able to use RTTI
	int class_id;

	// This virtual method must return constructed JSON object. Skip it if you will not plan sending this
	virtual uniform_json_object_representation toJsonRepresentation() const = 0;
	// This method must return true if initialization with JSON was successfull. Skip it if you wont need 
	// json parsing
	virtual bool fromJsonRepr(const uniform_json_object_representation&) = 0;
	// This method must return TTH pointer to associated table. Skip it if you don't need database operations.
	// Never return nullptr here if your code calls to dataprovider.
	virtual TemplatedTableHandler* getAssocTable() const = 0;
	// This method must return INSERT string with all data you want to serialize or deserialize
	virtual QString getContentsForDb() const = 0;
	// This method must return copy of your object. Datamodels need this method working.
	virtual abs_entity* fabricate() const = 0;
	// This method must unpack query and return true if deserialization was successfull. 
	// It can be used as iterator like while (o->fromSql()) do something
	virtual bool fromSql(QueryPtr q) = 0;
	// This method is used for filtration. It is different from normal operator==() because
	// sometimes you want to see here exotical actions with data, like conversions etc. 
	virtual bool isLikeString(const QRegExp& qregexp)const = 0;
	// This method is used to obtain polymorthic ID. You must always return valid id, if you dont have one - 
	// return at least hash of used string
	virtual IdInt extractId()const = 0;
	// This method must implement deep check of an object, from type to full data identity.
	// Usually this can be implemented as   return (myType() == o->myType() && getId() == o->getId());
	virtual bool compare(abs_entity* another) const = 0;
public:
	//This constructor is allowing to skip class_id initialization because class_id operations
	//can be replaced with RTTI
	explicit abs_entity(int class_id = 0);		
	//These are non-virtual wrappers for interfaces 
	//Returns json representation 
	uniform_json_object_representation toUniJson() const;
	//constructs object form uniJson, returns true if successful
	bool fromUniJson(const uniform_json_object_representation&);
	//Constructs object from sql query, returns true if successful
	bool fromSqlQuery(QueryPtr q);
	//Returns associated table handler
	const TemplatedTableHandler* const getAssociatedTable() const;
	//Returns id of this object
	IdInt getId() const;
	//Returns header of insertion
	QString insertToDBHeader() const;
	//Returns values to insert into DB
	QString insertToDBValues() const;
	//Returns full insertion query, equal to call ->insert(insertToDBValues()) 
	QString insertionQuery() const;
	//Returns full insertion query for another table
	QString insertionQuery(const QString another_table) const;
	//Performs deep comparation between two polymorthic objects
	bool deepCompare(abs_entity*) const;
	//Returns type of this
	int myType() const;
	//Applies filter to this element, true if it fits
	bool filter(const QRegExp& qrexp) const;
	// returns polymorthic copy of this
	abs_entity* clone() const;
	// Returns copy of table with another name
	TemplatedTableHandler getRenamedTable(const QString& tname) const;
	//Deprecated - returns copy of associated table with name obtained from predefined names
	TemplatedTableHandler getRenamedTable(TableNames tname) const;
};

// Shared pointer to this is used as polymorthic object, so operations with DataEntity are very common
typedef std::shared_ptr<abs_entity> DataEntity;
// Can be stored in QVariant
Q_DECLARE_METATYPE(DataEntity);

class InitializationError : public std::exception
	// Initialization error is thrown when initialization is interrupted, but not denied
	// This is common when you fed query with wrong values
{
private:
	std::string msg = "Error initializing entity with supportive value ";
public:
	int counter;
	InitializationError(int ctr) : counter(ctr) { msg += QString::number(ctr).toStdString(); };
	virtual const char* what() const noexcept override { return msg.c_str(); };
};

// These formats must be used for serialization\deserialization of the data to avoid strange errors
extern const QString dateSerializationFormat;
extern const QString dateTimeSerializationFormat;
