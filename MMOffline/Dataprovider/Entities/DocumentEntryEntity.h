#pragma once
#include "abs_entity.h"
#include <memory>



class DocumentEntryEntity : public abs_entity
{
public:
	int parentDocId;
	int entryId;
	int productId;
	QString productName;
	double price;
	int measure;
	double quantity;
	QString option1;
	QString option2;
	QString option3;
	QString comment;

protected:
	// Inherited via abs_entity
	virtual uniform_json_object_representation toJsonRepresentation() const override;
	virtual bool fromJsonRepr(const uniform_json_object_representation&) override;
	virtual TemplatedTableHandler* getAssocTable() const override;
	virtual QString getContentsForDb() const override;
	virtual abs_entity* fabricate() const override;
	virtual bool fromSql(QueryPtr q) override;

	void _listInit(const QStringList&);
public:
	explicit DocumentEntryEntity();
	explicit DocumentEntryEntity(int ID);
	explicit DocumentEntryEntity(const QStringList&);
};

typedef std::shared_ptr<DocumentEntryEntity> DocEntryPtr;