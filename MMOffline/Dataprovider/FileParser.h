#pragma once
#include <QtCore/QTextStream>
#include <QtCore/QString>
#include <QFile>
#include <QDir>
#include <QObject>
#include "Widgets/utils/ApplicationDataWorkset.h"

class CsvFileParser : protected QObject
{
	Q_OBJECT
private:
	QDir rootdir;
	QStringList errors;
	bool isReady;
	QChar separator;

	bool _assertElementsCount(QStringList*, int);
	template<class DataEnt>
	bool _getEntity(const QString path, int valuesPerElement, const QString tableName = QString::null)
	{
		QFile file(rootdir.absoluteFilePath(path));
		if (!file.open(QIODevice::ReadOnly))
			errors << tr("file can not be opened ") + path;
		QTextStream fin(&file);
		fin.setCodec("UTF-8");
		QString buffer;
		QStringList splitvalues;
		bool conversionResult = false;
		QVector<std::shared_ptr<DataEnt> > finalList;

		buffer = fin.readLine();
		splitvalues = buffer.split(separator);
		if (!_assertElementsCount(&splitvalues, valuesPerElement))
			return false;

		while (!fin.atEnd())
		{
			buffer = fin.readLine();
			splitvalues = buffer.split(separator);
			if (!_assertElementsCount(&splitvalues, valuesPerElement))
				return false;
			try
			{
				finalList << std::shared_ptr<DataEnt>(new DataEnt(splitvalues));
			}
			catch (InitializationError & ierror)
			{
				errors << tr("Impossible to convert value: ") + splitvalues.at(ierror.counter);
				return false;
			}
		}
		AppWorkset->dataprovider.pushEntityList<DataEnt>(finalList, tableName);
		return true;
	}

public:
	CsvFileParser(const QString path, QChar sep = QChar(';'));
	bool doParsing();
	QStringList errorList();
};
