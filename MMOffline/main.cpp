#include "MMOffline.h"
#include "Widgets/utils/ElementsStyles.h"
#include <QtWidgets/QApplication>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	allocateGlobalWorkset();
	GlobalAppSettings::instance()->packetTracing = true;
	MMOffline w;
	w.show();
	w.setMinimumSize(imitatePhoneSize(0.7));
	return a.exec();
}
