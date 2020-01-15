#include "MMOffline.h"
#include "Widgets/utils/ElementsStyles.h"
#include <QtWidgets/QApplication>
#include <QIcon>
int main(int argc, char* argv[])
{
#ifdef Q_OS_WIN
	FontAdapter(15, 30, 0.03);
#endif
#ifdef Q_OS_ANDROID
	FontAdapter(20, 60, 0.03);
#endif
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication a(argc, argv);
	a.setWindowIcon(QIcon(":/res/UNAOrders.png"));
	MMOffline w;
	w.show();
#ifdef Q_OS_WIN
	w.setMinimumSize(imitatePhoneSize(0.7));
#endif
	a.setDoubleClickInterval(1300);
	return a.exec();
}