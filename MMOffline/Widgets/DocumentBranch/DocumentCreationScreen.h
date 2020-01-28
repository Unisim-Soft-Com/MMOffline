#pragma once
#include <QtWidgets/qboxlayout.h>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QtWidgets/QFormLayout>
#include "Widgets/ElementWidgets/MegaIconButton.h"
#include "Widgets/ElementWidgets/BigButtonsSpinbox.h"
#include "Widgets/parents/inframedWidget.h"
#include "Dataprovider/DataEntities.h"

/*
	This widget allows user to create document. This document will not be inserted into database 
	and will not have entries. Document will not be created if creation was not primed with client 
	object. This object loads all his data on creation.

	Affected tables:
	V:	Depozits, Tips

	signals:
		documentCreated(Document)	
*/


class DocumentCreationScreen : public inframedWidget
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	QLabel* clientInfo;
	QFormLayout* formLayout;
	QLabel* idInfo;
	BigButtonsSpinbox* dateSpinBox;
	QComboBox* depozitField;
	QComboBox* tipField;
	BigButtonsSpinbox* summPaidField;
	QHBoxLayout* buttonLayout;
	MegaIconButton* backButton;
	MegaIconButton* okButton;

	// lists with depozits and tips
	NamedIdList depozits;
	NamedIdList tips;

	// current document is created only when selection is primed
	Document currentDocument;
public:
	DocumentCreationScreen(QWidget*);
	// creates new document for provided client, then allows user to fill document data
	void primeCreation(Client);
	// sets focus 
	virtual void show() override;
protected slots:
	// activated when user confirms data
	void okPressed();
signals:
	void documentCreated(Document);
};