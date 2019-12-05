#pragma once
#include <QString>
#include "Networking/dataupdateengine.h"
#include "Dataprovider/SqliteDataProvider.h"

struct DataWorkset
{
	DataUpdateEngine* networkingEngine;
	SqliteDataProvider dataprovider;

	explicit DataWorkset();
	static DataWorkset* instance();
};

void allocateGlobalWorkset();
void dumpGlobalWorkset(QString filename);
