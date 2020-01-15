#include "ProductEntity.h"
#include <limits>
#include <QVariant>
#include <qtextstream.h>
using namespace fieldPredefinitions;
const QStringList fieldDefaults
{
QStringLiteral("-1"),
QStringLiteral(""),
QStringLiteral("0.0"),
QStringLiteral("0"),
QStringLiteral("0"),
QStringLiteral(""),
};
ProductEntity;
const QStringList compressedFieldNames
{
QStringLiteral("id"),
QStringLiteral("name"),
QStringLiteral("price"),
QStringLiteral("um"),
QStringLiteral("group_id"),
QStringLiteral("clients_id_list")
};

QString _serializeIdList(const QVector<int>& vect)
{
	QString outstr;
	QTextStream out(&outstr);
	QVector<int>::ConstIterator begin = vect.constBegin();
	while (begin != vect.constEnd())
	{
		out << *(begin++) << "|";
	}
	out.flush();
	return outstr;
}
void _deserializeIdList(QVector<int>& vect, const QString& string)
{
	if (vect.count() > 0)
		vect.clear();
	auto temp = string.split("|", QString::SplitBehavior::SkipEmptyParts);
	bool ok = true;
	int buff;
	for (QString& str : temp)
	{
		buff = str.toInt(&ok);
		if (!ok)
			continue;
		vect.push_back(buff);
	}
}
uniform_json_object_representation ProductEntity::toJsonRepresentation() const
{
	return uniform_json_object_representation(
		compressedFieldNames,
		QStringList
		{
			QString::number(id),
			name,
			QString::number(price),
			QString::number(measure),
			QString::number(groupId),
			_serializeIdList(clientIds)
		}
	);
}

bool ProductEntity::fromJsonRepr(const uniform_json_object_representation& o)
{
	if (o.size() == 0)
		return false;
	QStringList L = o.mapValues(compressedFieldNames, fieldDefaults);
	_listInit(L);
	return true;
}

TemplatedTableHandler* ProductEntity::getAssocTable() const
{
	return predefinedTables + Products;
}

QString ProductEntity::getContentsForDb() const
{
	return QStringLiteral("( ") +
		QString::number(id)
		+ QStringLiteral(" , \"") +
		name
		+ QStringLiteral("\" , ") +
		QString::number(price)
		+ QStringLiteral(" , ") +
		QString::number(measure)
		+ QStringLiteral(" , ") +
		QString::number(groupId)
		+ QStringLiteral(" , \"") +
		_serializeIdList(clientIds) +
		QStringLiteral("\" )");
}

abs_entity* ProductEntity::fabricate() const
{
	return new ProductEntity(*this);
}

bool ProductEntity::fromSql(QueryPtr q)
{
	if (!q->next())
		return false;
	QStringList values;
	for (int i = 0; i < fieldsOfProductEntity.count(); ++i)
	{
		values << q->value(i).toString();
	}
	_listInit(values);
	return true;
}

void ProductEntity::_listInit(const QStringList& flist)
{
	int i = flist.count() - 1;
	bool ok = true;
	switch (flist.count())
	{
	case 6:
		_deserializeIdList(clientIds, flist.at(i--));
	case 5:
		groupId = flist.at(i--).toLongLong(&ok);
		if (!ok) break;
	case 4:
		measure = flist.at(i--).toLongLong(&ok);
		if (!ok) break;
	case 3:
		price = flist.at(i--).toDouble(&ok);
		if (!ok) break;
	case 2:
		name = flist.at(i--);
	case 1:
		id = flist.at(i--).toLongLong(&ok);
		break;
	default:
		ok = false;
	}
	if (!ok)
	{
		throw InitializationError(i);
	}
}

ProductEntity::ProductEntity()
	:abs_entity(Products), id(std::numeric_limits<int>::min()), name(),
	price(0.0), measure(0),
	groupId(0), clientIds()

{
}

ProductEntity::ProductEntity(IdInt Id, QString Name, QString ShortName, double Price, double PriceWithTaxes)
	:abs_entity(Products), id(Id), name(Name), price(Price), measure(0), groupId(0), clientIds()
{
}

ProductEntity::ProductEntity(const QStringList& flist)
	: abs_entity(Products)
{
	_listInit(flist);
}

bool ProductEntity::compare(abs_entity* another) const
{
	auto temp = dynamic_cast<ProductEntity*>(another);
	if (temp == nullptr)
		return false;
	return id == temp->id;
}

bool ProductEntity::isLikeString(const QRegExp& qregexp) const
{
	if (name.contains(qregexp.pattern(), Qt::CaseInsensitive))
	{
		return true;
	}
	else if (QString::number(id).contains(qregexp.pattern()))
	{
		return true;
	}
	return false;
}

IdInt ProductEntity::extractId() const
{
	return id;
}