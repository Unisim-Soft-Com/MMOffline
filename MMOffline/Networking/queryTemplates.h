#pragma once
#include <QtCore/QMap>
#include <QtCore/QString>

enum queryIDs
{
	Ping,
	// system args
	GetGroups,
	// 1 arg
	GetTips,
	GetDepozits,
	GetMeasures,
	GetOptions,
	// 2 args
	Login,
	// 3 args
	GetClients,
	GetProducts,

	PostDocument,
	PostEntry
};
const int totalTemplates = PostEntry + 1;
QMap<int, QString> _initTemplates();
bool _checkArgQuantity(queryIDs id, int argc);