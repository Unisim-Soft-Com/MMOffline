#pragma once
#include <QtCore/QTranslator>
#include <QtCore/QDateTime>

/*
	This file contains application settings object, which is storing all global variables.
	It must be loaded on start and dumped after changes. Also here is stored translation tool set.
*/

#define AppSettings GlobalAppSettings::instance()

const float VERSION = 0.52;
const char SUFFIX[] = "beta";
// default path for loading database from csv files
extern const char* LOCAL_LOAD_PATH;

namespace Translations
	// here are stored translation tools
{
	// languages, supported for dynamic translation 
	enum Languages { English, Romanian, Russian };
	// cycling increment
	void operator++(Languages& l);

	// filenames of translations
	extern const char* langPaths[Russian + 1];
	// Lazy conversion to string, does not checks actual string meaning. By default, returns English
	Languages langFromString(const QString&);
	// converts language to string
	QString langToString(const Languages);
}

class GlobalAppSettings
	// this class stores all important global variables
{
public:
	//	Stored vars
	QString HttpUrl;			//	url base for attaching quaries to it
	QStringList alternativeUrls;	// additional urls for combobox
	QString language;			//	language of the application
	QString localLogin;			//	previously used login
	QDate lastSyncDate;			//	date of last synchronization
	QString filePath;			//	last used path to csv files
	QTranslator translator;		//	global translator
	int timeoutint;				//	timeout value 

	// Launch-defined vars
	bool packetTracing;			//	enables storing in logs info about sent and received queries
	bool dbTracing;				//	enables storing in logs all queries executed on database

	static GlobalAppSettings* _instance;

	// utility
	void setTranslator();
	// set ups translator using enum value to define which language to choose
	void setTranslator(Translations::Languages lang);
	// saves into ini file all variables
	void dump();
	// easy switcher for launch arguments. Not used now, meant to be used via command line arguments
	void  switchLaunchArgument(int arg);
	// returns global instanse
	static GlobalAppSettings* instance();

	// loads settings values from database
	explicit GlobalAppSettings();
	// DO NOT DUMP VALUES HERE (crashes, I tried)
	~GlobalAppSettings();
};