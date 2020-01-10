#pragma once
#include "abs_entity.h"
#include <QtCore/QMetaType>
#include <QtCore/QAbstractListModel>
#include <QAbstractTableModel>
#include <QHash>
#include <QStack>
class GroupEntity : public abs_entity
{
public:
	// deserialized data
	QString name;
	IdInt id;
	IdInt superiorGroupId;
protected:
	// vector of subgroups
	QVector<std::shared_ptr<GroupEntity> > subgroups;

	// Inherited via abs_entity
	virtual uniform_json_object_representation toJsonRepresentation() const override;
	virtual bool fromJsonRepr(const uniform_json_object_representation&) override;
	virtual TemplatedTableHandler* getAssocTable() const override;
	virtual QString getContentsForDb() const override;
	virtual abs_entity* fabricate() const override;
	virtual bool fromSql(QueryPtr q) override;
	virtual bool isLikeString(const QRegExp& qregexp) const override;
	virtual IdInt extractId() const override;
	bool _listInit(QStringList& L);
	virtual bool compare(abs_entity* another) const override;
public:
	explicit GroupEntity();
	explicit GroupEntity(QString name, IdInt Id = 0, IdInt SuperiorGroupId = 0);
	const QVector<std::shared_ptr<GroupEntity> >& getSubgroups() const;
	bool isTopLevel() const;
	bool hasSubgroups() const;
	bool appendIfOwned(const std::shared_ptr<GroupEntity> &);
	bool owns(const std::shared_ptr<GroupEntity>&);
	const  std::shared_ptr<GroupEntity>& getSubgroupIfExists(IdInt id) const;
	const std::shared_ptr<GroupEntity>& getSubgroup(int index) const;
	int countSubgroups() const;
	bool operator==(const GroupEntity&) const;

};
typedef std::shared_ptr<GroupEntity> Group;
typedef QVector<Group> GroupList;

Q_DECLARE_METATYPE(Group);

class GroupTreeModel: public QAbstractTableModel
{
	Q_OBJECT
protected:
	GroupList topLayerList;
	QHash<uint, Group> topLayerLinker;
	QStack<uint> layersIds;
	int currentVisibleLayer;
	Group currentGroup;
public:
	using QAbstractTableModel::QAbstractTableModel;
	GroupTreeModel(const GroupList& clients, QObject* parent);
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	void setList(const GroupList& l);
	void clearLayers();
public slots:
	void stepToNextLayer(const QModelIndex& index);
	void stepToUpperLevel();
signals:
	void groupSelected(const Group&);
	void backRequired();
};