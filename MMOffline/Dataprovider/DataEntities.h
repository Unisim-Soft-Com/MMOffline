#pragma once
#include "Entities/ClientEntity.h"
#include "Entities/ProductEntity.h"
#include "Entities/DocumentEntity.h"
#include "Entities/DocumentEntryEntity.h"
#include "Entities/GroupEntity.h"
#include "Entities/NamedIdEntity.h"
#include <QtCore/QSortFilterProxyModel>

extern const abs_entity * const associateTableWithData[PREDEFINED_TABLES_QUANTITY];

template<class DataEntityInh>
QVector<DataEntity> downcastEntity(QVector<DataEntityInh>& other)
{
	QVector<DataEntity> datavect;
	datavect.reserve(other.count());
	auto start = other.begin();
	while (start != other.end())
	{
		datavect.push_back(DataEntity(*start++));
	}
	return datavect;
}


class DataEntityListModel : public QAbstractListModel
{
	Q_OBJECT
protected:
	QVector<DataEntity> innerList;
public:
	using QAbstractListModel::QAbstractListModel;
	DataEntityListModel(const QVector<DataEntity>& clients, QWidget* parent);
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	void setData(const QVector<DataEntity>& data);
public slots:
	void mapClickToEntity(const QModelIndex& index);
signals:
	void dataEntityClicked(DataEntity);
};

class DataEntityFilterModel : public QSortFilterProxyModel
{
	Q_OBJECT
protected:
	bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;
public:
	using QSortFilterProxyModel::QSortFilterProxyModel;
};
