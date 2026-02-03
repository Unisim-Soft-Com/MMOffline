#include "MMOffline.h"
#include "Widgets/utils/ElementsStyles.h"
#include <QtWidgets/QApplication>
#include <QIcon>

/*
	This is main function of an app. For details about app - check MMOffline class.
	Here is located only initiation of main app objects.

	In app exists 4 singletone classes and some static data. Their code works before this.
*/

int main(int argc, char* argv[])
{
	// initiating font adapter
#ifdef Q_OS_WIN
	// on windows lesser coefficients because of lesser height of the screent
	FontAdapter(15, 30, 0.03);
#endif

#ifdef Q_OS_ANDROID
	// on android larger coefficients because of larger screens
	FontAdapter(20, 60, 0.03);
#endif
	// must be here. DO NOT DELETE. DO NOT MOVE LATER THAN QApplication
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
	// window icon
	a.setWindowIcon(QIcon(":/res/UNAOrders.png"));
	// creating root widget
	MMOffline w;
	w.show();
#ifdef Q_OS_WIN
	// for windows - make lesser size
	w.setMinimumSize(imitatePhoneSize(0.7));
#endif
	// larger double click interval for bad and
	a.setDoubleClickInterval(1300);
	return a.exec();
}
