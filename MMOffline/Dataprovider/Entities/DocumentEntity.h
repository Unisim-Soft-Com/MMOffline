#pragma once
#include "abs_entity.h"
#include <QDate>
#include "DocumentEntryEntity.h"
#include <QtCore/QAbstractListModel>



class DocumentEntity : public abs_entity
{
public:
	int documentId;
	QDate dateWhenCreated;
	QDate shippingDate;
	int clientId;
	QString clientName;
	int warehouseId;
	QString warehouseName;
	int documentType;
	QString documentTypeName;
	double alreadyPaid;

	QVector<DocEntryPtr> linkedEntries;
protected:
	// Inherited via abs_entity
	virtual uniform_json_object_representation toJsonRepresentation() const override;
	virtual bool fromJsonRepr(const uniform_json_object_representation&) override;
	virtual TemplatedTableHandler* getAssocTable() const override;
	virtual QString getContentsForDb() const override;
	virtual abs_entity* fabricate() const override;
	virtual bool fromSql(QueryPtr q) override;

	void _listInit(const QStringList& l);
public:
	explicit DocumentEntity();
	explicit DocumentEntity(int ID);
	explicit DocumentEntity(const QStringList& args);

	bool linkEntry(DocEntryPtr);
	bool unlinkEntry(DocEntryPtr);
	bool hisEntry(DocEntryPtr);
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
