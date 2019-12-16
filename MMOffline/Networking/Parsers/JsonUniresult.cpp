#include "JsonUniresult.h"
#include <QTextStream>


QString joinFields(const QHash<QString, QString>& h, const QStringList& l, const char* delimiter)
{
	if (l.isEmpty())
		return QString();
	QString result = l.at(0);
	for (int i = 1; i < l.count(); ++i)
	{
		result += delimiter + h.value(l.at(i));
	}
	return result;
}


QString NotAResult(QString::null);
uniform_json_object_representation::uniform_json_object_representation()
	:fieldNames(), fields(), sizeOfThis(0)
{
}

uniform_json_object_representation::uniform_json_object_representation(int sz)
	: fieldNames(), fields(), sizeOfThis(sz)
{
	for (int i = 0; i < sz; ++i)
	{
		fieldNames << "";
	}
	fields.insert("", "");
}

uniform_json_object_representation::uniform_json_object_representation(QStringList& l)
	: fieldNames(l), fields(), sizeOfThis(l.count())
{
	for (int i = 0; i < sizeOfThis; ++i)
	{
		fields.insert(fieldNames.at(i), "");
	}
}

uniform_json_object_representation::uniform_json_object_representation(QJsonObject& obj)
	: fieldNames(obj.keys()), fields(), sizeOfThis(obj.count())
{
	QJsonValue val;
	for (QString& key : fieldNames)
	{
		val = obj.value(key);
		switch (val.type())
		{
		case QJsonValue::String:
			fields.insert(key,val.toString());
			break;
		case QJsonValue::Type::Double:
			fields.insert(key, QString::number(val.toDouble()));
			break;
		case QJsonValue::Bool:
			fields.insert(key, ((val.toBool()) ? QStringLiteral("true") : QStringLiteral("false")));
			break;
		}
	}
}

uniform_json_object_representation::uniform_json_object_representation(QStringList fn, QStringList f)
	: fieldNames(fn), fields() ,sizeOfThis(fn.count())
{
	int min = std::min(fn.count(), f.count());
	for (int i = 0; i < min; ++i)
	{
		fields.insert(fn.at(i), f.at(i));
	}
}

QString& uniform_json_object_representation::operator[](int ind)
{
	return fields[fieldNames.at(ind)];
}

QString& uniform_json_object_representation::operator[](QLatin1String key)
{
	return fields[key];
}

QString& uniform_json_object_representation::operator[](QString& key)
{
	return fields[key];
}

uniform_json_object_representation& uniform_json_object_representation::operator<<(QString& nval)
{
	for (int i = 0; i < sizeOfThis; ++i)
	{
		if (fields.value(fieldNames.at(i)).isEmpty())
		{
			fields[fieldNames.at(i)] = nval;
			return *this;
		}
	}
	return *this;
}

QString uniform_json_object_representation::at(int ind) const
{
	return fields.value(fieldNames.at(ind));
}

QString uniform_json_object_representation::toString() const
{
	QString temp;
	QTextStream cin(&temp);
	cin << "{";
	for (int i = 0; i < fieldNames.count(); ++i)
	{
		cin << "\"" << fieldNames.at(i) << "\":\"" << fields.value(fieldNames.at(i)) << "\"," << endl;
	}
	cin << "{" << endl;
	return temp;
}

int uniform_json_object_representation::size() const
{
	return sizeOfThis;
}

const QStringList& uniform_json_object_representation::keys() const
{
	return fieldNames;
}

const QString uniform_json_object_representation::value(QString key)const
{
	return fields.value(key);
}

const QString uniform_json_object_representation::value(QLatin1String key) const
{
	return fields.value(key);
}

void uniform_json_object_representation::setKeys(QStringList& l)
{
	fieldNames.clear();
	fields.clear();
	fieldNames << l;
	sizeOfThis = l.count();
	for (int i = 0; i < sizeOfThis; ++i)
	{
		fields.insert(l.at(i), "");
	}
}

void uniform_json_object_representation::setFields(QStringList& l)
{
	if (l.count() < sizeOfThis)
		return;
	fields.clear();
	for (int i = 0; i < sizeOfThis; ++i)
	{
		fields.insert(fieldNames.at(i), l.at(i));
	}
}

void uniform_json_object_representation::addField(QString key, QString val)
{
	fieldNames << key;
	fields.insert(key, val);
	++sizeOfThis;
}

QStringList uniform_json_object_representation::mapValues(const QStringList& l, bool* ok) const
{
	QStringList mapped;
	for (const QString& key : l)
	{
		if (fields.contains(key))
			mapped << fields.value(key, QStringLiteral(""));
		else
		{
			if (ok != nullptr)
				*ok = false;
			return mapped;
		}
	}
	if (ok != nullptr)
		*ok = true;
	return mapped;
}

QStringList uniform_json_object_representation::mapValues(const QStringList& l, const QStringList& defaults) const
{
	if (l.count() != defaults.count())
		return QStringList();
	QStringList mapped;
	for (int i = 0; i < l.count(); ++i)
	{
		if (fields.contains(l.at(i)))
			mapped << fields.value(l.at(i), QStringLiteral(""));
		else
		{
			mapped << defaults.at(i);
		}
	}
	return mapped;
}

QString uniform_json_object_representation::makeTableDefinition() const
{
	return QStringLiteral("( ") + fieldNames.join(" , ") + QStringLiteral(" ) ");
}

QString uniform_json_object_representation::makeTableInsertion() const
{
	return QStringLiteral("( ") + joinFields(fields, fieldNames, " , ") + QStringLiteral(" )"); 
}

