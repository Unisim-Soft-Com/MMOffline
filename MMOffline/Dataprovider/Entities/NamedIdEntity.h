#pragma once
#include "abs_entity.h"
#include <QMetaType>



class NamedIdEntity : public abs_entity
{
public:
	int id;
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
public:
	explicit NamedIdEntity();
	explicit NamedIdEntity(QString Name, int Id);
	explicit NamedIdEntity(const QStringList& List);
};

typedef std::shared_ptr<NamedIdEntity> NamedId;
typedef QVector<NamedId> NamedIdList;
Q_DECLARE_METATYPE(NamedId);