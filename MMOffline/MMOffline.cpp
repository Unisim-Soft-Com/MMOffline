#include "MMOffline.h"
#include "debugtrace.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
#include "Widgets/utils/GlobalAppSettings.h"
#include <qapplication.h>
#include "Networking/Parsers/LinearParser.h"
#include "Networking/Parsers/RequestParser.h"
#include "Dataprovider/DataEntities.h"
MMOffline::MMOffline()
	: inframedWidget(nullptr), 
	abstractDynamicNode(),
// docroot(),//	
docroot(new ProductSelectionWidget(this)),
	startingScreen(new StartingScreen(this)),
	myAwaiter(new RequestAwaiter(10000, this))
{
	mainLayout = new QVBoxLayout(this);
	this->setLayout(mainLayout);
	//
	mainLayout->addWidget(docroot);
//
	currentlyOpened = docroot;
//	
//	
	untouchable = docroot;
}

void MMOffline::do_action()
{
	detrace_METHCALL("do_action");
	GlobalAppSettings::instance()->packetTracing = true;
	DataWorkset::instance()->networkingEngine->initiateSession("Agent0", "123", myAwaiter);
	while (myAwaiter->isAwaiting())
	{
		qApp->processEvents();
	}
	detrace_METHEXPL("initiation return" << myAwaiter->restext);
	auto logres = RequestParser::getLoginResult(myAwaiter->restext, myAwaiter->errtext);
	detrace_METHEXPL("logres: " << logres.isError << " errors: " << logres.error << " and values " << logres.uid << " " << logres.session);
	AppWorkset->networkingEngine->setSession(logres.session, logres.uid);
	AppWorkset->dataprovider.dropTable(Groups);
	AppWorkset->networkingEngine->execQueryByTemplate(GetGroups, myAwaiter);
	while (myAwaiter->isAwaiting())
	{
		qApp->processEvents();
	}
	detrace_METHEXPL("Groups result: " << myAwaiter->restext);
	auto parseres = RequestParser::parseAndInterpretListAs<GroupEntity>(myAwaiter->restext, myAwaiter->errtext);
	if (parseres.isError)
	{
		detrace_METHPERROR("parseAndinterpret", parseres.error);
	}
	QVector<DataEntity>& parsedData = parseres.data;
	for (int i = 0; i < parsedData.count(); ++i)
	{
		detrace_METHEXPL("\r\n" << "Parsed entity of type " << parsedData.at(i)->myType());
		detrace_METHEXPL(" it will make query of insertion as: " << parsedData.at(i)->insertionQuery());
	}
	AppWorkset->dataprovider.pushData(parsedData);
}
