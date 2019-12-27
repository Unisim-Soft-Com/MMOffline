#include "GlobalAppSettings.h"
#include <QtCore/QSettings>
#include <QtWidgets/qapplication.h>
#include <cstring>
// utility

std::shared_ptr<GlobalAppSettings> globalSettings;

const char* networkTraceFlag = "-nptr";
const char* databaseTrackFlag = "-dbtr";	
const int totalFlags = 2;
const char * launchFlags[totalFlags] = {networkTraceFlag, databaseTrackFlag };

void GlobalAppSettings::setTranslator()
{
	if (this->language == "Russian")
	{
		this->translator.load(":/translations/uamobi_ru.qm");
	}
	else if (this->language == "Romanian")
	{
		this->translator.load(":/translations/MMOffline_ro.qm");
	}
	else
	{
		this->translator.load(":/translations/MMOffline_en.qm");
	}
	qApp->installTranslator(&translator);
}

void GlobalAppSettings::dump()
{
	QSettings setting("settings.ini", QSettings::IniFormat);
	setting.setValue("app_lang", language);
	setting.setValue("http_host", HttpUrl);
	setting.setValue("alt_addresses", alternativeUrls);
	setting.setValue("local_user", localLogin);
	setting.setValue("timeout_interval", timeoutint);
	setting.setValue("last_sync", lastSyncDate);
}

void GlobalAppSettings::switchLaunchArgument(int arg)
{
	switch (arg)
	{
	case 0:
		packetTracing = !packetTracing;
		break;
	case 1:
		dbTracing = !dbTracing;
		break;
	}
}

void GlobalAppSettings::parseLaunchArgs(int argc, char** argv)
{
	for (int i = 0; i < argc; ++i)
	{
		if (strlen(argv[i]) == strlen(databaseTrackFlag))
		{
			for (int j = 0; j < totalFlags; ++j)
			{
				if (std::strncmp(argv[i], launchFlags[j], strlen(databaseTrackFlag)))
				{
					switchLaunchArgument(j);
					break;
				}
			}
		}
	}
}

std::shared_ptr<GlobalAppSettings> GlobalAppSettings::instance()
{
	return globalSettings;
}

GlobalAppSettings::GlobalAppSettings()
	:	HttpUrl(), alternativeUrls(), language("English"),
	localLogin(), translator(), packetTracing(false), dbTracing(false)
{
	QSettings settings("settings.ini", QSettings::IniFormat);
	HttpUrl = settings.value("http_host", "http://www.una.md:3323/milkmark/").toString();
	language = settings.value("app_lang", "").toString();
	alternativeUrls = settings.value("alt_addresses", "").toStringList();
	localLogin = settings.value("local_user", "").toString();
	timeoutint = settings.value("timeout_interval", QVariant(10000)).toInt();
	lastSyncDate = settings.value("last_sync", QVariant(QDate())).toDate();
	setTranslator();
}

GlobalAppSettings::~GlobalAppSettings()
{
	dump();
}

void allocateGlobalSettings()
{
	globalSettings = std::shared_ptr<GlobalAppSettings>(new GlobalAppSettings());
}
