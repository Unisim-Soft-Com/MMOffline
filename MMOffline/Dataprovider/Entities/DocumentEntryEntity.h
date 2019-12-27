#pragma once
#include "abs_entity.h"
#include <memory>
#include <qmetatype.h>


class DocumentEntryEntity : public abs_entity
{
public:
	IdInt parentDocId;
	IdInt entryId;
	IdInt productId;
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
	virtual bool isLikeString(const QRegExp& qregexp) const override;
	virtual IdInt extractId() const override;
	void _listInit(const QStringList&);
public:
	explicit DocumentEntryEntity();
	explicit DocumentEntryEntity(IdInt ID);
	explicit DocumentEntryEntity(const QStringList&);

	// Inherited via abs_entity
};
typedef std::shared_ptr<DocumentEntryEntity> DocumentEntry;
Q_DECLARE_METATYPE(DocumentEntry);
typedef QVector<DocumentEntry> DocEntryList;
extern const QString entryIdAssertionQuery;