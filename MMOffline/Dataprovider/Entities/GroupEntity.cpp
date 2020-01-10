#include "GroupEntity.h"


using namespace fieldPredefinitions;
Group nullGroup(nullptr);
uniform_json_object_representation GroupEntity::toJsonRepresentation() const
{
	return uniform_json_object_representation(
		fieldsOfGroupEntity,
		QStringList
		{
			name,
			QString::number(id),
			QString::number(superiorGroupId)
		}
	);
}
inline QPair<int, int> splitIndex(int i, int count);
inline int joinIndex(const QModelIndex& ind, int count);
inline bool assertIndex(const QModelIndex& ind, int count)
{	return count <= joinIndex(ind, count);
}

bool GroupEntity::fromJsonRepr(const uniform_json_object_representation& o)
{
	QStringList L = o.mapValues(fieldsOfGroupEntity, QStringList{ "","0","0" });
	return _listInit(L);
}

TemplatedTableHandler* GroupEntity::getAssocTable() const
{
	return predefinedTables + Groups;
}

QString GroupEntity::getContentsForDb() const
{
	return QStringLiteral("( \"") +
		name
		+ QStringLiteral("\" , ") +
		QString::number(id)
		+ QStringLiteral(" , ") +
		QString::number(superiorGroupId)
		+ QStringLiteral(" )");
}

abs_entity* GroupEntity::fabricate() const
{
	return new GroupEntity(*this);
}

bool GroupEntity::fromSql(QueryPtr q)
{
	if (!q->next())
		return false;
	QStringList values;
	for (int i = 0; i < fieldsOfGroupEntity.count(); ++i)
	{
		values << q->value(i).toString();
	}
	_listInit(values);
	return true;
}

bool GroupEntity::isLikeString(const QRegExp& qregexp) const
{
	return name.contains(qregexp.pattern());
}

IdInt GroupEntity::extractId() const
{
	return id;
}

bool GroupEntity::_listInit(QStringList& L)
{
	int i = L.count() - 1;
	bool ok;
	switch (L.count())
	{
	case 0:
		return false;
	case 3:
		superiorGroupId = L.at(i--).toLongLong(&ok);
		if (!ok) throw InitializationError(i);
	case 2:
		id = L.at(i--).toLongLong(&ok);
		if (!ok) throw InitializationError(i);
	case 1:
		name = L.at(i--);
	}
	return true;
}

GroupEntity::GroupEntity()
	: abs_entity(Groups), name(""), id(0), superiorGroupId(0), subgroups()
{
}

GroupEntity::GroupEntity(QString Name, IdInt Id, IdInt SuperiorGroupId)
	: abs_entity(Groups), name(Name), id(Id), superiorGroupId(SuperiorGroupId), subgroups()
{

}

bool GroupEntity::isTopLevel() const
{
	return superiorGroupId == 0;
}

bool GroupEntity::hasSubgroups() const
{
	return subgroups.count() != 0;
}

bool GroupEntity::appendIfOwned(const std::shared_ptr<GroupEntity>& grp)
{
	if (owns(grp))
	{
		subgroups.append(grp);
		return true;
	}
	else
	{
		return false;
	}
}

bool GroupEntity::owns(const std::shared_ptr<GroupEntity>& another)
{
	if (id == another->superiorGroupId)
	{
		return true;
	}
	return false;
}

const Group& GroupEntity::getSubgroupIfExists(IdInt id) const
{
	QVector<Group>::ConstIterator begin = subgroups.constBegin();
	while (begin != subgroups.constEnd())
	{
		if ((*begin)->id == id)
			return *(begin);
		++begin;
	}
	return nullGroup;
}

const std::shared_ptr<GroupEntity>& GroupEntity::getSubgroup(int index) const
{
	return subgroups.at(index);
}

int GroupEntity::countSubgroups() const
{
	return subgroups.count();
}

bool GroupEntity::operator==(const GroupEntity& another) const
{
	return id == another.id;
}

bool GroupEntity::compare(abs_entity* another) const
{
	auto temp = dynamic_cast<GroupEntity*>(another);
	if (temp == nullptr)
		return false;
	return id == temp->id;
}

const QVector<std::shared_ptr<GroupEntity>>& GroupEntity::getSubgroups() const
{
	return subgroups;
}

GroupTreeModel::GroupTreeModel(const GroupList& groups, QObject* parent)
	: QAbstractTableModel(parent)
{
	setList(groups);
}

int GroupTreeModel::rowCount(const QModelIndex& parent) const
{
	switch (currentVisibleLayer)
	{
	case 0:	//	top level layer has no group bound
		return topLayerList.count()/2 + topLayerList.count() % 2;
	default:
		return currentGroup->countSubgroups()/2 + currentGroup->countSubgroups() % 2;
	}	
}

int GroupTreeModel::columnCount(const QModelIndex& parent) const
{
	switch (currentVisibleLayer)
	{
	case 0:
		return ((topLayerList.count() > 1)? 2 : 1);
	default:
		return (currentGroup->countSubgroups() > 1)? 2 : 1;
	}
}

