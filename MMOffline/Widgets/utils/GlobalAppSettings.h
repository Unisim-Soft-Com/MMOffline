#pragma once
#include <QtCore/QTranslator>
#include <QtCore/QDateTime>
#define AppSettings GlobalAppSettings::instance()

const float VERSION = 0.1;
const char SUFFIX[] = "alpha";
extern const char* LOCAL_LOAD_PATH;

namespace Translations
{
	enum Languages { English, Romanian, Russian };
	void operator++(Languages& l);

	extern const char* langPaths[Russian + 1];
	Languages langFromString(const QString&);
	QString langToString(const Languages);
}

class GlobalAppSettings
{
public:
	//	Stored vars
	QString HttpUrl;
	QStringList alternativeUrls;
	QString language;
	QString localLogin;
	QDate lastSyncDate;
	QString filePath;
	QTranslator translator;
	int timeoutint;

	// Launch-defined vars
	bool packetTracing;
	bool dbTracing;

	static GlobalAppSettings* _instance;

	// utility
	void setTranslator();
	void setTranslator(Translations::Languages lang);
	void dump();
	void  switchLaunchArgument(int arg);
	static GlobalAppSettings* instance();

	explicit GlobalAppSettings();
	~GlobalAppSettings();
};