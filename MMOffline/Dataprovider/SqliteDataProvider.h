#pragma once
#include <QtSql/QSqlDatabase>
#include <QObject>
#include "DataEntities.h"
#include "IdGenerator.h"
const QString DATABASE_PATH(QStringLiteral("maindb.db"));
const QString DATABASE_NAME(QStringLiteral("MainDB"));

typedef std::shared_ptr<QSqlQuery> QueryPtr;

class SqliteDataProvider : public QObject
{
private:
	QSqlDatabase mainDatabase;
	bool isSessionOpened;


	bool executeQuery(const QString& sqlquery);
	bool executeWithinSession(const QString& sqlquery);
	QueryPtr runQuery(const QString& sqlquery);

public:
	SqliteDataProvider(QObject * parent = nullptr);
	~SqliteDataProvider();


	bool storeEntity(const DataEntity);
	bool loadEntity(QueryPtr q,  const DataEntity);
	bool doesTableExist(const TableNames table);
	bool doesTableExist(const QString sqlquery);
	bool dropTable(const TableNames);
	bool dropTable(const QString table);
	bool createTable(const TableNames);
	bool createTable(const QString);
	bool createTableFor(const DataEntity);
	bool createTableFor(const DataEntity, const QString& tname);
	bool pushData(const QVector<DataEntity>& data);
	bool pushData(const QVector<DataEntity>& data, QString& table);
	void dropEverything();
	QHash<IdInt, int> loadIdPairs( const QString& filter);
	QVector<DataEntity> loadDataFrom(const TableNames);
	QStringList loadDataFrom(const QString table);
	int assertId(const QString& query, const int id);
	void forcedCommit();

	template <class DataEnt>
	bool pushEntityList(const QVector<std::shared_ptr<DataEnt> >& elist)
	{
		mainDatabase.open();
		for (const std::shared_ptr<DataEnt> entity : elist)
		{
			if (!doesTableExist(entity->getAssociatedTable()->declaration()))
			{
				if (!createTableFor(entity)) return false;
			}
			if (!executeWithinSession(entity->insertionQuery()))
			{
				mainDatabase.close();
				return false;
			}
		}
		if (isSessionOpened)
			mainDatabase.close();
		return true;
	}
	template <class DataEnt>
	bool pushEntityList(const QVector<std::shared_ptr<DataEnt> >& elist, QString tname)
	{
		mainDatabase.open();
		for (const std::shared_ptr<DataEnt> entity : elist)
		{
			if (!doesTableExist(tname))
			{
				if (!createTableFor(entity, tname)) return false;
			}
			if (!executeWithinSession(entity->insertionQuery(tname)))
			{
				mainDatabase.close();
				return false;
			}
		}
		if (isSessionOpened)
			mainDatabase.close();
		return true;
	}
	template <class DataEnt>
	QVector<DataEntity> loadDataAs()
	{
		QVector<DataEntity> toreturn;
		DataEnt temp;
		QueryPtr newQuery = runQuery(temp.getAssociatedTable()->select_all());
		if (newQuery == nullptr)
			return toreturn;
		while (temp.fromSqlQuery(newQuery))
		{
			toreturn.push_back(DataEntity(temp.clone()));
		}
		return toreturn;
	}
	template <class DataEnt>
	QVector<std::shared_ptr<DataEnt>> loadEntities()
	{
		QVector<std::shared_ptr<DataEnt>> toreturn;
		DataEnt temp;
		QueryPtr newQuery = runQuery(temp.getAssociatedTable()->select_all());
		if (newQuery == nullptr)
			return toreturn;
		while (temp.fromSqlQuery(newQuery))
		{
			toreturn.push_back(std::shared_ptr<DataEnt>((static_cast<DataEnt*>(temp.clone()))));
		}
		return toreturn;
	}
	template<class DataEnt>
	QVector<std::shared_ptr<DataEnt>> loadEntitiesFromTable(QString name)
	{
		QVector<std::shared_ptr<DataEnt>> toreturn;
		DataEnt temp;
		QueryPtr newQuery = runQuery(temp.getAssociatedTable()->select_all(name));
		if (newQuery == nullptr)
			return toreturn;
		while (temp.fromSqlQuery(newQuery))
		{
			toreturn.push_back(std::shared_ptr<DataEnt>((static_cast<DataEnt*>(temp.clone()))));
		}
		return toreturn;
	}
	template <class DataEnt>
	QVector<DataEntity> loadDataFromTableAs(QString tname)
	{
		DataEnt temp;
		QVector<DataEntity> toreturn;

		auto newQuery = runQuery(temp.getAssociatedTable()->select_all(tname));
		if (newQuery == nullptr)
			return toreturn;
		while (temp.fromSqlQuery(newQuery))
		{
			toreturn.push_back(DataEntity(temp.clone()));
		}
		return toreturn;
	}
	template <class DataEnt>
	QVector<DataEntity> loadDataWithFieldFilter(const QString& filter)
	{
		DataEnt temp;
		QVector<DataEntity> toreturn;
		auto newQuery = runQuery(temp.getAssociatedTable()->select_filtered(filter));
		if (newQuery == nullptr)
			return toreturn;
		while (temp.fromSqlQuery(newQuery))
		{
			toreturn.push_back(DataEntity(temp.clone()));
		}
		return toreturn;
	}
	template <class DataEnt>
	bool recreateTable()
	{
		DataEnt d;
		mainDatabase.open();
		if (mainDatabase.tables().contains(d.getAssociatedTable()->declaration()))
		{
			executeQuery(d.getAssociatedTable()->drop());
		}
		return executeQuery(d.getAssociatedTable()->definition());
	}
	
};