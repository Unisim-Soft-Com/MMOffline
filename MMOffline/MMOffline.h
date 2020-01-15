#pragma once
#include "Widgets/parents/abstractNodeInterface.h"

class MMOffline : public inframedWidget, abstractDynamicNode
{
	Q_OBJECT
private:
	inframedWidget* startingScreen;
	inframedWidget* docroot;
	inframedWidget* logsBranch;
public:
	MMOffline();
public slots:
	void toDocumentRoot();
	void toStart();
	void toLogs();
};