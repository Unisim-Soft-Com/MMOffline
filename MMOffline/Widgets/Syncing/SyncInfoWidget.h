#pragma once
#include <QWidget>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGridLayout>




class SyncInfoWidget : public QWidget
{
	Q_OBJECT
public:
	enum syncSteps { 
		uploadStart=1,
		uploadEnd=100,
	
		downloadStart=101,
			GettingClients = 105,

		downloadEnd=200 
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

	void setLogins();
	void setDate();
public:
	SyncInfoWidget(QWidget* parent = nullptr);
	void setInfopack(QString & newLogin, QString & pendingChanges);
	void setErrorLog(QString& error);
	void reload();
public slots:
	void setProgress(int step);
};