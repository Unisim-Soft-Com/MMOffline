#pragma once
#include "Entities/ClientEntity.h"
#include "Entities/ProductEntity.h"
#include "Entities/DocumentEntity.h"
#include "Entities/DocumentEntryEntity.h"
#include "Entities/GroupEntity.h"
#include "Entities/NamedIdEntity.h"
#include <QtCore/QSortFilterProxyModel>

extern const abs_entity * const associateTableWithData[PREDEFINED_TABLES_QUANTITY];

typedef int (*IdExtractionPtr)(DataEntity);


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
template <class DataEntityInh>
QVector<std::shared_ptr<DataEntityInh> > upcastEntityVector(QVector<DataEntity>& other)
{
	QVector<std::shared_ptr<DataEntityInh> > temp;
	temp.reserve(other.size());
	auto start = other.begin();
	std::shared_ptr<DataEntityInh> tempEnt;
	while (start != other.end())
	{
		tempEnt = std::dynamic_pointer_cast<DataEntityInh>(*(start++));
		if (tempEnt != nullptr)
		{
			temp.push_back(tempEnt);
			tempEnt = nullptr;
		}
	}
	return temp;
}

class DataEntityListModel : public QAbstractListModel
{
	Q_OBJECT
protected:
	QVector<DataEntity> innerList;
public:
	enum ExtendedRoles {SearchRole = Qt::UserRole + 1, QuantityView, DataCopyRole};


	using QAbstractListModel::QAbstractListModel;
	DataEntityListModel(const QVector<DataEntity>& clients, QWidget* parent);
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	void setData(const QVector<DataEntity>& data);
	void removeDataEntity(const QModelIndex&);
	void removeDataEntity(DataEntity);
	void replaceDataEntity(DataEntity);
	void reset();
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
public slots:
	void mapClickToEntity(const QModelIndex& index);
signals:
	void dataEntityClicked(DataEntity);
};

class DataCountingDataModel : public DataEntityListModel
{
	Q_OBJECT
protected:
	QHash<IdInt, int> quantityLinker;
	IdExtractionPtr extractionFunct;
public:
	using DataEntityListModel::DataEntityListModel;
	QVariant data(const QModelIndex& index, int role) const override;
	void assignQuantityInfo(QHash<IdInt, int>&& quinfo);
	bool assignQuantityUpdate(DataEntity, int);
	bool assignQuantityUpdate(IdInt, int);

};
