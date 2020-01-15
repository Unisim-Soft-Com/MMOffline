#include "ApplicationDataWorkset.h"
#include <QSettings>
#include "Networking/dataupdateengine-http.h"

#include "Widgets/utils/GlobalAppSettings.h"

DataWorkset::DataWorkset()
	: networkingEngine(), dataprovider(nullptr)
{
	networkingEngine = new HttpUpdateEngine(GlobalAppSettings::instance()->HttpUrl, nullptr);
}
DataWorkset* DataWorkset::instance()
{
	if (_instanse == nullptr)
	{
		_instanse = new DataWorkset();
	}
	return _instanse;
}
DataWorkset* DataWorkset::_instanse = nullptr;