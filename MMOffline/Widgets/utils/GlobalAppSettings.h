#pragma once

#include "networking/dataupdateengine-http.h"
#include <QtCore/QTranslator>

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
	QTranslator translator;
	int timeoutint;

	// Launch-defined vars
	bool packetTracing;
	bool dbTracing;
	

	// utility
	void setDefaultsToEmpty();
	void setTranslator();
	void dump();
	void  switchLaunchArgument(int arg);
	void parseLaunchArgs(int argc, char** argv);
	static GlobalAppSettings* instance();

	explicit GlobalAppSettings();
	~GlobalAppSettings();
};
extern GlobalAppSettings GlobalSettings;