#pragma once
#include "Widgets/parents/inframedWidget.h"
#include <QtWidgets/QLabel>
#include <QtWidgets/QBoxLayout>
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include "Networking/RequestAwaiter.h"
#include "SyncInfoWidget.h"
#include "Dataprovider/DataEntities.h"
class SyncMenuWidget : public inframedWidget
{
	Q_OBJECT
private:
	QVBoxLayout* mainLayout;
	SyncInfoWidget* syncInfo;
	QHBoxLayout* partialActionsLayout;
	MegaIconButton* sendButton;
	MegaIconButton* receiveButton;
	MegaIconButton* syncButton;
	MegaIconButton* backButton;

	RequestAwaiter* awaiter;
	
	int synchrostep;
	int entriesFrom;
	int entriesTo;

	ClientList dClients;
	int currentClientId;

	bool repeatingQuery;

	bool _send_data_request();
	bool _process_clients_response();
	bool _next_step();
	void _download_complete();
	bool _assertError(bool isErr, QString& errtext);
	QString _subdictionaryNameConversion();
public:
	SyncMenuWidget(QWidget* parent);
	void setLogin(QString& nlogin);
private slots:
	void sendData();
	void receiveData();
	void makeFullSync();
	void operate_on_response();
	void was_timeout();
signals:
	void syncCompleted();
};