#pragma once
#include <QtCore/QMap>
#include <QtCore/QString>


enum queryIDs
{
	Ping,
	// system args
	GetWeight,
	GetRouteList,
	GetGroups,
	// 2 args + system args
	Login,
	// 3 args
	GetRoutes,
	GetTask,
	GetClients,
	GetOrders,
	// 5 args
	GetShipOrders,
	// 6 args
	GetProducts


};
const int totalTemplates = GetProducts + 1;
QMap<int, QString> _initTemplates();
bool _checkArgQuantity(queryIDs id, int argc);