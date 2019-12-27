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
	GetProducts

};
const int totalTemplates = GetProducts + 1;
QMap<int, QString> _initTemplates();
bool _checkArgQuantity(queryIDs id, int argc);