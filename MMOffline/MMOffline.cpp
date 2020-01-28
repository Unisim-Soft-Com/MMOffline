#include "MMOffline.h"
#include "Widgets/DocumentBranch/DocumentRootWidget.h"
#include "Widgets/ModeDefining/StartingScreen.h"
#include "Widgets/LogBranch/LogBranchRoot.h"

MMOffline::MMOffline()
	: inframedWidget(nullptr),
	abstractDynamicNode(new StartingScreen(this), new QVBoxLayout(this))
{
	// binding layout
	this->setLayout(mainLayout);

	// connecting slots using upcasting current. We don't need to connect anything except current because it's all unallocated.
	QObject::connect(_upCO<StartingScreen>(), &StartingScreen::documentCreationInitiated, this, &MMOffline::toDocumentRoot);
	QObject::connect(_upCO<StartingScreen>(), &StartingScreen::logsRequired, this, &MMOffline::toLogs);
}

void MMOffline::toDocumentRoot()
{
	_hideAnyWithDelete(new DocumentRootWidget(this));
	QObject::connect(currentlyOpened, &inframedWidget::backRequired, this, &MMOffline::toStart);
}

void MMOffline::toStart()
{
	_hideAndDeleteCurrent(untouchable);
}

void MMOffline::toLogs()
{
	_hideAnyWithDelete(new LogBranchRoot(this));
	QObject::connect(currentlyOpened, &inframedWidget::backRequired, this, &MMOffline::toStart);
}