#pragma once

#include <QtWidgets/QWidget>
#include "Networking/dataupdateengine-http.h"
#include "Networking/RequestAwaiter.h"
#include "Widgets/parents/abstractNodeInterface.h"
#include "Widgets/ModeDefining/StartingScreen.h"
#include "Widgets/DocumentBranch/DocumentRootWidget.h"
#include "Widgets/MultibranchWidgets/GroupSelectionWidget.h"
#include "Widgets/MultibranchWidgets/ProductSelectionWidget.h"
class MMOffline : public inframedWidget, abstractDynamicNode
{
	Q_OBJECT
private:
	RequestAwaiter* myAwaiter;
	StartingScreen* startingScreen;
	ProductSelectionWidget* docroot;
public:
	MMOffline();
public slots:
	void do_action();
};