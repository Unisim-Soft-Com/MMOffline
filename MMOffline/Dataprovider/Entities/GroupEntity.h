#pragma once
#include "abs_entity.h"
#include <QtCore/QMetaType>
#include <QtCore/QAbstractListModel>
#include <QAbstractTableModel>
#include <QHash>
#include <QStack>

/*
	This file is containing group entity with it's own TableModel, which is suited for non-standard walking
*/

class GroupEntity : public abs_entity
	//	represents groups of the products
{
public:
	QString name;	//	name which is displayed to user
	IdInt id;		//	id of this group
	IdInt superiorGroupId;	//	id of superior group
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
	virtual bool compare(abs_entity* another) const override;
	virtual bool higherThan(const abs_entity* another) const override;
	// real constructor
	bool _listInit(QStringList& L);
public:
	explicit GroupEntity();
	explicit GroupEntity(QString name, IdInt Id = 0, IdInt SuperiorGroupId = 0);
	explicit GroupEntity(QStringList& l);

	// tree interfaces. Each groupEntity can be used as tree node.
	// returns subgroups linked to this entity
	const QVector<std::shared_ptr<GroupEntity> >& getSubgroups() const;
	// returns true if has no superior
	bool isTopLevel() const;
	// returns true if subgroup vector is not empty
	bool hasSubgroups() const;
	// appends to subgroups provided group if it is owned by this
	bool appendIfOwned(const std::shared_ptr<GroupEntity>&);
	// equal to this->id == o->superiorId
	bool owns(const std::shared_ptr<GroupEntity>&);
	// returns subgroup if has one with provided id
	const  std::shared_ptr<GroupEntity>& getSubgroupIfExists(IdInt id) const;
	// returns subgroup with index while checking bounds
	const std::shared_ptr<GroupEntity>& getSubgroup(int index) const;
	// returns count of inner vector
	int countSubgroups() const;
	// unique comparator, checks id equality
	bool operator==(const GroupEntity&) const;
};

// inner typedefs
typedef std::shared_ptr<GroupEntity> Group;
typedef QVector<Group> GroupList;

// can be used with QVariant
Q_DECLARE_METATYPE(Group);

class GroupTreeModel : public QAbstractTableModel
	// This model is non-standard, it contains multiple layers of the table, allowing
	// to step in and out
{
	Q_OBJECT
protected:
	GroupList topLayerList; // list of top layer groups
	QHash<uint, Group> topLayerLinker;	//	linker of top layer groups to avoid O(n) searches
	QStack<uint> layersIds;	//	stack of selected groups
	int currentVisibleLayer;	//	current layer
	Group currentGroup;	//	group, which subgroups are displayed now
public:
	using QAbstractTableModel::QAbstractTableModel;
	GroupTreeModel(const GroupList& clients, QObject* parent);
	// Inherited from QAbstractTableModel
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	void setList(const GroupList& l);
	// drops model to first layer
	void clearLayers();
public slots:
	// steps to next layer, if it exists. If not - groupSelected is emitted
	void stepToNextLayer(const QModelIndex& index);
	// steps to upper level
	void stepToUpperLevel();
signals:
	void groupSelected(const Group&);
	void backRequired();
};