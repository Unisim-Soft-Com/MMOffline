#pragma once
#include <QtWidgets/QWidget>
#include <QtGui/QKeyEvent>
#include "widgets/utils/EventsAndFilters.h"
#include <QJsonArray>
/*
		Main goal of this widget: uniform widget base which can react to native button presses.

		This widget is parent for everything what is going to use one-frame interface.
		This widgets reacts to some buttons with it's keyfilter that can be expanded to child widgets.
		Overload reaction methods for simple interactions. 
		This widget is carrying signal backRequired(), that is automatically emitted on back key press.

		WARNING
		Due to overusage of filters this widget base is slower than normal QWidget. To avoid performance
		loss on widgets which will never need reactions - use disableListening() or alternative constructor.


*/

class inframedWidget : public QWidget
	// key interactive widget base
{
	Q_OBJECT
protected:
	// filter used to find buttons presses
	QObject* keyfilter;

public:
	inframedWidget(QWidget* parent = nullptr);
	// will allow to define if reactions are required. Does not differ from qWidget if not listening
	inframedWidget(bool listen, QWidget* parent = nullptr);
	// will tell if this widget can be closed. Overload this for modal widgets.
	virtual bool back();
	// Overload this to change number press reaction
	virtual bool isExpectingControl(int);	//	Enhanced: now false is returned if value was not used
	// returns filter. You can use this filter to expand reactions to any other widgets
	QObject* getFilter() { return keyfilter; };
	// makes show() virtual to allow overloading with custom actions on show
	virtual void show();
	// installs other filter, deleting native
	void installEventFilter(QObject*);
	// removes filter. This greatly affects performance
	void disableListening();
	// equal to getFilter() == nullptr
	bool isListening();
protected slots:
	// overload this to change reaction to RETURN button
	virtual void returnReaction();
	// overload this to change reaction to ESCAPE, BACK buttons
	virtual void backReaction();
	// overload this to change raction to NUMBER buttons
	virtual void controlReaction(int);
signals:
	// emit this when you dont need this widget. By default, emitted on backReaction.
	void backRequired();
};

