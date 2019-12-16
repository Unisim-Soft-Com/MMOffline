#pragma once
#include "Widgets/parents/inframedWidget.h"
#include <QtWidgets/QLabel>
#include <QtWidgets/QBoxLayout>
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include "Networking/RequestAwaiter.h"
#include "SyncInfoWidget.h"

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
	void _send_data_request();
	bool _process_clients_response();
	bool _next_step();
	void _download_complete();
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