#include "ProductEntity.h"
#include <limits>
#include <QVariant>

const QStringList fieldDefaults
{
QStringLiteral("-1"),
QStringLiteral(""),
QStringLiteral(""),
QStringLiteral("0.0"),
QStringLiteral("0.0"),
QStringLiteral("0"),
QStringLiteral("0.0"),
QStringLiteral("0.0"),
QStringLiteral("0.0"),
QStringLiteral(""),
QStringLiteral("0"),
QStringLiteral(""),
QStringLiteral("0"),
QStringLiteral(""),
QStringLiteral("0"),
QStringLiteral("0"),
QStringLiteral("0")
};
const QStringList compressedFieldNames
{
QStringLiteral("id"),
QStringLiteral("name"),
QStringLiteral("sn"),
QStringLiteral("pr"),
QStringLiteral("pr2"),
QStringLiteral("um"),
QStringLiteral("qty"),
QStringLiteral("qtyb"),
QStringLiteral("sld"),
QStringLiteral("ex"),
QStringLiteral("gr"),
QStringLiteral("clr"),
QStringLiteral("ogr"),
QStringLiteral("ogrn"),
QStringLiteral("gr1"),
QStringLiteral("ocol"),
QStringLiteral("gr2"),
};

uniform_json_object_representation ProductEntity::toJsonRepresentation() const
{
	return uniform_json_object_representation(
		compressedFieldNames,
		QStringList
		{
			QString::number(id),
			name,
			shortName,
			QString::number(price),
			QString::number(priceWithTaxes),
			QString::number(measure),
			QString::number(quantity),
			QString::number(quantityInPieces),
			QString::number(rest),
			expires,
			QString::number(group),
			color,
			QString::number(groupInDictionary),
			groupName,
			QString::number(groupCounterparty),
			QString::number(column),
			QString::number(groupgr2)
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
		+ QStringLiteral("\" , \"")+
		shortName
		+ QStringLiteral("\" , ")+
		QString::number(price)
		+ QStringLiteral(" , ")+
		QString::number(priceWithTaxes)
		+ QStringLiteral(" , ")+
		QString::number(measure)
		+ QStringLiteral(" , ")+
		QString::number(quantity)
		+ QStringLiteral(" , ")+
		QString::number(quantityInPieces)
		+ QStringLiteral(" , ")+
		QString::number(rest)
		+ QStringLiteral(" , \"") +
		expires
		+ QStringLiteral("\" , ") +
		QString::number(group)
		+ QStringLiteral(" , \"") +
		color
		+ QStringLiteral("\" , ") +
		QString::number(groupInDictionary)
		+ QStringLiteral(" , \"") +
		groupName
		+ QStringLiteral("\" , ") +
		QString::number(groupCounterparty)
		+ QStringLiteral(" , ") +
		QString::number(column)
		+ QStringLiteral(" , ") +
		QString::number(groupgr2)
		+ QStringLiteral(" )");
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
	case 17:
		groupgr2 = flist.at(i--).toInt(&ok);
		if (!ok) break;
	case 16:
		column = flist.at(i--).toInt(&ok);
		if (!ok) break;
	case 15:
		groupCounterparty = flist.at(i--).toInt(&ok);
		if (!ok) break;
	case 14:
		groupName = flist.at(i--);
	case 13:
		groupInDictionary = flist.at(i--).toInt(&ok);
		if (!ok) break;
	case 12:
		color = flist.at(i--);
	case 11:
		group = flist.at(i--).toInt(&ok);
		if (!ok) break;
	case 10:
		expires = flist.at(i--);
	case 9:
		rest = flist.at(i--).toDouble(&ok);
		if (!ok) break;
	case 8:
		quantityInPieces = flist.at(i--).toDouble(&ok);
		if (!ok) break;
	case 7:
		quantity = flist.at(i--).toDouble(&ok);
		if (!ok) break;
	case 6:
		measure = flist.at(i--).toInt(&ok);
		if (!ok) break;
	case 5:
		priceWithTaxes = flist.at(i--).toDouble(&ok);
		if (!ok) break;
	case 4:
		price = flist.at(i--).toDouble(&ok);
		if (!ok) break;
	case 3:
		shortName = flist.at(i--);
	case 2:
		name = flist.at(i--);
	case 1:
		id = flist.at(i--).toInt(&ok);
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
	:abs_entity(Products), id(std::numeric_limits<int>::min()), name(), shortName(),
	price(0.0), priceWithTaxes(0.0), measure(0), quantity(0.0),
	quantityInPieces(0.0), rest(0.0), expires(), group(0),
	color(), groupInDictionary(0), groupName(), groupCounterparty(0), column(0),
	groupgr2()
{

}

ProductEntity::ProductEntity(int Id, QString Name, QString ShortName, double Price, double PriceWithTaxes)
	:abs_entity(Products), id(Id), name(Name), shortName(ShortName),
	price(Price), priceWithTaxes(PriceWithTaxes), measure(0), quantity(0.0),
	quantityInPieces(0.0), rest(0.0), expires(), group(0),
	color(), groupInDictionary(0), groupName(), groupCounterparty(0), column(0),
	groupgr2()
{
}

ProductEntity::ProductEntity(const QStringList& flist)
	:abs_entity(Products)
{
	_listInit(flist);
}
