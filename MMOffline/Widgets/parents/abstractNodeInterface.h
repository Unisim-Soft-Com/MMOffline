#pragma once
#include <QtCore/QPointer>
#include "widgets/parents/inframedWidget.h"

class abstractNode
{
protected:
	inframedWidget* current;
	inframedWidget* untouchable;
	inframedWidget* main;

	abstractNode() : current(Q_NULLPTR), untouchable(Q_NULLPTR), main(Q_NULLPTR) {};
	abstractNode(inframedWidget* c, inframedWidget* u, inframedWidget* m) : current(c), untouchable(u), main(m) {};
public:
	virtual bool _hideCurrent(inframedWidget* replacement);
	virtual void _hideAny(inframedWidget* replacement);
};

class abstractDynamicNode
{
protected:
	QLayout* mainLayout;
	inframedWidget* currentlyOpened;
	inframedWidget* untouchable;
public:
	abstractDynamicNode();
	virtual void _hideAndDeleteCurrent(inframedWidget* replacement);
	virtual void _hideAnyWithDelete(inframedWidget* replacement);
};