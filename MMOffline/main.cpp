#include "MMOffline.h"
#include "Widgets/utils/ElementsStyles.h"
#include <QtWidgets/QApplication>
int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication a(argc, argv);
	allocateGlobalWorkset();
	GlobalAppSettings::instance()->packetTracing = true;
	AppSettings->lastSyncDate = QDate::currentDate();
	MMOffline w;
	w.show();
	w.setMinimumSize(imitatePhoneSize(0.7));
	//QTimer::singleShot(100, &w, SLOT(do_action()));
	return a.exec();
}
