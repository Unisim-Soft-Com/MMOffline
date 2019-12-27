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



class EntryCreationScreen : public inframedWidget
{
	Q_OBJECT
protected:
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

	NamedIdList measures;
	NamedIdList options;

	Product operatedProduct;

	DocumentEntry currentEntry;
public:
	EntryCreationScreen(QWidget* parent);
	void primeEntryCreation(Product p, Document d);
	virtual void show() override;
protected slots:
	void confirmed();
signals:
	void entryCreated(DocumentEntry);
};