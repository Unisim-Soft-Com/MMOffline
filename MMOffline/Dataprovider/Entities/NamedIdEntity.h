#pragma once
#include "abs_entity.h"
#include <QMetaType>
#include <QtCore/QVector>
#include <memory>

/*
	This entity is specialized on simpliest variation of outer entity - id with a name. 
	These entities should never be stored in their default table to avoid unnecessary mistakes.
*/


class NamedIdEntity : public abs_entity
	// represents any entity which has id and name
{
public:
	IdInt id;			
	QString name;	
protected:
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
public:
	explicit NamedIdEntity();
	explicit NamedIdEntity(QString Name, IdInt Id);
	explicit NamedIdEntity(const QStringList& List);
};

typedef std::shared_ptr<NamedIdEntity> NamedId;
typedef QVector<NamedId> NamedIdList;
//can be used with QVariant
Q_DECLARE_METATYPE(NamedId);


// These functions are used to find index of named id in provided list
// by name
int findNamedId(const QString& qstr, const NamedIdList& list);
// by id
int findNamedId(const int, const NamedIdList& list);
