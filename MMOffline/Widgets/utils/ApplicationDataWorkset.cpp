#include "ApplicationDataWorkset.h"
#include <QSettings>
#include "Networking/dataupdateengine-http.h"


QString defaultUrl("http://www.una.md:3323/milkmark/");


DataWorkset::DataWorkset()
{
	networkingEngine = new HttpUpdateEngine(defaultUrl, nullptr);
}

DataWorkset globalWorkset;

void _initGlobalWorkset()
{
}


void dumpGlobalWorkset(QString filename)
{
}
