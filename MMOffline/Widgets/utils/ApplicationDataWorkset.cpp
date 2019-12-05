#include "ApplicationDataWorkset.h"
#include <QSettings>
#include "Networking/dataupdateengine-http.h"

#include "Widgets/utils/GlobalAppSettings.h"


DataWorkset * globalWorkset;
DataWorkset::DataWorkset()
	: networkingEngine(), dataprovider(nullptr)
{
	networkingEngine = new HttpUpdateEngine(GlobalAppSettings::instance()->HttpUrl, nullptr);
}
DataWorkset* DataWorkset::instance()
{
	return globalWorkset;
}





void allocateGlobalWorkset()
{
	allocateGlobalSettings();
	globalWorkset = new DataWorkset();
}

void dumpGlobalWorkset(QString filename)
{
}
