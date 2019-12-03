#pragma once

#include <QtWidgets/QWidget>
#include "Networking/dataupdateengine-http.h"
#include "Networking/RequestAwaiter.h"
#include "Widgets/parents/abstractNodeInterface.h"
class MMOffline : public inframedWidget, abstractDynamicNode
{
	Q_OBJECT
private:
	RequestAwaiter* myAwaiter;
public:
	MMOffline();
public slots:
	void do_action();
};