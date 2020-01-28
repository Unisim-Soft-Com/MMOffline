#pragma once
#include "abs_entity.h"
#include <QDateTime>
#include "DocumentEntryEntity.h"
#include <QtCore/QAbstractListModel>

/*
	This file contains entity of document, inner entity of the project. It is never obtained from server.
	It has non-stored possibility of linking entries to it.
*/

class DocumentEntity : public abs_entity
	// this represents a document with all required data
{
public:
	IdInt documentId;		//	id of this document
	QDateTime dateWhenCreated;	//	creation datetime
	QDate shippingDate;		//	shipping date
	IdInt clientId;			//	id of client used in this line
	QString clientName;		//	client name, used only for avoiding unnecessary queries
	IdInt warehouseId;		//	id of the warehouse
	QString warehouseName;	//	renundant warehouse name. Not used
	IdInt documentType;		//	id of document type
	QString documentTypeName; // name of the document type, renundant
	double alreadyPaid;		//	money already paid

	// This list is not stored into database. You need to collect it only for sending data.
	DocEntryList linkedEntries;
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

	// This init method is used as real constructor
	void _listInit(const QStringList& l);
public:
	explicit DocumentEntity();
	explicit DocumentEntity(IdInt ID);
	explicit DocumentEntity(const QStringList& args);

	// entry operations functionality
	// links entry if it's parent id is ok
	bool linkEntry(DocumentEntry);
	// unlinks entry, erasing it's parent id
	bool unlinkEntry(DocumentEntry);
	// equal to id == parent id
	bool hisEntry(DocumentEntry);
	// unlinks all
	void cleanEntryField();
};

//typedef for better usability
typedef std::shared_ptr<DocumentEntity> Document;
typedef QVector<Document> DocumentsList;

// can be used in QVariant
Q_DECLARE_METATYPE(Document);

