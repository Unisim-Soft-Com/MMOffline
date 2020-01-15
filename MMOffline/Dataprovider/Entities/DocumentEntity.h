#pragma once
#include "abs_entity.h"
#include <QDateTime>
#include "DocumentEntryEntity.h"
#include <QtCore/QAbstractListModel>

class DocumentEntity : public abs_entity
{
public:
	IdInt documentId;
	QDateTime dateWhenCreated;
	QDate shippingDate;
	IdInt clientId;
	QString clientName;
	IdInt warehouseId;
	QString warehouseName;
	int documentType;
	QString documentTypeName;
	double alreadyPaid;

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
	void _listInit(const QStringList& l);
	virtual bool compare(abs_entity* another) const override;
public:
	explicit DocumentEntity();
	explicit DocumentEntity(IdInt ID);
	explicit DocumentEntity(const QStringList& args);

	bool linkEntry(DocumentEntry);
	bool unlinkEntry(DocumentEntry);
	bool hisEntry(DocumentEntry);
	void cleanEntryField();
};
typedef std::shared_ptr<DocumentEntity> Document;
typedef QVector<Document> DocumentsList;

Q_DECLARE_METATYPE(Document);
class DocumentsListModel : public QAbstractListModel
{
	Q_OBJECT
protected:
	DocumentsList innerList;
public:
	DocumentsListModel(const DocumentsList& doclist, QWidget* parent);
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};
extern const QString documentIdAssertionQuery;