QVariant GroupTreeModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();
	if (index.row() >= rowCount())
		return QVariant();
	if (role == Qt::DisplayRole)
	{
		QVariant temp;
		switch (currentVisibleLayer)
		{
		case 0:
			if (assertIndex(index, topLayerList.count()))
				return temp;
			temp.setValue<QString>(topLayerList.at(joinIndex(index, topLayerList.count()))->name);
			break;
		default:
			if (assertIndex(index, currentGroup->countSubgroups( )))
				return temp;
			temp.setValue<QString>(currentGroup->getSubgroup(joinIndex(index, currentGroup->countSubgroups()))->name);
		}
		return temp;
	}
	if (role == Qt::ItemDataRole::UserRole)
	{
		QVariant temp;
		switch (currentVisibleLayer)
		{
		case 0:
			temp.setValue<DataEntity>(DataEntity(topLayerList.at(joinIndex(index, topLayerList.count()))));
			break;
		default:
			temp.setValue<DataEntity>(DataEntity(currentGroup->getSubgroup(joinIndex(index, currentGroup->countSubgroups()))));
		}
		return temp;
	}
	return QVariant();
}

QVariant GroupTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (section == 0 && role == Qt::DisplayRole)
	{
		if (currentVisibleLayer == 0)
			return QVariant();
		return QVariant(currentGroup->name);
	}
	return QVariant();
}

void GroupTreeModel::setList(const GroupList& l)
{
	topLayerList.clear();
	topLayerLinker.clear();
	layersIds.clear();
	currentVisibleLayer = 0;
	QHash<uint, Group> templinker;
	for (const Group & group : l)
	{
		templinker.insert(group->id, group);
	}
	for (const Group& group : l)
	{
		if (group->isTopLevel())
		{
			topLayerLinker.insert(group->id, group);
			topLayerList.push_back(group);
		}
		else
		{
			if (templinker.contains(group->superiorGroupId))
			{
				templinker[group->superiorGroupId]->appendIfOwned(group);
			}
		}
	}
	

}

void GroupTreeModel::clearLayers()
{
	beginResetModel();
	currentVisibleLayer = 0;
	currentGroup = nullptr;
	layersIds.clear();
	endResetModel();
}

void GroupTreeModel::stepToNextLayer(const QModelIndex& index)
{
	Group temp;
	beginResetModel();
	switch (currentVisibleLayer)
	{
	case 0:
	{
		if (assertIndex(index, topLayerList.count()))
			return;
		temp = topLayerList.at(joinIndex(index, topLayerList.count()));
		currentGroup = topLayerList.at(joinIndex(index, topLayerList.count()));
		if (!currentGroup->hasSubgroups())
		{
			emit groupSelected(currentGroup);
			endResetModel();
			emit dataChanged(createIndex(0, 0), createIndex(rowCount(), columnCount()));
			return;
		}
		break;
	}
	default:
	{
		temp = topLayerLinker.value(layersIds.at(0));
		int i = 1;
		while (i < layersIds.count())
		{
			temp = temp->getSubgroupIfExists(layersIds.at(i));
			++i;
		}
		if (assertIndex(index, temp->countSubgroups()))
		{
			endResetModel();
			return;
		}
		if (!temp->getSubgroup(joinIndex(index, temp->countSubgroups()))->hasSubgroups())
		{
			emit groupSelected(temp->getSubgroup(joinIndex(index, temp->countSubgroups())));
			endResetModel();
			emit dataChanged(createIndex(0, 0), createIndex(rowCount(), columnCount()));
			return;
		}
		if (layersIds.contains(temp->getSubgroup(joinIndex(index, temp->countSubgroups()))->id))
		{
			endResetModel();
			emit groupSelected(temp->getSubgroup(joinIndex(index, temp->countSubgroups())));
			return;
		}
		currentGroup = temp->getSubgroup(joinIndex(index, temp->countSubgroups()));
	}
	}
	++currentVisibleLayer;
	layersIds.push(currentGroup->id);
	endResetModel();
	emit dataChanged(createIndex(0, 0), createIndex(rowCount(), columnCount()));
}

void GroupTreeModel::stepToUpperLevel()
{
	switch (currentVisibleLayer)
	{
	case 0:
		layersIds.clear();
		emit backRequired();
		return;
	case 1:
		beginResetModel();
		layersIds.clear();
		--currentVisibleLayer;
		currentGroup = Group();
		endResetModel();
		emit dataChanged(createIndex(0, 0), createIndex(rowCount(), columnCount()));
		return;
	default:
		beginResetModel();
		Group temp = topLayerLinker.value(layersIds.at(0));
		int i = 1;
		while (i < layersIds.count() - 1 )
		{
			temp = temp->getSubgroupIfExists(layersIds.at(i));
			++i;
		}
		currentGroup = temp;
		break;
	}
	--currentVisibleLayer;
	endResetModel();
	if (!layersIds.isEmpty())
		layersIds.pop();
	emit dataChanged(createIndex(0, 0), createIndex(rowCount(), columnCount()));
}

inline int joinIndex(const QModelIndex& ind, int count)
{
	switch (ind.column())
	{
	case 0:
		return ind.row();
	case 1:
		return count / 2 + count % 2 + ind.row();
	default:
		return 0;
	}
}
inline QPair<int, int> splitIndex(int i, int count)
{
	int middle = (i > count / 2 + count % 2);
	return QPair<int, int>(
		(i > middle) ? i - middle : i,
		(i > middle) ? 1 : 0
		);
}