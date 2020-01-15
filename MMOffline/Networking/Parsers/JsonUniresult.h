#pragma once
#include <QStringList>
#include <QString>
#include <QJsonObject>
#include <QHash>

class uniform_json_object_representation
{
private:
	QStringList fieldNames;
	QHash<QString, QString> fields;
	int sizeOfThis;
public:
	uniform_json_object_representation();
	explicit uniform_json_object_representation(int sz);
	explicit uniform_json_object_representation(QStringList&);
	explicit uniform_json_object_representation(QJsonObject&);
	explicit uniform_json_object_representation(QStringList, QStringList);
	QString& operator[](int ind);
	QString& operator[](QLatin1String);
	QString& operator[](QString&);
	uniform_json_object_representation& operator<<(QString&);
	QString at(int ind) const;
	QString toString() const;
	int size() const;
	const QStringList& keys() const;
	const QString value(QString key) const;
	const QString value(QLatin1String) const;
	void setKeys(QStringList&);
	void setFields(QStringList&);
	void addField(QString key, QString val);
	QStringList mapValues(const QStringList& l, bool* ok) const;
	QStringList mapValues(const QStringList& l, const QStringList& defaults) const;
	QString makeTableDefinition() const;
	QString makeTableInsertion() const;
};

typedef uniform_json_object_representation JSUniresult;
typedef QVector<uniform_json_object_representation> JsUniresList;