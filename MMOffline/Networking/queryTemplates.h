#pragma once
#include <QtCore/QMap>
#include <QtCore/QString>

/*
	This header contains query templates and enumeration for access to them.

	Tips: comment for query ID  shows how much arguments it requires. 
		s means system args, it requires session id
		ns means no sys args, it sends query without session id
*/
enum queryIDs
{
	Ping,					//	sends ping to server 0ns
	// system args			
	GetGroups,				//	request for groups 0s
	// 1 arg
	GetTips,				//	request for types of document 1s
	GetDepozits,			//	request for warehouses 1s
	GetMeasures,			//	request for measures 1s
	GetOptions,				//	request for options 1s
	// 2 args
	Login,					//	request for system variables, 2ns
	// 3 args
	GetClients,				//	request for the clients, 3s
	GetProducts,			//	request for the products, 3s
	// post requests
	PostDocument,			//  post document, 7s
	PostEntry				//	post entry, 10s
};
const int totalTemplates = PostEntry + 1;
// load from ini file all templates, create this file if it does not exists
QMap<int, QString> _initTemplates();
// check argument quantity for embedded queries. You can add queries yourself
bool _checkArgQuantity(queryIDs id, int argc);