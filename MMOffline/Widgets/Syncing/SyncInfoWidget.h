#pragma once
#include <QWidget>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGridLayout>

/*
	This widget is used for displaying usefull info about synchronization process.
	It asserts login and sync date, also it operates over two progressbars as one.
	It does not affects database or internet, but it contains synchronization steps
	that are used in SyncMenu. It is not based on inframed.
*/

class SyncInfoWidget : public QWidget
	// shows synchronization progress and values
{
	Q_OBJECT
public:
	// defines current step of synchronization. This does not equals to progress, but it can be used to describe it
	enum syncSteps {
		uploadStart = 1,		//	upload started, but no actions done
		PostingDocument = 25,	//	phase when document is prepared for posting
		PostingEntry = 55,		//	phase when entry is prepared for posting
		uploadEnd = 100,		//	upload ended. Will show 100% if used as parameter to setProgress

		downloadStart = 101,	//	dowload started, but no queries sent
		GettingClients = 105,	//	phase when clients table is obtained
		GettingGroups = 115,	//	phase when groups table is obtained
		GettingTips = 140,		//	phase when tips table is obtained
		GettingDepozits = 145,	//	phase when depozits table is obtained
		GettingMeasures = 150,	//	phase when measures table is obtained
		GettingOptions = 155,	//	phase when options table is obtained
		GettingProducts = 160,  //	phase when clients table is obtained
		downloadEnd = 200		//	download ended. Will show 100% if used as parameter to setProgress
	};
private:
	QGridLayout* mainLayout;
	QLabel* currentLogin;
	QLabel* previousLogin;
	QLabel* lastSyncDate;
	QLabel* pendingChanges;
	QLabel* errorLog;
	QLabel* downloadInfo;
	QLabel* uploadInfo;
	QProgressBar* downloadProgress;
	QProgressBar* uploadProgress;

	// asserts date and sets right color to it
	void setDate();
public:
	SyncInfoWidget(QWidget* parent = nullptr);
	// sets all info to labels
	void setInfopack(QString& newLogin, QString& oldLogin, QString pendingChanges);
	// shows error log with provided text
	void setErrorLog(const QString& error);
	// reloads all info. Now only resets date
	void reload();
	// translates all texts 
	void fillTexts();
public slots:
	// sets progress bars as it was one progress bar from 0 to 200
	void setProgress(int step);
};
