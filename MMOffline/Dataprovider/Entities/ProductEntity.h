#pragma once
#include "abs_entity.h"
#include <QtCore/QMetaType>
#include <QtCore/QVector>
class ProductEntity : public abs_entity
{
public:
	IdInt id;
	QString name;
	double price;
	int measure;
	IdInt groupId;
	QVector<int> clientIds;
protected:
	virtual uniform_json_object_representation toJsonRepresentation() const override;
	virtual bool fromJsonRepr(const uniform_json_object_representation&) override;
	virtual TemplatedTableHandler* getAssocTable() const override;
	virtual QString getContentsForDb() const override;
	virtual abs_entity* fabricate() const override;
	virtual bool fromSql(QueryPtr q) override;
	virtual bool isLikeString(const QRegExp& qregexp) const override;
	virtual IdInt extractId() const override;
	void _listInit(const QStringList&);
	virtual bool compare(abs_entity* another) const override;
public:
	ProductEntity();
	ProductEntity(IdInt id, QString name, QString shortname = QString::null, double price = 0.0, double priceWithTaxes = 0.0);
	ProductEntity(const QStringList& flist);
};
typedef std::shared_ptr<ProductEntity> Product;
typedef QVector<Product> ProductList;

Q_DECLARE_METATYPE(Product);
