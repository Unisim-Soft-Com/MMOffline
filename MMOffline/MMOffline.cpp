#include "MMOffline.h"
#include "Widgets/DocumentBranch/DocumentRootWidget.h"
#include "Widgets/ModeDefining/StartingScreen.h"
#include "Widgets/LogBranch/LogBranchRoot.h"

MMOffline::MMOffline()
	: inframedWidget(nullptr),
	abstractDynamicNode(),
	docroot(),
	startingScreen(new StartingScreen(this)),
	logsBranch()
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