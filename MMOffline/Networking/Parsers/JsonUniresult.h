#pragma once
#include <QStringList>
#include <QString>
#include <QJsonObject>
#include <QHash>

/*
	This file contains definition of uniform json representation class. This class is used 
	for string representation of the json object without connection to json document. This class
	is renundant, but it still is usefull due to set of methods.
*/
class uniform_json_object_representation
	// represents json object without document connection
{
private:
	QStringList fieldNames;	//	names of the fields   ->"name" : "abc"
	QHash<QString, QString> fields; // values of the fields "name" : "abc"<-
	int sizeOfThis;			//	size of the object in fields
public:
	uniform_json_object_representation();
	explicit uniform_json_object_representation(int sz);
	explicit uniform_json_object_representation(QStringList&);
	explicit uniform_json_object_representation(QJsonObject&);
	explicit uniform_json_object_representation(QStringList, QStringList);
	// normal indexation operators, are allowing quick access to the contents with any type of key
	// by number of the field
	QString& operator[](int ind);
	//by name of the field
	QString& operator[](QLatin1String);
	QString& operator[](QString&);
	// inserts field into object
	uniform_json_object_representation& operator<<(QString&);
	// indexation operator like in array
	QString at(int ind) const;
	// joins object, returning json string
	QString toString() const;
	// number of fields in the object
	int size() const;
	// returns field names
	const QStringList& keys() const;
	// access operator like in map
	const QString value(QString key) const;
	const QString value(QLatin1String) const;
	// resets objects, adding keys
	void setKeys(QStringList&);
	// resets fields with new values
	void setFields(QStringList&);
	// adds field to the end of the list
	void addField(QString key, QString val);
	// maps object fields to provided field names
	QStringList mapValues(const QStringList& l, bool* ok) const;
	// maps object fields to provided names, replacing missing fields with the defaults
	QStringList mapValues(const QStringList& l, const QStringList& defaults) const;
	// creates field definition to store this object
	QString makeTableDefinition() const;
	// creates insertion query, allowing to store this object
	QString makeTableInsertion() const;
};

typedef uniform_json_object_representation JSUniresult;
typedef QVector<uniform_json_object_representation> JsUniresList;