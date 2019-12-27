#include "SqliteDataProvider.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include "qapplication.h"
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include <QtSql/qsqldatabase.h>

bool SqliteDataProvider::executeQuery(const QString& sqlquery)
{
	if (!isSessionOpened)
	{
		mainDatabase.open(); isSessionOpened = true;
	}
	QSqlQuery q(mainDatabase);

#ifdef DEBUG
	//detrace_METHFRECALL("runQuery " << sqlquery);
#endif
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
	qApp->processEvents();
	return true;
}


bool SqliteDataProvider::executeWithinSession(const QString& sqlquery)
{
	if (!isSessionOpened)
	{
		mainDatabase.open(); isSessionOpened = true;
		isSessionOpened = true;
	}
	QSqlQuery q(mainDatabase);

#ifdef DEBUG
	//detrace_METHFRECALL("runQuery " << sqlquery);
#endif
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
	mainDatabase.commit();
	return true;
}


QueryPtr SqliteDataProvider::runQuery(const QString& sqlquery)
{
	if (!isSessionOpened)
	{
		mainDatabase.open(); isSessionOpened = true;
		isSessionOpened = true;
	}
#ifdef DEBUG
	//detrace_METHFRECALL("runQuery " << sqlquery);
#endif
	QueryPtr query(new QSqlQuery(mainDatabase));
	query->exec(sqlquery);
	if (query->lastError().isValid())
	{
#ifdef DEBUG
		detrace_METHPERROR("executeQuery", "error executing sql -> " << sqlquery << "<- " << query->lastError().text());
#endif
		query->clear();
		mainDatabase.close();
		isSessionOpened = false;
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
	mainDatabase;
}

bool SqliteDataProvider::storeEntity(const DataEntity entity)
{
	if (!isSessionOpened)
	{
		mainDatabase.open(); isSessionOpened = true;
	}
	if (!doesTableExist(entity->getAssociatedTable()->declaration()))
	{
		if (!createTableFor(entity)) return false;
	}
	if (!executeWithinSession(entity->insertionQuery()))
	{
		mainDatabase.close();
		isSessionOpened = false;
		return false;
	}
	return true;
}

bool SqliteDataProvider::loadEntity(QueryPtr q, const DataEntity entity)
{
	return entity->fromSqlQuery(q);
}

bool SqliteDataProvider::doesTableExist(const TableNames table)
{
	if (!isSessionOpened)
	{
		mainDatabase.open();
		isSessionOpened = true;
	}
	return mainDatabase.tables(QSql::Tables).contains(predefinedTables[table].declaration());
}

bool SqliteDataProvider::doesTableExist(const QString sqlquery)
{
	if (!isSessionOpened)
	{
		mainDatabase.open();
		isSessionOpened = true;
	}
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

bool SqliteDataProvider::createTableFor(const DataEntity entity, const QString& tname)
{
	return executeQuery(entity->getAssociatedTable()->definition(tname));
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

int SqliteDataProvider::assertId(const QString& query, const int id)
{
	auto newQuery = runQuery(query.arg(id));
	if (newQuery == nullptr)
		return id;
	if (!newQuery->next())
	{
		return id;
	}
	return assertId(query, rand());
}

void SqliteDataProvider::forcedCommit()
{
	mainDatabase.open(); 
	mainDatabase.commit();
	mainDatabase.close();
	isSessionOpened = false;
}

bool SqliteDataProvider::pushData(const QVector<DataEntity>& data)
{
	mainDatabase.open(); isSessionOpened = true;
	for (DataEntity entity : data)
	{
		if (!doesTableExist(entity->getAssociatedTable()->declaration()))
		{
			if (!createTableFor(entity)) return false;
		}
		if (!executeWithinSession(entity->insertionQuery()))
		{
			mainDatabase.close();
			isSessionOpened = false;
			return false;
		}
	}
	mainDatabase.commit();
	if (isSessionOpened)
	{
		mainDatabase.close();
		isSessionOpened = false;
	}
	return true;
}

bool SqliteDataProvider::pushData(const QVector<DataEntity>& data, QString& table)
{
	mainDatabase.open(); isSessionOpened = true;
	for (DataEntity entity : data)
	{
		if (!doesTableExist(table))
		{
			if (!createTableFor(entity, table)) return false;
		}
		if (!executeWithinSession(entity->insertionQuery(table)))
		{
			mainDatabase.close();
			isSessionOpened = false;
			return false;
		}
	}
	mainDatabase.commit();
	if (isSessionOpened)
	{
		mainDatabase.close();
		isSessionOpened = false;
	}
	
	return true;
}

void SqliteDataProvider::dropEverything()
{
	mainDatabase.open(); isSessionOpened = true;
	for (QString& table : mainDatabase.tables())
	{
		dropTable(table);
	}
}

QHash<IdInt, int> SqliteDataProvider::loadIdPairs(const QString& filter)
{
	QHash<IdInt, int> temp;
	mainDatabase.open(); isSessionOpened = true;
	auto q = runQuery(filter);
	bool ok;
	int buffer, buffer2;
	if (q == nullptr)
		return temp;
	while (q->next())
	{
		if (q->record().count() != 2)
			continue;
		buffer = q->value(0).toLongLong(&ok);
		if (!ok) continue;
		buffer2 = q->value(1).toInt(&ok);
		if (!ok) continue;
		temp[buffer] = buffer2;
		//detrace_METHEXPL("Loaded data: " << QString::number(buffer) << " " << QString::number(buffer2) << "\n");
	}
	return temp;
}
