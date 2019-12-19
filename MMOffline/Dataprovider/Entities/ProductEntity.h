#pragma once
#include "abs_entity.h"
#include <QtCore/QMetaType>

extern const QStringList fieldsOfProductEntity;
class ProductEntity : public abs_entity
{
public:
	int id;
	QString name;
	QString shortName;
	double price;
	double priceWithTaxes;
	int measure;
	double quantity;
	double quantityInPieces;
	double rest;
	QString expires;
	int groupId;
	QString color;
	int groupInDictionary;
	QString groupName;
	int groupCounterparty;
	int columnBE;				//	This variable's functionality is unknown
	int groupgr2;			//	This variable's functionality is unknown
protected:
	virtual uniform_json_object_representation toJsonRepresentation() const override;
	virtual bool fromJsonRepr(const uniform_json_object_representation&) override;
	virtual TemplatedTableHandler* getAssocTable() const override;
	virtual QString getContentsForDb() const override;
	virtual abs_entity* fabricate() const override;
	virtual bool fromSql(QueryPtr q) override;
	virtual bool isLikeString(const QRegExp& qregexp) const override;

	void _listInit(const QStringList&);
public:
	ProductEntity();
	ProductEntity(int id, QString name, QString shortname = QString::null, double price = 0.0, double priceWithTaxes = 0.0);
	ProductEntity(const QStringList & flist);
};
typedef std::shared_ptr<ProductEntity> Product;
typedef QVector<Product> ProductList;

Q_DECLARE_METATYPE(Product);