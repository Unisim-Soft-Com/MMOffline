#include "JsonUniresult.h"
#include <QTextStream>




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
		fields << "";
	}
}

uniform_json_object_representation::uniform_json_object_representation(QStringList& l)
	: fieldNames(l), fields(), sizeOfThis(l.count())
{
	for (int i = 0; i < sizeOfThis; ++i)
	{
		fields << "";
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
			fields << val.toString();
			break;
		case QJsonValue::Type::Double:
			fields << QString::number(val.toDouble());
			break;
		case QJsonValue::Bool:
			fields << ((val.toBool()) ? QStringLiteral("true") : QStringLiteral("false"));
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
		fields << f.at(i);
	}
}

QString& uniform_json_object_representation::operator[](int ind)
{
	return fields[ind];
}

QString& uniform_json_object_representation::operator[](QLatin1String key)
{
	int ind = fieldNames.indexOf(key);
	if (ind == -1)
		return NotAResult;
	else
	{
		return fields[ind];
	}
}

QString& uniform_json_object_representation::operator[](QString& key)
{
	int ind = fieldNames.indexOf(key);
	if (ind == -1)
		return NotAResult;
	else
	{
		return fields[ind];
	}
}

uniform_json_object_representation& uniform_json_object_representation::operator<<(QString& nval)
{
	for (int i = 0; i < sizeOfThis; ++i)
	{
		if (fields.at(i).isEmpty())
		{
			fields[i] = nval;
			return *this;
		}
	}
	return *this;
}

QString uniform_json_object_representation::at(int ind) const
{
	return fields.at(ind);
}

QString uniform_json_object_representation::toString() const
{
	QString temp;
	QTextStream cin(&temp);
	cin << "{";
	for (int i = 0; i < fieldNames.count(); ++i)
	{
		cin << "\"" << fieldNames.at(i) << "\":\"" << fields.at(i) << "\"," << endl;
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

const QString& uniform_json_object_representation::value(QString key)const
{
	int ind = fieldNames.indexOf(key);
	if (ind == -1)
		return NotAResult;
	else
	{
		return fields[ind];
	}
}

const QString& uniform_json_object_representation::value(QLatin1String key) const
{
	int ind = fieldNames.indexOf(key);
	if (ind == -1)
		return NotAResult;
	else
	{
		return fields[ind];
	}
}

void uniform_json_object_representation::setKeys(QStringList& l)
{
	fieldNames.clear();
	fields.clear();
	fieldNames << l;
	sizeOfThis = l.count();
	for (int i = 0; i < sizeOfThis; ++i)
	{
		fields << "";
	}
}

void uniform_json_object_representation::setFields(QStringList& l)
{
	if (l.count() < sizeOfThis)
		return;
	fields.clear();
	for (int i = 0; i < sizeOfThis; ++i)
	{
		fields << l.at(i);
	}
}

void uniform_json_object_representation::addField(QString key, QString val)
{
	fieldNames << key;
	fields << val;
	++sizeOfThis;
}

QString uniform_json_object_representation::makeTableDefinition() const
{
	return QStringLiteral("( ") + fieldNames.join(" , ") + QStringLiteral(" ) ");
}

QString uniform_json_object_representation::makeTableInsertion() const
{
	return QStringLiteral("( ") + fields.join(" , ") + QStringLiteral(" )"); 
}
