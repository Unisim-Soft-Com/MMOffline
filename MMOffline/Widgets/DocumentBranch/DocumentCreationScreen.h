#pragma once
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QDateEdit>
#include <QFrame>
#include <QScrollArea>
#include "Widgets/parents/inframedWidget.h"
#include "Widgets/ElementWidgets/OutlineActionButton.h"
#include "Widgets/ElementWidgets/ModernAmountStepper.h"
#include "Widgets/utils/ModernDatePickerDialog.h"
#include "Dataprovider/DataEntities.h"
#include "Widgets/utils/ElidedComboBox.h"

/*
    This widget allows user to create document. This document will not be
    inserted into database and will not have entries. Document will not be
    created if creation was not primed with client object.

    REDESIGNED: Modern card-based UI matching UNAOrders design system.
    - Gradient header
    - Customer info banner with avatar
    - Styled cards for form fields
    - Modern footer with OutlineActionButtons

    Affected tables:
    V:  Depozits, Tips

    signals:
        documentCreated(Document)
*/

class DocumentCreationScreen : public inframedWidget
{
    Q_OBJECT

protected:
    // Main layout
    QVBoxLayout* mainLayout;

    // Header
    QFrame* headerFrame;
    QLabel* headerTitle;

    // Customer banner
    QFrame* customerBanner;
    QLabel* customerAvatar;
    QLabel* customerName;
    QLabel* customerAddress;
    QLabel* customerCode;

    // Content (scrollable)
    QScrollArea* scrollArea;
    QWidget* contentWidget;
    QVBoxLayout* contentLayout;

    // Card: Delivery & Document
    QFrame* deliveryCard;
    QLabel* dateDisplayLabel;
    QLabel* dateSubtitleLabel;
    QLabel* dateBadgeLabel;
    QDate m_currentDate;
    ElidedComboBox* depozitField;
    ElidedComboBox* tipField;

    // Card: Payment
    QFrame* paymentCard;
    ModernAmountStepper* summPaidField;

    // Footer
    QWidget* footerWidget;
    QHBoxLayout* footerLayout;
    OutlineActionButton* backButton;
    OutlineActionButton* okButton;

    // Data
    NamedIdList depozits;
    NamedIdList tips;
    Document currentDocument;

    // Setup methods
    void setupHeader();
    void setupCustomerBanner();
    void setupContent();
    void setupDeliveryCard();
    void setupPaymentCard();
    void setupFooter();
    void applyStyles();
    void updateDateDisplay();

    bool eventFilter(QObject* obj, QEvent* event) override;

public:
    DocumentCreationScreen(QWidget* parent);

    // Creates new document for provided client
    void primeCreation(Client client);

    // Sets focus
    virtual void show() override;

    // Translate all visible texts
    void fillTexts();

protected slots:
    void okPressed();

signals:
    void documentCreated(Document);
};
