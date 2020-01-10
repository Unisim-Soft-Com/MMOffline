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
	docroot(),
	startingScreen(new StartingScreen(this)),
	logsBranch(),
	myAwaiter(new RequestAwaiter(10000, this))
{
	mainLayout = new QVBoxLayout(this);
	this->setLayout(mainLayout);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	mainLayout->addWidget(startingScreen);
	currentlyOpened = startingScreen;
	untouchable = startingScreen;
	QObject::connect(_upCO<StartingScreen>(), &StartingScreen::documentCreationInitiated, this, &MMOffline::toDocumentRoot);
	QObject::connect(_upCO<StartingScreen>(), &StartingScreen::logsRequired, this, &MMOffline::toLogs);
}

void MMOffline::toDocumentRoot()
{
	_hideAnyWithDelete(new DocumentRootWidget(this));
	docroot = currentlyOpened;
	QObject::connect(currentlyOpened, &inframedWidget::backRequired, this, &MMOffline::toStart);
}

void MMOffline::toStart()
{
	_hideAndDeleteCurrent(untouchable);
}

void MMOffline::toLogs()
{
	_hideAnyWithDelete(new LogBranchRoot(this));
	logsBranch = currentlyOpened;
	QObject::connect(currentlyOpened, &inframedWidget::backRequired, this, &MMOffline::toStart);
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
	
	AppWorkset->networkingEngine->execQueryByAutofillTemplate(GetTips, myAwaiter);
	while (myAwaiter->isAwaiting())
	{
		qApp->processEvents();
	}
	detrace_METHEXPL("Groups result: " << myAwaiter->restext);
	auto parseres = RequestParser::parseAndInterpretListAs<NamedIdEntity>(myAwaiter->restext, myAwaiter->errtext);
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
	AppWorkset->dataprovider.pushData(parsedData, QStringLiteral( "Tips"));
}
