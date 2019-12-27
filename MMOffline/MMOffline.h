#pragma once

#include <QtWidgets/QWidget>
#include "Networking/dataupdateengine-http.h"
#include "Networking/RequestAwaiter.h"
#include "Widgets/parents/abstractNodeInterface.h"
#include "Widgets/ModeDefining/StartingScreen.h"
#include "Widgets/DocumentBranch/DocumentRootWidget.h"
#include "Widgets/MultibranchWidgets/GroupSelectionWidget.h"
#include "Widgets/MultibranchWidgets/ProductSelectionWidget.h"
#include "Widgets/DocumentBranch/DocumentCreationScreen.h"
class MMOffline : public inframedWidget, abstractDynamicNode
{
	Q_OBJECT
private:
	RequestAwaiter* myAwaiter;
	inframedWidget* startingScreen;
	inframedWidget* docroot;
public:
	MMOffline();
public slots:
	void do_action();
	void toDocumentRoot();
	void toStart();
};