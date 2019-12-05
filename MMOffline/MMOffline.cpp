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
	startingScreen(new StartingScreen(this)),
	myAwaiter(new RequestAwaiter(10000, this))
{
	mainLayout = new QVBoxLayout(this);
	this->setLayout(mainLayout);
	mainLayout->addWidget(startingScreen);
	currentlyOpened = startingScreen;
	untouchable = startingScreen;
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
	DataWorkset::instance()->networkingEngine->execQueryByTemplate(GetClients, QString::number(0), QString::number(50), "", myAwaiter);
	while (myAwaiter->isAwaiting())
	{
		qApp->processEvents();
	}
	auto parseres = RequestParser::parseAndInterpretListAs<ClientEntity>(myAwaiter->restext, myAwaiter->errtext);
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
	QVector<DataEntity> loaded = DataWorkset::instance()->dataprovider.loadDataAs<ClientEntity>();
	for (int i = 0; i < loaded.count(); ++i)
	{
		detrace_METHEXPL("\r\n" << "Loaded entity of type " << loaded.at(i)->myType());
		detrace_METHEXPL(" it will make query of insertion as: " << loaded.at(i)->insertionQuery());
	}
}
