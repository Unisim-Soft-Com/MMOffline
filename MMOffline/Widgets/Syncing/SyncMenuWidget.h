#pragma once
#include "Widgets/parents/inframedWidget.h"
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include "Networking/RequestAwaiter.h"
#include "SyncInfoWidget.h"
#include "Dataprovider/DataEntities.h"

/*
	This widget contains all code required for synchronization of the database. 
	It performs full wiping, then synchronically sends requests. After synchronization
	it unlocks buttons. 
	Affected tables:
	VE:			All
*/

class SyncMenuWidget : public inframedWidget
	// performs synchronization of the database via HTTP
{
	Q_OBJECT
private:
	// widgets
	QVBoxLayout* mainLayout;
	SyncInfoWidget* syncInfo;
	QHBoxLayout* partialActionsLayout;
	MegaIconButton* sendButton;
	MegaIconButton* receiveButton;
	MegaIconButton* syncButton;
	MegaIconButton* backButton;

	// awaiter required for incapsulated async work
	RequestAwaiter* awaiter;

	// this string used to track login differences - it holds login used before last login attempt
	QString previousLogin;

	// This variable is used to track status of synchronization and to pick right action in minor functions
	int synchrostep;

	// these variables used to limit next page of repeating quary
	int entriesFrom;
	int entriesTo;

	// possibly renundant
	ClientList dClients;
	int currentClientId;

	// this flag defines that next query should repeat itself without calling hext_step()
	bool repeatingQuery;

	// this list contains all documents required to be sent
	DocumentsList docsToUpload;

	// this index points on the document which must be sent
	int currentDoc;

	// this index points to the entry which must be sent
	int currentEntry;

	// this flag defines that upload end must call download start
	bool isFullSync;

	// this function checks if upload or download should end
	void _assertEnd();
	// this function counts upload progress by scaling it to % of sent documents
	int _count_upload_progress();
	// this function sends request which corresponds to current synchrostep
	bool _send_data_request();
	// this functions interprets server response 
	bool _process_clients_response();
	// this function is trying to increase synchrostep to next phase of synchronization
	bool _next_step();
	// this function commits database changes
	void _download_complete();
	// this function performs wipe of sent values
	void _upload_complete();
	// this function checks request result and shows errors on infoWidget. Also it drops synchrostep to 0
	bool _assertError(bool isErr, QString& errtext);
	// this function extracts subdictionary name from synchrostep
	QString _subdictionaryNameConversion();
	// this function extracts data from current document and sends request
	bool _post_document();
	// this function extracts data from current entry and sends request
	bool _post_entry();
public:
	SyncMenuWidget(QWidget* parent);
	// This function refreshes infoWidget
	void setLogin(QString& nlogin, QString & ologin);
	// This function retranslates all
	void fillTexts();
private slots:
	// this slot is blocking buttons and prepares sending
	void sendData();
	// this slot is blocking buttons and prepares receiving
	void receiveData();
	// this slot raises isFullSync and calls sendData()
	void makeFullSync();
	// this slot is used to select next action over received response
	void operate_on_response();
	// this slot is setting up timeout error
	void was_timeout();
signals:
	// is emitted on sync completion
	void syncCompleted();
};