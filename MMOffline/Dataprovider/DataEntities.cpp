#include "DataEntities.h"
#include <QWidget>

#include "debugtrace.h"
const abs_entity* const associateTableWithData[PREDEFINED_TABLES_QUANTITY]
{
	new ClientEntity(),
	new ProductEntity(),
	new DocumentEntity(),
	new DocumentEntryEntity()
};

int indexOfEntityById(int id, const QVector<DataEntity>& v)
{
	for (int i = 0; i < v.count(); ++i)
	{
		if (v.at(i)->getId() == id)
			return i;
	}
	return -1;
}

DataEntityListModel::DataEntityListModel(const QVector<DataEntity>& entities, QWidget* parent)
	: QAbstractListModel(parent), innerList(entities)
{
}

int DataEntityListModel::rowCount(const QModelIndex& parent) const
{
	return innerList.count();
}

QVariant DataEntityListModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();
	if (index.row() >= rowCount())
		return QVariant();
	if (role == Qt::DisplayRole || role == SearchRole)
	{
		QVariant temp;
		temp.setValue<DataEntity>(innerList.at(index.row()));
		return temp;
	}
	else if (role == DataCopyRole)
	{
		QVariant temp;
		temp.setValue<DataEntity>(DataEntity(innerList.at(index.row())->clone()));
		return temp;
	}
	return QVariant();
}

QVariant DataEntityListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

void DataEntityListModel::setData(const QVector<DataEntity>& data)
{
	beginResetModel();
	innerList.clear();
	innerList << data;
	endResetModel();
}

void DataEntityListModel::removeDataEntity(const QModelIndex& mindex)
{
	if (!mindex.isValid())
		return;
	beginRemoveRows(mindex, mindex.row(), mindex.row());
	innerList.removeAt(mindex.row());
	endRemoveRows();
}

void DataEntityListModel::removeDataEntity(DataEntity e)
{
	for (int i = 0; i < innerList.count(); ++i)
	{
		if (innerList.at(i)->deepCompare(&(*(e))))
		{
			beginRemoveRows(createIndex(i, 0), i, i);
			innerList.removeAt(i);
			endRemoveRows();
		}
	}
}

void DataEntityListModel::replaceDataEntity(DataEntity e)
{
	for (int i = 0; i < innerList.count(); ++i)
	{
		if (innerList.at(i)->deepCompare(&(*(e))))
		{
			innerList[i] = e;
		}
	}
}

void DataEntityListModel::reset()
{
	beginResetModel();
	innerList.clear();
	endResetModel();
}

void DataEntityListModel::mapClickToEntity(const QModelIndex& index)
{
	if (!index.isValid())
		return;
	emit dataEntityClicked(innerList.at(index.row()));
}

bool DataEntityFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
	DataEntity temp = sourceModel()->index(sourceRow, 0, sourceParent).data(filterRole()).value<DataEntity>();
	if (temp == nullptr)
		return false;
	return temp->filter(filterRegExp());
}

void DataEntityFilterModel::mapClickToEntity(const QModelIndex& index)
{
	detrace_METHCALL("mapClickToEntity");
	if (!index.isValid())
		return;
	QVariant temp = mapToSource(index).data(Qt::DisplayRole);
	DataEntity tde(nullptr);
	tde = temp.value<DataEntity>();
	if (tde != nullptr)
	{
		emit dataEntityClicked(tde);
	}
}

QVariant DataCountingDataModel::data(const QModelIndex& index, int role) const
{
	if (role == QuantityView)
	{
		return QVariant(quantityLinker.value(innerList.at(index.row())->getId()));
	}
	return DataEntityListModel::data(index, role);
}

void DataCountingDataModel::assignQuantityInfo(QHash<IdInt, int>&& quinfo)
{
	beginResetModel();
	quantityLinker = QHash<IdInt, int>(quinfo);
	endResetModel();
}

bool DataCountingDataModel::assignQuantityUpdate(DataEntity dent, int q)
{
	if (quantityLinker.contains(dent->getId()))
	{
		quantityLinker[dent->getId()] = q;
		return true;
	}
	return false;
}

bool DataCountingDataModel::assignQuantityUpdate(IdInt id, int q)
{
	if (quantityLinker.contains(id))
	{
		quantityLinker[id] = q;
		QModelIndex updated = createIndex(indexOfEntityById(id, innerList) - 1, 0);
		QModelIndex postUpdated = createIndex(updated.row() + 1, 0);
		dataChanged(updated, postUpdated);
		return true;
	}
	return false;
}

void DataCountingDataModel::assingEmptyCounters(int def)
{
	for (DataEntity e : innerList)
	{
		quantityLinker[e->getId()] = def;
	}
}

bool DataCountingDataModel::incrementQuantity(IdInt id, int q)
{
	if (quantityLinker.contains(id))
	{
		quantityLinker[id] += q;
		QModelIndex updated = createIndex(indexOfEntityById(id, innerList) - 1, 0);
		QModelIndex postUpdated = createIndex(updated.row() + 1, 0);
		dataChanged(updated, postUpdated);
		return true;
	}
	return false;
}