#include "queryTemplates.h"
#include <QtCore/QSettings>

const QString queryTemplates[totalTemplates]
{
	QStringLiteral("Ping"),
	QStringLiteral("GetWeight"),
	QStringLiteral("GetRouteList"),
	QStringLiteral("Login"),
	QStringLiteral("GetRoutes"),
	QStringLiteral("GetTask"),
	QStringLiteral("GetClients"),
	QStringLiteral("GetOrders"),
	QStringLiteral("GetShipOrders"),
	QStringLiteral("Getproducts")


};

const QString queryDefaults[totalTemplates]
{
	QStringLiteral("ping"),
	QStringLiteral("pg_web_order.get_weight?r=%1&s=%2"),
	QStringLiteral("pg_web_order.get_route_list?r=%1&s=%2"),
	QStringLiteral("pg_web_base.login_user?r=%1&a=orders&u=%2&p=%3&p_only_session=1"),
	QStringLiteral("pg_web_order.get_routes?r=%1&s=%2&i=%3&n=%4&f=%5"),
	QStringLiteral("pg_web_order.get_tasks?r=%1&s=%2&i=%3&n=%4&f=%5"),
	QStringLiteral("pg_web_order.get_clients?r=%1&s=%2&i=%3&n=%4&f=%5"),
	QStringLiteral("pg_web_order.get_orders?r=%1&s=%2&i=%3&n=%4&f=%5&hide=%6"),
	QStringLiteral("pg_web_order.get_ship_orders?r=%1&s=%2&i=%3&n=%4&f=%5&g1=%6"),
	QStringLiteral("pg_web_order.get_products?r=%1&s=%2&i=%3&n=%4&f=%5&g1=%6&g2=%7")

};


QMap<int, QString> _initTemplates()
{
	QMap<int, QString> map;
	QSettings templateStorage("request_templates.ini", QSettings::IniFormat);
	for (int i = 0; i < totalTemplates; ++i)
	{
		map[i] = templateStorage.value(queryTemplates[i]).toString();
		if (map[i].isEmpty())
		{
			templateStorage.setValue(queryTemplates[i], queryDefaults[i]);
			map[i] = queryDefaults[i];
		}
	}
	return map;
}

bool _checkArgQuantity(queryIDs id, int argc)
{
	switch (id)
	{
	case Ping:
	case GetWeight:
		return argc == 0;
	case Login:
		return argc == 2;
	case GetRoutes:
	case GetTask:
	case GetClients:
	case GetOrders:
		return argc == 3;
	case GetShipOrders:
	case GetProducts:
		return argc == 5;
	default:
		return false;
	}
}
