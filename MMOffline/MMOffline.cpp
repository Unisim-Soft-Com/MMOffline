#include "MMOffline.h"
#include "debugtrace.h"
#include "Widgets/utils/ApplicationDataWorkset.h"
#include "Widgets/utils/GlobalAppSettings.h"
#include <qapplication.h>
MMOffline::MMOffline()
	: inframedWidget(nullptr), myAwaiter(new RequestAwaiter(10000, this))
{
}

void MMOffline::do_action()
{
	detrace_METHCALL("do_action");
	GlobalAppSettings::instance()->packetTracing = true;
	globalWorkset.networkingEngine->initiateSession("Agen", "23", myAwaiter);
	while (myAwaiter->isAwaiting())
	{
		qApp->processEvents();
	}
	globalWorkset.networkingEngine->execQueryByTemplate(GetWeight, myAwaiter);
}
