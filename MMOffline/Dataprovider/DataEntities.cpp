#include "DataEntities.h"
#include <QWidget>

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
	if (role == Qt::DisplayRole)
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
