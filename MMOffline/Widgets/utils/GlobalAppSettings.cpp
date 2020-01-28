#include "GlobalAppSettings.h"
#include <QtCore/QSettings>
#include <QtWidgets/qapplication.h>
// utility

GlobalAppSettings* GlobalAppSettings::_instance = nullptr;

#ifdef Q_OS_WIN
const char* LOCAL_LOAD_PATH = "/UNAOrders/";
#endif
#ifdef Q_OS_ANDROID
const char* LOCAL_LOAD_PATH = "/storage/emulated/0/UNAOrders/";
#endif

namespace Translations
{
	const char* langPaths[Russian + 1]
	{
		":/translations/mmoffline_en.qm",
		":/translations/mmoffline_ro.qm",
		":/translations/mmoffline_ru.qm"
	};
	void operator++(Languages& l)
	{
		if (l == Russian)
			l = English;
		else
			l = static_cast<Languages>(l + 1);
	}
	Languages langFromString(const QString& str)
	{
		switch (str.count())
		{
		case 7:
			if (str.startsWith(QChar('R')))
				return Russian;
			else
				return English;
		case 8:
			return Romanian;
		default:
			return English;
		}
	}
	QString langToString(const Languages l)
	{
		switch (l)
		{
		case Russian:
			return QStringLiteral("Russian");
		case Romanian:
			return QStringLiteral("Romanian");
		default:
			return QStringLiteral("English");
		}
	}
}

void GlobalAppSettings::setTranslator()
{
	using namespace Translations;
	translator.load(langPaths[langFromString(language)]);
	qApp->installTranslator(&translator);
}

void GlobalAppSettings::setTranslator(Translations::Languages lang)
{
	using namespace Translations;
	QString temp(langPaths[lang]);
	this->translator.load(langPaths[lang]);
	qApp->installTranslator(&translator);
	language = langToString(lang);
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
	setting.setValue("path_to_offline_load", filePath);
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

GlobalAppSettings* GlobalAppSettings::instance()
{
	if (_instance == nullptr)
	{
		_instance = new GlobalAppSettings();
	}
	return _instance;
}

GlobalAppSettings::GlobalAppSettings()
	: HttpUrl(), alternativeUrls(), language("English"),
	localLogin(), translator(), packetTracing(false), dbTracing(false)
{
	QSettings settings("settings.ini", QSettings::IniFormat);
	HttpUrl = settings.value("http_host", "http://www.una.md:3323/milkmark/").toString();
	language = settings.value("app_lang", "").toString();
	alternativeUrls = settings.value("alt_addresses", "").toStringList();
	localLogin = settings.value("local_user", "").toString();
	timeoutint = settings.value("timeout_interval", QVariant(10000)).toInt();
	lastSyncDate = settings.value("last_sync", QVariant(QDate())).toDate();
	filePath = settings.value("path_to_offline_load", LOCAL_LOAD_PATH).toString();
	setTranslator();
#ifdef DEBUG
	packetTracing = true;
	dbTracing = true;
#else
	packetTracing = false;
	dbTracing = false;
#endif
}

GlobalAppSettings::~GlobalAppSettings()
{
	dump();
}