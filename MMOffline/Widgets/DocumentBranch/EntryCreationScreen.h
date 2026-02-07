#pragma once
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFrame>
#include <QScrollArea>
#include "Widgets/parents/inframedWidget.h"
#include "Widgets/ElementWidgets/OutlineActionButton.h"
#include "Widgets/ElementWidgets/ModernAmountStepper.h"
#include "Dataprovider/DataEntities.h"
#include "Widgets/utils/ElidedComboBox.h"

/*
    Modern product entry creation/editing screen.
    Card-based layout matching the app's modernized design language.
    Uses ElidedComboBox + eventFilter + ModernSelectionDialog for dropdowns.

    REDESIGNED: Following exact DocumentCreationScreen patterns:
    - ObjectName-based styling via applyStyles()
    - Gradient header with adaptive height
    - Product info banner (like customer banner)
    - Form cards with #formCard objectName
    - Shadow only on non-Android
    - Modern footer with OutlineActionButtons

    Tables affected:
    V		Measures, Options, Entries
    signals:
        entryCreated(DocumentEntry)
*/
class EntryCreationScreen : public inframedWidget
{
    Q_OBJECT
protected:
    // Main layout
    QVBoxLayout* mainLayout;

    // Header
    QFrame* headerFrame;
    QLabel* headerTitle;

    // Product banner
    QFrame* productBanner;
    QLabel* productAvatar;
    QLabel* productNameLabel;
    QLabel* productCodeLabel;
    QLabel* productPriceBadge;

    // Content (scrollable)
    QScrollArea* scrollArea;
    QWidget* contentWidget;
    QVBoxLayout* contentLayout;

    // Card: Details (measure + option)
    QFrame* detailsCard;
    ElidedComboBox* measureField;
    ElidedComboBox* optionField;

    // Card: Quantity
    QFrame* quantityCard;
    ModernAmountStepper* quantityStepper;

    // Card: Comment
    QFrame* commentCard;
    QLineEdit* commentField;

    // Footer
    QWidget* footerWidget;
    QHBoxLayout* footerLayout;
    OutlineActionButton* backButton;
    OutlineActionButton* okButton;

    // Data
    NamedIdList measures;
    NamedIdList options;
    Product operatedProduct;
    DocumentEntry currentEntry;

    // Hidden option indices for data compatibility
    int currentSOptionIndex;
    int currentTOptionIndex;

    // Setup methods
    void setupHeader();
    void setupProductBanner();
    void setupContent();
    void setupDetailsCard();
    void setupQuantityCard();
    void setupCommentCard();
    void setupFooter();
    void applyStyles();
    void updateProductBanner();

    bool eventFilter(QObject* obj, QEvent* event) override;

public:
    EntryCreationScreen(QWidget* parent);

    // Creates new entry filling it with data from product and document
    void primeEntryCreation(Product p, Document d);
    // Creates new entry using data from old one
    void primeEntryCreation(DocumentEntry);
    // Sets focus on quantity
    virtual void show() override;

    // Translate all visible texts
    void fillTexts();

protected slots:
    void confirmed();

signals:
    void entryCreated(DocumentEntry);
};
