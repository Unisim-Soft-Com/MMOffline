#pragma once
#include "abs_entity.h"
#include <memory>
#include <qmetatype.h>

/*
	This entity is representing one line in document. It is associated with document with parentDocId id.
	This entity is inner, it is not received from server.
*/

class DocumentEntryEntity : public abs_entity
	// represents one line in document
{
public:
	IdInt parentDocId;		//	id of the parent document
	IdInt entryId;			//	id of this entry
	IdInt productId;		//	id of the product
	QString productName;	//	name of the product, used to avoid usage of additional queries
	double price;			//	price of used product
	IdInt measure;			// id of the measure 
	double quantity;		//	quantity of this product
	IdInt option1;			//	id of the option
	IdInt option2;			
	IdInt option3;
	QString comment;		//	comment. Check it to avoid "" or other ruining symbols

protected:
	// Inherited via abs_entity
	virtual uniform_json_object_representation toJsonRepresentation() const override;
	virtual bool fromJsonRepr(const uniform_json_object_representation&) override;
	virtual TemplatedTableHandler* getAssocTable() const override;
	virtual QString getContentsForDb() const override;
	virtual abs_entity* fabricate() const override;
	virtual bool fromSql(QueryPtr q) override;
	virtual bool isLikeString(const QRegExp& qregexp) const override;
	virtual IdInt extractId() const override;
	virtual bool compare(abs_entity* another) const override;
	virtual bool higherThan(const abs_entity* another) const override;
	// real constructor
	void _listInit(const QStringList&);
public:
	explicit DocumentEntryEntity();
	explicit DocumentEntryEntity(IdInt ID);
	explicit DocumentEntryEntity(const QStringList&);
};

typedef std::shared_ptr<DocumentEntryEntity> DocumentEntry;
typedef QVector<DocumentEntry> DocEntryList;

Q_DECLARE_METATYPE(DocumentEntry);