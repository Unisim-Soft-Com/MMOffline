#include "ClientEntity.h"
#include <QVariant>
#include <QWidget>

uniform_json_object_representation ClientEntity::toJsonRepresentation() const
{
	return uniform_json_object_representation(
		QStringList{ "id", "name" },
		QStringList{ QString::number(id), name }
	);
}

using namespace fieldPredefinitions;
bool ClientEntity::fromJsonRepr(const uniform_json_object_representation& o)
{
	QString t = o.value(QLatin1String("id"));
	bool ok;
	int tmp = t.toLongLong(&ok);
	if (ok)
	{
		id = tmp;
	}
	else
		return false;
	name = o.value(QLatin1String("name"));
	if (name.isNull())
		return false;
	return true;
}

TemplatedTableHandler* ClientEntity::getAssocTable() const
{
	return predefinedTables + Clients;
}

QString ClientEntity::getContentsForDb() const
{
	return QStringLiteral("( ") + QString::number(id) + QStringLiteral(" , \"") + name + QStringLiteral("\" )");
}

abs_entity* ClientEntity::fabricate() const
{
	return new ClientEntity(id, name);
}

ClientEntity::ClientEntity()
	:abs_entity(Clients)
{
}

ClientEntity::ClientEntity(const IdInt Id, const QString Name)
	: abs_entity(Clients), id(Id), name(Name)
{
}

ClientEntity::ClientEntity(const QStringList& list)
{
	if (list.count() != 2)
		throw InitializationError(2);
	bool ok;
	id = list.first().toInt(&ok);
	if (!ok) throw InitializationError(2);
	name = list.last();
}

bool ClientEntity::higherThan(const abs_entity* another) const
{
	if (class_id == another->myType())
	{
		const ClientEntity* temp = dynamic_cast<const ClientEntity*>(another);
		bool higher = false;
		int max = (temp->name.count() > name.count()) ? name.count() : temp->name.count();
		max = (max > 5) ? 5 : max;
		for (int i = 0; (i < max); ++i)
		{
			if (name.at(i).toCaseFolded() == temp->name.at(i).toCaseFolded())
			{
				continue;
			}
			higher = name.at(i).toCaseFolded() > temp->name.at(i).toCaseFolded();
			max = -1;
			break;
		}
		if (max > 0)
			return id > another->getId();
		return higher;
	}
	else
	{
		return id > another->getId();
	}
}

bool ClientEntity::compare(abs_entity* another) const
{
	auto temp = dynamic_cast<ClientEntity*>(another);
	if (temp == nullptr)
		return false;
	return id == temp->id;
}

bool ClientEntity::isLikeString(const QRegExp& qregexp) const
{
	if (name.contains(qregexp.pattern(), Qt::CaseInsensitive))
	{
		return true;
	}
	else if (QString::number(id).contains(qregexp.pattern()))
	{
		return true;
	}
	return false;
}

IdInt ClientEntity::extractId() const
{
	return id;
}

bool ClientEntity::fromSql(QueryPtr q)
{
	if (!q->next())
		return false;
	bool ok;
	QVariant temp(q->value(0));
	if (!temp.isValid())
		return false;
	id = temp.toLongLong(&ok);
	if (!ok)
		return false;
	temp = q->value(1);
	if (!temp.isValid())
		return false;
	name = temp.toString();
	return true;
}

ClientDataModel::ClientDataModel(QWidget* parent)
	: QAbstractListModel(parent)
{
}

ClientDataModel::ClientDataModel(const ClientList& clients, QWidget* parent)
	: QAbstractListModel(parent), innerList(clients)
{
}

int ClientDataModel::rowCount(const QModelIndex& parent) const
{
	return innerList.count();
}

QVariant ClientDataModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();
	if (index.row() >= rowCount())
		return QVariant();
	if (role == Qt::DisplayRole)
	{
		QVariant temp;
		temp.setValue<Client>(innerList.at(index.row()));
		return temp;
	}
	if (role == Qt::ItemDataRole::UserRole)
	{
		QVariant temp;
		temp.setValue<DataEntity>(std::shared_ptr<abs_entity>(innerList.at(index.row())));
		return temp;
	}
	return QVariant();
}

QVariant ClientDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

void ClientDataModel::setList(ClientList l)
{
	innerList.clear();
	innerList << l;
}