#pragma once
#include "abs_entity.h"
#include <QtCore/QMetaType>
#include <QtCore/QVector>

/*
	This file contains product entity. This is outer entity, it is obtained from server.
	It has no specific methods.
*/

class ProductEntity : public abs_entity
	// This entity represents products
{
public:
	IdInt id;				//	id of the product
	QString name;			//	name of the product
	double price;			//	price of the product
	int measure;			//	measure of the product
	IdInt groupId;			//	id of the group containing this product
	QVector<int> clientIds;	//  connection to clients. Some clients has no access to some products.
protected:
	virtual uniform_json_object_representation toJsonRepresentation() const override;
	virtual bool fromJsonRepr(const uniform_json_object_representation&) override;
	virtual TemplatedTableHandler* getAssocTable() const override;
	virtual QString getContentsForDb() const override;
	virtual abs_entity* fabricate() const override;
	virtual bool fromSql(QueryPtr q) override;
	virtual bool isLikeString(const QRegExp& qregexp) const override;
	virtual IdInt extractId() const override;
	virtual bool higherThan(const abs_entity* another) const override;
	virtual bool compare(abs_entity* another) const override;

	// real constructor
	void _listInit(const QStringList&);
public:
	ProductEntity();
	ProductEntity(IdInt id, QString name, QString shortname = QString::null, double price = 0.0, double priceWithTaxes = 0.0);
	ProductEntity(const QStringList& flist);
};

typedef std::shared_ptr<ProductEntity> Product;
typedef QVector<Product> ProductList;

// can be used with QVariant
Q_DECLARE_METATYPE(Product);
