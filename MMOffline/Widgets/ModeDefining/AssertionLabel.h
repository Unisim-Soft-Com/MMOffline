#pragma once
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QtCore/QStringList>
#include <QIcon>
#include <QtWidgets/qgridlayout.h>

/*
	This label is suited for parsing error string lists. It will show OK label in case 
	when there was no errors and ERROR if something happened. This widget is independent from
	inframed widgets and branching system. 
*/

class AssertionLabel : public QWidget
{
	Q_OBJECT
private:
	// elements
	QGridLayout* mainLayout;
	QVector<QLabel*> labels;

	// icons to optimize disk work
	QIcon errorIcon;
	QIcon okIcon;

	// equals to quantity of labels/2, used to define in which row to place new label
	int topLvl;

	// constructs new error label, setting there text
	void _makeErrorLabel(const QString&);
	// constructs new ok label with default text
	void _makeOkLabel();

public:
	AssertionLabel(QWidget* parent);
	// sets error list and creates labels
	void setErrorlist(const QStringList&);
	// destroys all labels
	void clear();
};