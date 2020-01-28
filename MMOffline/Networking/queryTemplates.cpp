#include "queryTemplates.h"
#include <QtCore/QSettings>


const QString queryTemplates[totalTemplates]
{
	QStringLiteral("Ping"),
	QStringLiteral("GetGroups"),
	QStringLiteral("GetTips"),
	QStringLiteral("GetDepozits"),
	QStringLiteral("GetMeasures"),
	QStringLiteral("GetOptions"),
	QStringLiteral("Login"),
	QStringLiteral("GetClients"),
	QStringLiteral("GetProducts"),
	QStringLiteral("PostDocument"),
	QStringLiteral("PostEntry")
};

const QString queryDefaults[totalTemplates]
{
	QStringLiteral("ping"),
	QStringLiteral("pg_web_order.get_groups?p_request_id=%1&p_session_id=%2"),
	QStringLiteral("pg_web_order.get_reference?p_response_id=%1&p_session_id=%2&p_type=%3"),
	QStringLiteral("pg_web_order.get_reference?p_response_id=%1&p_session_id=%2&p_type=%3"),
	QStringLiteral("pg_web_order.get_reference?p_response_id=%1&p_session_id=%2&p_type=%3"),
	QStringLiteral("pg_web_order.get_reference?p_response_id=%1&p_session_id=%2&p_type=%3"),
	QStringLiteral("pg_web_base.login_user?r=%1&a=orders&u=%2&p=%3&p_only_session=1"),
	QStringLiteral("pg_web_order.get_clients?r=%1&s=%2&i=%3&n=%4&f=%5"),
	QStringLiteral("pg_web_order.get_products_by_userid?p_responce_id=%1&p_session_id="
	"%2&p_userid=%3&p_first_rn=%4&p_last_rn=%5"),
	QStringLiteral("pg_web_order.create_doc?p_session=%2&p_request_id=%1&p_doc_id=%3"
		"&p_created_date=%4&p_shipping_date=%5&p_client_id=%6&p_dep=%7&p_tip_doc=%8&p_payed=%9"),
	QStringLiteral("pg_web_order.add_entry?p_session=%2&p_request_id=%1&p_parent_doc_id=%3"
		"&p_entry_id=%4&p_product_id=%5&p_price=%6&p_measure=%7&p_qnt=%8&"
		"p_add_info1=%9&p_add_info2=%10&p_add_info3=%11&p_comment=%12")
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
	case GetGroups:
		return argc == 0;
	case GetTips:
	case GetDepozits:
	case GetMeasures:
	case GetOptions:
		return argc == 1;
	case Login:
		return argc == 2;
	case GetClients:
	case GetProducts:
		return argc == 3;
	case PostDocument:
		return argc == 7;
	case PostEntry:
		return argc == 10;
	default:
		return false;
	}
}