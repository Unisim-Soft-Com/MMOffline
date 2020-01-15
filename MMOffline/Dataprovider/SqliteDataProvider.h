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
	void assertAndOpenSession();
	void assertAndCloseSession();
	bool loadEntity(QueryPtr q, const DataEntity);
public:
	SqliteDataProvider(QObject* parent = nullptr);
	~SqliteDataProvider();

	bool storeEntity(const DataEntity);
	bool doesTableExist(const TableNames table);
	bool doesTableExist(const QString sqlquery);
	bool dropTable(const TableNames);
	bool dropTable(const QString table);
	bool createTable(const TableNames);
	bool createTable(const QString);
	bool createTableFor(const DataEntity);
	bool createTableFor(const DataEntity, const QString& tname);
	bool pushData(const QVector<DataEntity>& data);
	bool pushData(const QVector<DataEntity>& data, QString table);
	bool replaceData(const DataEntity, const QString& tname = QString::null);
	bool removeOneEntity(const DataEntity, const QString& tname = QString::null);
	void dropEverything();
	QHash<IdInt, int> loadIdPairs(const QString& filter);
	QVector<DataEntity> loadDataFrom(const TableNames);
	QStringList loadDataFrom(const QString table);
	int countData(const QString& table);
	int assertId(const QString& query, const int id);
	void forcedCommit();

	template <class DataEnt>
	bool pushEntityList(const QVector<std::shared_ptr<DataEnt> >& elist, QString tname = QString::null)
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
	QVector<DataEntity> loadDataAs(const QString& filter = QString::null, QString table = QString::null)
	{
		assertAndOpenSession();
		QVector<DataEntity> toreturn;
		DataEnt temp;
		QueryPtr newQuery;
		if (filter.isNull())
		{
			newQuery = runQuery(temp.getAssociatedTable()->select_all(table));
		}
		else
		{
			newQuery = runQuery(temp.getAssociatedTable()->select_filtered(filter, table));
		}
		if (newQuery == nullptr)
		{
			assertAndCloseSession();
			return toreturn;
		}
		while (temp.fromSqlQuery(newQuery))
		{
			toreturn.push_back(DataEntity(temp.clone()));
		}
		assertAndCloseSession();
		return toreturn;
	}
	template <class DataEnt>
	QVector<std::shared_ptr<DataEnt>> loadEntities(
		const QString& filter = QString::null, const QString& tname = QString::null
	)
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
	bool recreateTable()
	{
		DataEnt d;
		assertAndOpenSession();
		if (mainDatabase.tables().contains(d.getAssociatedTable()->declaration()))
		{
			executeQuery(d.getAssociatedTable()->drop());
		}
		return executeQuery(d.getAssociatedTable()->definition());
	}

	template <class DataEnt>
	std::shared_ptr<DataEnt> loadEntityById(IdInt id, const QString another_name = QString::null)
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
	bool removeEntitiesFiltered(const QString filter, const QString another_name = QString::null)
	{
		DataEnt temp;
		return executeQuery(temp.getAssociatedTable()->delete_filtered(filter, another_name));
	}
	template <class DataEnt>
	QVector<std::shared_ptr<DataEnt> > loadEntitiesWithForcedQuery(const QString query)
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
