#pragma once
#include "Widgets/parents/abstractNodeInterface.h"


/*
	This widget is the main root widget of all application - it chooses which subbranch to open
	and this is all it's functionality. It saves a lot of memory and resources by deleting every branch
	which is not required now. Any core new branch must start here to benefit from dynamic allocation. 
	Also this widget does not operate on data. It even has no fields - except inherited ones.
*/


class MMOffline : public inframedWidget, abstractDynamicNode
	// main root widget
{
	Q_OBJECT
public:
	MMOffline();
public slots:
	// allocates and opens document root
	void toDocumentRoot();
	// allocates and opens start root
	void toStart();
	// allocates and opens logs root
	void toLogs();
};