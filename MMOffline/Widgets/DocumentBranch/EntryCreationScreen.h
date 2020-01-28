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
	This widget is representing a form for creating or editing entries. This widget
	will not insert entry into database.  This widget performs unique check on old entries.
	Creation can be primed with old entry or with product - document pair. This object 
	loads all data during creation. This widget	has unused option fields. 
	They can be usefull later.
	Tables affected:
	V		Measures, Options, Entries
	signals:
		entryCreated(DocumentEntry)
*/
class EntryCreationScreen : public inframedWidget
	//This class allows user to create or edit document entry
{
	Q_OBJECT
protected:
	// Subwidgets
	QVBoxLayout* mainLayout;
	QLabel* productInfo;
	QFormLayout* formLayout;
	QLabel* priceInfo;
	QComboBox* measureField;
	BigButtonsSpinbox* quantitySpinbox;
	QComboBox* foptionField;
	QComboBox* soptionField;
	QComboBox* toptionField;
	QLineEdit* commentField;
	QHBoxLayout* buttonLayout;
	MegaIconButton* backButton;
	MegaIconButton* okButton;

	// combo box contents
	NamedIdList measures;
	NamedIdList options;

	// product link
	Product operatedProduct;

	// current entry, wiped after editing end
	DocumentEntry currentEntry;
public:
	EntryCreationScreen(QWidget* parent);
	// Creates new entry filling it with data from product and document
	void primeEntryCreation(Product p, Document d);
	// Creates new entry using data from old one
	void primeEntryCreation(DocumentEntry);
	// Sets focus on quantity
	virtual void show() override;
protected slots:
	// fills entry object from form and emits it
	void confirmed();
signals:
	void entryCreated(DocumentEntry);
};