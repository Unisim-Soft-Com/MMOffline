#include "SqliteDataProvider.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif

bool SqliteDataProvider::executeQuery(const QString& sqlquery)
{
	if (!isSessionOpened)
	{
		mainDatabase.open();
	}
	QSqlQuery q(mainDatabase);
	q.exec(sqlquery);
	if (q.lastError().isValid())
	{
#ifdef DEBUG
		detrace_METHPERROR("executeQuery", "error executing sql -> " << sqlquery << "<- " << q.lastError().text());
#endif
		mainDatabase.close();
		isSessionOpened = false;
		return false;
	}
	mainDatabase.close();
	isSessionOpened = false;
	mainDatabase.commit();
	return true;
}


bool SqliteDataProvider::executeWithinSession(const QString& sqlquery)
{
	if (!isSessionOpened)
	{
		mainDatabase.open();
		isSessionOpened = true;
	}
	QSqlQuery q(mainDatabase);
	q.exec(sqlquery);
	if (q.lastError().isValid())
	{
#ifdef DEBUG
		detrace_METHPERROR("executeQuery", "error executing sql -> " << sqlquery << "<- " << q.lastError().text());
#endif
		mainDatabase.close();
		return false;
	}
	mainDatabase.close();
	return true;
}


QueryPtr SqliteDataProvider::runQuery(const QString& sqlquery)
{
	if (!isSessionOpened)
	{
		mainDatabase.open();
		isSessionOpened = true;
	}
#ifdef DEBUG
	detrace_METHFRECALL("runQuery " << sqlquery);
#endif
	QueryPtr query(new QSqlQuery(mainDatabase));
	query->exec(sqlquery);
	if (query->lastError().isValid())
	{
#ifdef DEBUG
		detrace_METHPERROR("executeQuery", "error executing sql -> " << sqlquery << "<- " << query->lastError().text());
#endif
		mainDatabase.close();
		return QueryPtr(nullptr);
	}
	return query;
}


SqliteDataProvider::SqliteDataProvider(QObject * parent)
	: QObject(parent), mainDatabase(), isSessionOpened(false)
{
	mainDatabase = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), DATABASE_PATH);
	mainDatabase.setDatabaseName(DATABASE_NAME);
}

SqliteDataProvider::~SqliteDataProvider()
{
	QSqlDatabase::removeDatabase(DATABASE_NAME);

}

bool SqliteDataProvider::storeEntity(const DataEntity entity)
{
	return executeQuery(entity->insertionQuery());
}

bool SqliteDataProvider::loadEntity(QueryPtr q, const DataEntity entity)
{
	return entity->fromSqlQuery(q);
}

bool SqliteDataProvider::doesTableExist(const TableNames table)
{
	return mainDatabase.tables(QSql::Tables).contains(predefinedTables[table].declaration());
}

bool SqliteDataProvider::doesTableExist(const QString sqlquery)
{
	return mainDatabase.tables(QSql::Tables).contains(sqlquery);
}

bool SqliteDataProvider::dropTable(const TableNames tab)
{
	return executeQuery(predefinedTables[tab].drop());
}

bool SqliteDataProvider::dropTable(const QString table)
{
	return executeQuery(query_templates::QueryTemplateTable[query_templates::DropTableQuery].arg(table));
}

bool SqliteDataProvider::createTable(const TableNames tab)
{
	return executeQuery(predefinedTables[tab].definition());
}

bool SqliteDataProvider::createTable(const QString querydef)
{
	return executeQuery(QStringLiteral("create table ") + querydef);
}

bool SqliteDataProvider::createTableFor(const DataEntity entity)
{
	return executeQuery(entity->getAssociatedTable()->definition());
}

QVector<DataEntity> SqliteDataProvider::loadDataFrom(const TableNames table)
{
	QVector<DataEntity> toreturn;
	DataEntity temp(associateTableWithData[table]->clone());
	auto newQuery = runQuery(temp->getAssociatedTable()->select_all());
	if (newQuery == nullptr)
	{
#ifdef DEBUG
		detrace_METHPERROR("loadDataFrom " + QString::number(table)," returned null query");
#endif
		return toreturn;
	}
	while (temp->fromSqlQuery(newQuery))
	{
		toreturn.push_back(DataEntity(temp->clone()));
	}
	return toreturn;
}

QStringList SqliteDataProvider::loadDataFrom(const QString table)
{
	QStringList toreturn;
	auto newQuery = runQuery(QStringLiteral("select * from ") + table);
	if (newQuery == nullptr)
	{
#ifdef DEBUG
		detrace_METHPERROR("loadDataFrom " + table, "returned null query");
#endif
		return toreturn;
	}
	while (newQuery->next())
	{
		if (newQuery->value(0).canConvert(QMetaType::QString))
		{
			toreturn.push_back(newQuery->value(0).toString());
		}
	}
	return toreturn;
}

bool SqliteDataProvider::pushData(const QVector<DataEntity>& data)
{
	for (DataEntity entity : data)
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
