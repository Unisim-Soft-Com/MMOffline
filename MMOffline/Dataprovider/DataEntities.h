#pragma once
#include "Entities/ClientEntity.h"
#include "Entities/ProductEntity.h"
#include "Entities/DocumentEntity.h"
#include "Entities/DocumentEntryEntity.h"
#include "Entities/GroupEntity.h"
#include "Entities/NamedIdEntity.h"
#include <QtCore/QSortFilterProxyModel>

/*
	This file is used as aggregate header for all entities used in the project. Here
	also are defined some usefull template functions and extended data models for 
	polymorthic usage. It is enough to include only this header to use all entities.
*/


// this table is usefull as hash of all types of entities. This is part of prototype pattern.
extern const abs_entity* const associateTableWithData[PREDEFINED_TABLES_QUANTITY];

template<class DataEntityInh>
QVector<DataEntity> downcastEntity(QVector<DataEntityInh>& other)
// This template function is used for dynamic downcasting of data entity vectors.
// If cast fails, nullptr is inserted into output vector.
{
	QVector<DataEntity> datavect;
	datavect.reserve(other.count());
	auto start = other.begin();
	while (start != other.end())
	{
		datavect.push_back(DataEntity(std::dynamic_pointer_cast<DataEntity>(*start++)));
	}
	return datavect;
}
template <class DataEntityInh>
QVector<std::shared_ptr<DataEntityInh> > upcastEntityVector(QVector<DataEntity>& other)
// This function is dynamically downcasting vector of entities. Non-casted entities are skipped.
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
	// This data model is used for fully dynamical data entity model. Use right delegate to show data in view.
	// You can place in this model even polymorthic data, just install suitable delegate for it.
{
	Q_OBJECT
protected:
	// real data storage
	QVector<DataEntity> innerList;
public:
	// Roles for data entity operations
	enum ExtendedRoles { 
		// Search role returns polymorthic pointer to call ->compare
		SearchRole = Qt::UserRole + 1,
		// Quantity view is used to return quantity of items. Not used in this model
		QuantityView,
		// DataCopyRole returns full copy of an object to avoid changing model
		DataCopyRole 
	};

	using QAbstractListModel::QAbstractListModel;
	DataEntityListModel(const QVector<DataEntity>& clients, QWidget* parent = nullptr);
	// Inherited from QAbstractListModel
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	// dataoperation methods
	//post-constructor
	void setData(const QVector<DataEntity>& data);
	// deletes data entity in the model by it's index or pointer
	void removeDataEntity(const QModelIndex&);
	void removeDataEntity(DataEntity);
	// replaces data entity by it's id. This method is used only if you have a copy with the same id and other data.
	void replaceDataEntity(DataEntity);
	// empties model
	void reset();
public slots:
	//If index was valid - emits signal with pointer to clicked entity.
	void mapClickToEntity(const QModelIndex& index);
signals:
	// delivers pointer to data entity. Warning - changing this entity by pointer will affect model.
	void dataEntityClicked(DataEntity);
};

class DataEntityFilterModel : public QSortFilterProxyModel
	// This model is overloading normal sortFilterProxyModel, adding compatibility with polymorthic
	// DataEntity interfaces
{
	Q_OBJECT
protected:
	// Uses ->filter() to define if row is containing string
	bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;
public:
	using QSortFilterProxyModel::QSortFilterProxyModel;
public slots:
	// Similar with DataEntityListModel
	void mapClickToEntity(const QModelIndex& index);
signals:
	void dataEntityClicked(DataEntity);
};

class DataCountingDataModel : public DataEntityListModel
	// This data model is used for attaching counter to each entity by it's id. This is
	// usefull when counters are connected to actions or aggregated data from nested selects.
{
	Q_OBJECT
protected:
	// linker stores connections between ID's and counters.
	QHash<IdInt, int> quantityLinker;
public:
	using DataEntityListModel::DataEntityListModel;
	QVariant data(const QModelIndex& index, int role) const override;
	// obtains hash with connections. 
	void assignQuantityInfo(QHash<IdInt, int>&& quinfo);
	// updates quantity by id or pointer
	bool assignQuantityUpdate(DataEntity, int);
	bool assignQuantityUpdate(IdInt, int);
	// resets model with provided defaults
	void assingEmptyCounters(int def = 0);
	// increments quantity by provided value
	bool incrementQuantity(IdInt, int);
};
