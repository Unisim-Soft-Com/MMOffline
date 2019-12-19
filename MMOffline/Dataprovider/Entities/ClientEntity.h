#pragma once
#include "abs_entity.h"
#include <QtCore/QAbstractListModel>

class ClientEntity : public abs_entity
{
public:
	int id;
	QString name;
protected:
	// Inherited via abs_entity
	virtual uniform_json_object_representation toJsonRepresentation() const override;
	virtual bool fromJsonRepr(const uniform_json_object_representation&) override;
	virtual bool fromSql(QueryPtr q) override;
	virtual TemplatedTableHandler* getAssocTable() const override;
	virtual QString getContentsForDb() const override;
	virtual abs_entity* fabricate() const override;
	virtual bool isLikeString(const QRegExp& qregexp) const override;
public:
	ClientEntity();
	ClientEntity(int id, QString name);
};
typedef std::shared_ptr<ClientEntity> Client;
typedef QVector<Client> ClientList;
Q_DECLARE_METATYPE(Client);
class ClientDataModel : public QAbstractListModel
{
	Q_OBJECT
protected:
	ClientList innerList;
public:
	explicit ClientDataModel(QWidget* parent = nullptr);
	ClientDataModel(const ClientList& clients, QWidget* parent);
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	void setList(ClientList l);
};
