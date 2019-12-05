#pragma once

#include "networking/dataupdateengine-http.h"
#include <QtCore/QTranslator>
#include <memory>

#define AppSettings GlobalAppSettings::instance()

const float VERSION = 0.001;
const char SUFFIX[] = "alpha";

class GlobalAppSettings
{
public:
	//	Stored vars
	QString HttpUrl;
	QStringList alternativeUrls;
	QString language;
	QString localLogin;
	QDate lastSyncDate;

	QTranslator translator;
	int timeoutint;

	// Launch-defined vars
	bool packetTracing;
	bool dbTracing;
	

	// utility
	void setTranslator();
	void dump();
	void  switchLaunchArgument(int arg);
	void parseLaunchArgs(int argc, char** argv);
	static std::shared_ptr<GlobalAppSettings> instance();

	explicit GlobalAppSettings();
	~GlobalAppSettings();
};
void allocateGlobalSettings();