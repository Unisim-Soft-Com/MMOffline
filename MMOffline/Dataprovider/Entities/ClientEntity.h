#pragma once
#include "abs_entity.h"
#include <QtCore/QAbstractListModel>

/*
	This entity is used for storing client id-name pairs. It's fuctional is similar with 
	namedId. This entity is separated from namedId entity for future possibility of expanding fields
	and adding extra interfaces. Clients are shallow-compared using id AND name.
*/

class ClientEntity : public abs_entity
	// Represents client
{
public:
	IdInt id;		//	id of this client. Primary key, must be unique.
	QString name;	//	name used during display
protected:
	// Inherited via abs_entity
	virtual uniform_json_object_representation toJsonRepresentation() const override;
	virtual bool fromJsonRepr(const uniform_json_object_representation&) override;
	virtual bool fromSql(QueryPtr q) override;
	virtual TemplatedTableHandler* getAssocTable() const override;
	virtual QString getContentsForDb() const override;
	virtual abs_entity* fabricate() const override;
	virtual bool isLikeString(const QRegExp& qregexp) const override;
	virtual IdInt extractId() const override;
	virtual bool compare(abs_entity* another) const override;
	virtual bool higherThan(const abs_entity* another) const override;

public:
	explicit ClientEntity();
	explicit ClientEntity(const IdInt id, const QString name);
	
	// list initialization is order-dependent
	explicit ClientEntity(const QStringList& list);
};

// Own structures for easier writing
typedef std::shared_ptr<ClientEntity> Client;
typedef QVector<Client> ClientList;

// Can be used in QVariant
Q_DECLARE_METATYPE(Client);


class ClientDataModel : public QAbstractListModel
	// Additionally specialized model for better optimization
{
	Q_OBJECT
protected:
	// inner list holding data
	ClientList innerList;
public:
	explicit ClientDataModel(QWidget* parent = nullptr);
	ClientDataModel(const ClientList& clients, QWidget* parent = nullptr);
	// inherited from QAbstractListModel
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	// post-constructor
	void setList(ClientList l);
};
