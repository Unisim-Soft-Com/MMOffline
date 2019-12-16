#pragma once
#include "Entities/ClientEntity.h"
#include "Entities/ProductEntity.h"
#include "Entities/DocumentEntity.h"
#include "Entities/DocumentEntryEntity.h"


extern const abs_entity * const associateTableWithData[PREDEFINED_TABLES_QUANTITY];
typedef std::shared_ptr<abs_entity> DataEntity;
Q_DECLARE_METATYPE(DataEntity);
class DataEntityListModel : public QAbstractListModel
{
	Q_OBJECT
protected:
	QVector<DataEntity> innerList;
public:
	DataEntityListModel(const QVector<DataEntity>& clients, QWidget* parent);
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

};
