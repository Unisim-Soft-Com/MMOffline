#pragma once
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QtCore/QStringList>
#include <QIcon>
#include <QtWidgets/qgridlayout.h>

class AssertionLabel : public QWidget
{
	Q_OBJECT
private:
	QGridLayout* mainLayout;
	QVector<QLabel*> labels;

	QIcon errorIcon;
	QIcon okIcon;

	int topLvl;

	void _makeErrorLabel(const QString&);
	void _makeOkLabel();

public:
	AssertionLabel(QWidget* parent);
	void setErrorlist(const QStringList&);
	void clear();
};