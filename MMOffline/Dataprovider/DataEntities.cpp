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
	if (role == Qt::DisplayRole || role == Qt::UserRole)
	{
		QVariant temp;
		temp.setValue<DataEntity>(innerList.at(index.row()));
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
	innerList.clear();
	innerList << data;
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

