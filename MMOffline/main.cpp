#include "MMOffline.h"
#include <QtWidgets/QApplication>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MMOffline w;
	w.show();
	QTimer::singleShot(100, &w, SLOT(do_action()));
	return a.exec();
}
