#pragma once
#include <QtSql/QSqlDatabase>
#include <QObject>
#include "DataEntities.h"
#include "IdGenerator.h"

/*
	This file contains data provider object. It is used for incapsulation, to hide database access under
	template methods for polymorthic access to entities. It allows to use abstract interfaces of data entities for easy 
	pushing and loading data.
*/

#define AppData SqliteDataProvider::instanse()


const QString DATABASE_PATH(QStringLiteral("maindb.db"));
const QString DATABASE_NAME(QStringLiteral("MainDB"));

// pointer to query is used for saving query during initialization of new entity
typedef std::shared_ptr<QSqlQuery> QueryPtr;

class SqliteDataProvider : public QObject
	// manages database access
{
private:
	QSqlDatabase mainDatabase;	//	inner database
	bool isSessionOpened;		//	true if session is opened

	static SqliteDataProvider* _instanse;

	// executes query without returning query
	bool executeQuery(const QString& sqlquery);
	// executes query without checking session bounds and without returning query. Fastest.
	bool executeWithinSession(const QString& sqlquery);
	// executes query and returns query pointer for future usage
	QueryPtr runQuery(const QString& sqlquery);
	// check isSessionOpened and open session if necessary
	void assertAndOpenSession();
	// check isSessionOpened and close session if necessary
	void assertAndCloseSession();
	// renundant wrapper for loading data entity
	bool loadEntity(QueryPtr q, const DataEntity);
public:
	explicit SqliteDataProvider(QObject* parent = nullptr);
	~SqliteDataProvider();

	static SqliteDataProvider* instanse();

	// pushes one entity to database using it's abstract interfaces
	bool storeEntity(const DataEntity);
	// check if table exists by it's number. Leaves session open.
	bool doesTableExist(const TableNames table);
	// check if table exists by it's name. Leaves session open
	bool doesTableExist(const QString sqlquery);
	// drops table by it's number
	bool dropTable(const TableNames);
	// drops table by it's name
	bool dropTable(const QString table);
	// creates table and index for it using it's number
	bool createTable(const TableNames);
	// creates table without index by full definition
	bool createTable(const QString);
	// creates default table for data entity
	bool createTableFor(const  DataEntity);
	// creates named table for entity
	bool createTableFor(const  DataEntity, const QString& tname);
	// pushes vector with polymorthic data into database (default table)
	bool pushData(const QVector<DataEntity>& data);
	// pushes vector with polymorthic data into database in other table
	bool pushData(const QVector<DataEntity>& data, QString table);
	// replaces data in the table with simple replace. Be sure that you has primary key.
	bool replaceData(const DataEntity, const QString& tname = QString::null);
	// deletes one entity with binding to it's primary key. Be sure that there is primary key.
	bool removeOneEntity(const DataEntity, const QString& tname = QString::null);
	// performs full database drop
	void dropEverything();
	// loads id pairs using custom query. This is very usefull for connection establishing
	QHash<IdInt, int> loadIdPairs(const QString& filter);
	// loads data from particular default table
	QVector<DataEntity> loadDataFrom(const TableNames);
	// performs simpliest select * on table and returns list with results. 
	QStringList loadDataFrom(const QString table);
	// counts all rows in table
	int countData(const QString& table);
	// renundant id check
	int assertId(const QString& query, const int id);
	// performs forced .commit()
	void forcedCommit();
	// deletes entities from table suited for particular entity
	bool removeEntitiesFiltered(DataEntity prototype, const QString filter, const QString another_name = QString::null);
	// deletes, then creates table for entity
	bool recreateTable(DataEntity prototype);
	// loads data into polymorthic vector
	QVector<DataEntity> loadDataAs(DataEntity, const QString& filter = QString::null, QString table = QString::null);

	// Next methods are templated loaders from database. You can load data without this templating,
	// using abstract interfaces, then upcast them manually, but to avoid such actions - use these templates


	template <class DataEnt>
	bool pushEntityList(const QVector<std::shared_ptr<DataEnt> >& elist, QString tname = QString::null)
		// pushes list of not downcasted entities
	{
		if (elist.isEmpty()) return true;
		assertAndOpenSession();
		if (tname.isNull())
		{
			tname = elist.first()->getAssociatedTable()->declaration();
		}
		if (!doesTableExist(tname))
		{
			if (!createTableFor(elist.first(), tname)) return false;
		}
		for (const std::shared_ptr<DataEnt> entity : elist)
		{
			if (!executeWithinSession(entity->insertionQuery(tname)))
			{
				assertAndCloseSession();
				return false;
			}
		}
		assertAndCloseSession();
		return true;
	}
	
	template <class DataEnt>
	QVector<std::shared_ptr<DataEnt>> loadEntities(
		const QString& filter = QString::null, const QString& tname = QString::null
	)
		// loads upcasted entities from database
	{
		assertAndOpenSession();
		QVector<std::shared_ptr<DataEnt>> toreturn;
		DataEnt temp;
		QueryPtr newQuery;
		if (filter.isNull())
		{
			newQuery = runQuery(temp.getAssociatedTable()->select_all(tname));
		}
		else
		{
			newQuery = runQuery(temp.getAssociatedTable()->select_filtered(filter, tname));
		}
		if (newQuery == nullptr)
		{
			assertAndCloseSession();
			return toreturn;
		}
		while (temp.fromSqlQuery(newQuery))
		{
			toreturn.push_back(std::shared_ptr<DataEnt>((static_cast<DataEnt*>(temp.clone()))));
		}
		assertAndCloseSession();
		return toreturn;
	}

	template <class DataEnt>
	std::shared_ptr<DataEnt> loadEntityById(IdInt id, const QString another_name = QString::null)
		// loads one entity using it's primary key
	{
		assertAndOpenSession();
		std::shared_ptr<DataEnt> temp(new DataEnt());
		auto newQuery = runQuery(
			temp->getAssociatedTable()->select_by_primary_key(QString::number(id), another_name));
		if (newQuery == nullptr)
			return std::shared_ptr<DataEnt>(nullptr);
		temp->fromSqlQuery(newQuery);
		assertAndCloseSession();
		return temp;
	}

	template <class DataEnt>
	std::shared_ptr<DataEnt> loadEntityAs(const QString& filter, const QString& tname = QString::null)
		// loads one entity using filter
	{
		assertAndOpenSession();
		std::shared_ptr<DataEnt> temp(new DataEnt());
		auto newQuery = runQuery(
			temp->getAssociatedTable()->select_filtered(filter, tname));
		if (newQuery == nullptr)
		{
			assertAndCloseSession();
			return temp;
		}
		try
		{
			temp->fromSqlQuery(newQuery);
		}
		catch (InitializationError & ie)
		{
			temp.reset(new DataEnt());
		}
		assertAndCloseSession();
		return temp;
	}

	
	template <class DataEnt>
	QVector<std::shared_ptr<DataEnt> > loadEntitiesWithForcedQuery(const QString query)
		// loads upcasted entities with full query provided
	{
		assertAndOpenSession();
		QVector<std::shared_ptr<DataEnt>> toreturn;
		DataEnt temp;
		QueryPtr newQuery;
		newQuery = runQuery(query);
		if (newQuery == nullptr)
		{
			assertAndCloseSession();
			return toreturn;
		}
		while (temp.fromSqlQuery(newQuery))
		{
			toreturn.push_back(std::shared_ptr<DataEnt>((static_cast<DataEnt*>(temp.clone()))));
		}
		assertAndCloseSession();
		return toreturn;
	}
};
