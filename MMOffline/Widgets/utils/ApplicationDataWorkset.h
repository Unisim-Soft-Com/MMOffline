#pragma once
#include <QString>
#include "Networking/dataupdateengine.h"


struct DataWorkset
{
	DataUpdateEngine* networkingEngine;
	explicit DataWorkset();
};

extern DataWorkset globalWorkset;

void dumpGlobalWorkset(QString filename);
