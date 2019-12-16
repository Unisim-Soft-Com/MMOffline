#include "DocumentEntity.h"
#include <QVariant>
#include<QWidget>

const QStringList fieldDefaults
{
QStringLiteral("0"),
QStringLiteral("12.12.2019"),
QStringLiteral("12.12.2019"),
QStringLiteral("0"),
QStringLiteral(""),
QStringLiteral("0"),
QStringLiteral(""),
QStringLiteral("0"),
QStringLiteral(""),
QStringLiteral("0.0")
};

uniform_json_object_representation DocumentEntity::toJsonRepresentation() const
{
	return uniform_json_object_representation(
		documentFields,
		QStringList
		{
			QString::number(documentId),
			dateWhenCreated.toString(),
			shippingDate.toString(),
			QString::number(clientId),
			clientName,
			QString::number(warehouseId),
			warehouseName,
			QString::number(documentType),
			documentTypeName,
			QString::number(alreadyPaid)
		}
	);
}

bool DocumentEntity::fromJsonRepr(const uniform_json_object_representation& o)
{
	if (o.size() == 0)
		return false;
	_listInit(o.mapValues(documentFields, fieldDefaults));
	return true;
}

TemplatedTableHandler* DocumentEntity::getAssocTable() const
{
	return predefinedTables + Documents;
}

QString DocumentEntity::getContentsForDb() const
{
	return	QStringLiteral("( ") +
		QString::number(documentId)
		+ QStringLiteral(" , \"") +
		dateWhenCreated.toString()
		+ QStringLiteral("\" , \"") +
		shippingDate.toString()
		+ QStringLiteral("\" , ") +
		QString::number(clientId)
		+ QStringLiteral(" , \"") +
		clientName
		+ QStringLiteral("\" , ") +
		QString::number(warehouseId)
		+ QStringLiteral(" , \"") +
		warehouseName
		+ QStringLiteral("\" , ") +
		QString::number(documentType)
		+ QStringLiteral(" , \"") +
		documentTypeName
		+ QStringLiteral("\" , ") +
		QString::number(alreadyPaid)
		+ QStringLiteral(" )");
}

abs_entity* DocumentEntity::fabricate() const
{
	return new DocumentEntity(*this);
}

bool DocumentEntity::fromSql(QueryPtr q)
{
	if (!q->next())
		return false;
	QStringList values;
	for (int i = 0; i < documentFields.count(); ++i)
	{
		values << q->value(i).toString();
	}
	_listInit(values);
	return true;
}

void DocumentEntity::_listInit(const QStringList& l)
{
	int i = l.count()-1;
	bool ok = true;
	switch (l.count())
	{
	case 10:
		alreadyPaid = l.at(i--).toDouble(&ok);
		if (!ok) break;
	case 9:
		documentTypeName = l.at(i--);
	case 8:
		documentType = l.at(i--).toInt(&ok);
		if (!ok) break;
	case 7:
		warehouseName = l.at(i--);
	case 6:
		warehouseId = l.at(i--).toInt(&ok);
		if (!ok) break;
	case 5:
		clientName = l.at(i--);
	case 4:
		clientId = l.at(i--).toInt(&ok);
		if (!ok) break;
	case 3:
		shippingDate =  QDate::fromString(l.at(i--));
	case 2:
		dateWhenCreated = QDate::fromString(l.at(i--));
	case 1:
		documentId = l.at(i--).toInt(&ok);
		if (!ok) break;
	default:
		ok = false;
	}
	if (!ok)
	{
		throw InitializationError(i);
	}
}

DocumentEntity::DocumentEntity()
	: abs_entity(Documents), documentId(0), dateWhenCreated(),
	shippingDate(), clientId(0), clientName(), warehouseId(0),
	warehouseName(), documentType(0), documentTypeName(), 
	alreadyPaid(0.0), linkedEntries()
{
}

DocumentEntity::DocumentEntity(int ID)
	: abs_entity(Documents), documentId(ID), dateWhenCreated(),
	shippingDate(), clientId(0), clientName(), warehouseId(0),
	warehouseName(), documentType(0), documentTypeName(),
	alreadyPaid(0.0), linkedEntries()
{
}

DocumentEntity::DocumentEntity(const QStringList& args)
{
	_listInit(args);
}

bool DocumentEntity::unlinkEntry(DocEntryPtr e)
{
	if (e != nullptr)
	{
		linkedEntries.removeOne(e);
		e->parentDocId = 0;
		return true;
	}
	return false;
}

void DocumentEntity::cleanEntryField()
{
	QVector<DocEntryPtr> temp;
	auto begin = linkedEntries.begin();
	while (begin != linkedEntries.end())
	{
		if ((*begin)->parentDocId != documentId)
		{
			temp.push_back((*begin));
		}
		++begin;
	}
	for (DocEntryPtr e : temp)
	{
		unlinkEntry(e);
	}
}

bool DocumentEntity::linkEntry(DocEntryPtr e)
{
	if (e->parentDocId != 0)
		return false;
	e->parentDocId = documentId;
	linkedEntries.push_back(e);
	return true;
}

bool DocumentEntity::hisEntry(DocEntryPtr e)
{
	return e->parentDocId == documentId;
}

DocumentsListModel::DocumentsListModel(const DocumentsList& doclist, QWidget* parent)
	: QAbstractListModel(parent), innerList(doclist)
{

}

int DocumentsListModel::rowCount(const QModelIndex& parent) const
{
	return innerList.count();
}

QVariant DocumentsListModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();
	if (index.row() >= rowCount())
		return QVariant();
	if (role == Qt::DisplayRole)
	{
		QVariant temp;
		temp.setValue<Document>(innerList.at(index.row()));
		return temp;
	}
	return QVariant();
}

QVariant DocumentsListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}