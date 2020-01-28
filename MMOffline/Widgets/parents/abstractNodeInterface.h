#pragma once
#include "widgets/parents/inframedWidget.h"
#include <exception>
#include <QtCore/QString>

/*
	This file is containing two kinds of widget tree nodes. First one is static node,
	it expects all widgets to exist. Do not use it if you can not create all subbranches.
	Second one is dynamic node, which allows to dynamically emplace widget subbranch onto 
	layout and to delete it on replacement. Also second one allows to upcast it's contents.

	WARNING!
	dynamic node WILL NOT connect slots for you. Never forget to connect them after creation
	manually.
*/

class CastFailedException : public std::exception
	// is thrown if cast failed during upcasting widget
{
private:
	std::string msg = "Error upcasting inframed pointer with from type ";
public:
	CastFailedException(QString str) { msg += str.toStdString(); };
	virtual const char* what() const noexcept override { return msg.c_str(); };
};

class abstractNode
	// provides interfaces for widget tree node - hiding, showing, replacing
{
protected:
	// widget which is now shown
	inframedWidget* current;
	// widget which considered as root one
	inframedWidget* untouchable;
	// widget which considered as expecting focus after hiding all subbranches
	inframedWidget* main;

	abstractNode() : current(Q_NULLPTR), untouchable(Q_NULLPTR), main(Q_NULLPTR) {};
	abstractNode(inframedWidget* c, inframedWidget* u, inframedWidget* m) : current(c), untouchable(u), main(m) {};
public:
	// hides current widget with respect to root one, then shows replacement
	virtual bool _hideCurrent(inframedWidget* replacement);
	// hides any widget
	virtual void _hideAny(inframedWidget* replacement);
};

class abstractDynamicNode
	// provides interfaces for dynamic node - auto emplacing of widgets, auto deleting
{
protected:
	// layout where widgets are emplaced
	QLayout* mainLayout;
	// widget which is now shown
	inframedWidget* currentlyOpened;
	// widget which must never be deleted (root one)
	inframedWidget* untouchable;
public:
	abstractDynamicNode(inframedWidget* untouch = nullptr, QLayout* mLayout = nullptr);
	// hides current, but respects root widget
	virtual void _hideAndDeleteCurrent(inframedWidget* replacement);
	// hides any widget. Use this if your root is too heavy or never used later
	virtual void _hideAnyWithDelete(inframedWidget* replacement);
	template <class T>
	T* _upCO()
		// upcasts currently opened widget, thus allowing you to connect it as normal
	{
		T* temp = qobject_cast<T*>(currentlyOpened);
		if (temp == nullptr)
		{
			throw CastFailedException(currentlyOpened->objectName());
		}
		return temp;
	}
};
