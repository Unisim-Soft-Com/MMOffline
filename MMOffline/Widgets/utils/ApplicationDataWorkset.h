#pragma once
#include <QString>
#include "Networking/dataupdateengine.h"
#include "Dataprovider/SqliteDataProvider.h"

#define AppWorkset DataWorkset::instance()
struct DataWorkset
{
private:
	static DataWorkset* _instanse;
public:
	DataUpdateEngine* networkingEngine;
	SqliteDataProvider dataprovider;

	explicit DataWorkset();
	static DataWorkset* instance();
};
