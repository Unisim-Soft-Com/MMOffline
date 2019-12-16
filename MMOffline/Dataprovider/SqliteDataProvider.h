#pragma once
#include <QtSql/QSqlDatabase>
#include <QObject>
#include "DataEntities.h"

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
	bool pushData(const QVector<DataEntity>& data);
	QVector<DataEntity> loadDataFrom(const TableNames);
	QStringList loadDataFrom(const QString table);



	template <class DataEnt>
	QVector<DataEntity> loadDataAs()
	{
		QVector<DataEntity> toreturn;
		DataEnt temp;
		QueryPtr newQuery = runQuery(temp.getAssociatedTable()->select_all());
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
		while (temp.fromSqlQuery(newQuery))
		{
			toreturn.push_back(DataEntity(temp.clone()));
		}
		return toreturn;
	}
	
	
};