#pragma once
#include "widgets/parents/inframedWidget.h"
#include <exception>
#include <QtCore/QString>

class CastFailedException : public std::exception
{
private:
	std::string msg = "Error upcasting inframed pointer with from type ";
public:
	CastFailedException(QString str) { msg += str.toStdString(); };
	virtual const char* what() const noexcept override { return msg.c_str(); };
};

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
	template <class T>
	T* _upCO()
	{
		T* temp = qobject_cast<T*>(currentlyOpened);
		if (temp == nullptr)
		{
			throw CastFailedException(currentlyOpened->objectName());
		}
		return temp;
	}
};
