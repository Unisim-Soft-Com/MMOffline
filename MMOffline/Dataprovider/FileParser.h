#pragma once
#include <QtCore/QTextStream>
#include <QtCore/QString>
#include <QFile>
#include <QDir>
#include <QObject>
#include "Dataprovider/SqliteDataProvider.h"

/*
	This file contains parser of CSV data used to fill inner tables. It inherits 
	QObject to use translations. 
*/

class CsvFileParser : protected QObject
	// Parses directory with CSV files to fill inner tables
{
	Q_OBJECT
private:
	QDir rootdir;		//	directory to parse
	QStringList errors;	//	list with errors
	bool isReady;		//	true if directory and files exists
	QChar separator;	//	separator in csv. For Windows Excel it usually is ;

	// checks quantity of elements in list
	bool _assertElementsCount(QStringList*, int);


	template<class DataEnt>
	bool _getEntity(const QString path, int valuesPerElement, const QString tableName = QString::null)
	// loads from path list of entities and pushes them into database
	{
		// pre-reading initialization
		QFile file(rootdir.absoluteFilePath(path));
		if (!file.open(QIODevice::ReadOnly))
		{
			errors << tr("file can not be opened ") + path;
			return false;
		}
		QTextStream fin(&file);
		fin.setCodec("UTF-8");
		QString buffer;			//	buffer for new line
		QStringList splitvalues;	//	splitted values suitable for initialization
		bool conversionResult = false;	//	true if item was converted without errors
		QVector<std::shared_ptr<DataEnt> > finalList;

		// read and split line before the cycle to get rid of the header of the table
		buffer = fin.readLine();
		splitvalues = buffer.split(separator);
		if (!_assertElementsCount(&splitvalues, valuesPerElement))
			return false;


		while (!fin.atEnd())
			// main cycle
		{
			// read new line and split it
			buffer = fin.readLine();
			splitvalues = buffer.split(separator);
			if (!_assertElementsCount(&splitvalues, valuesPerElement))
				return false;
			try
			{
				// try to initialize new entity, if ok - save it to final list
				finalList << std::shared_ptr<DataEnt>(new DataEnt(splitvalues));
			}
			catch (InitializationError & ierror)
			{
				errors << tr("Impossible to convert value: ") + splitvalues.at(ierror.counter);
				return false;
			}
		}
		// push all list
		AppData->pushEntityList<DataEnt>(finalList, tableName);
		return true;
	}

public:
	CsvFileParser(const QString path, QChar sep = QChar(';'));
	// launches parsing
	bool doParsing();
	QStringList errorList();
};
