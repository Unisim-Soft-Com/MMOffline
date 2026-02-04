#pragma once
#include "Networking/RequestAwaiter.h"
#include "Widgets/ElementWidgets/FooterButton.h"
#include "Widgets/parents/abstractNodeInterface.h"
#include "Widgets/Syncing/SyncMenuWidget.h"
#include <QtWidgets/QLineEdit>
#include <QFrame>
#include <QScrollArea>

/*
    Modern login widget with card-based design.
    Features:
    - Header with title
    - Logo display
    - Card container with styled inputs
    - Icon indicators for username/password
    - Show/hide password toggle
    - Footer buttons (Back/Login)
    - Error display via StatusDialog
*/
class OnlineLoginWidget : public inframedWidget, abstractNode
{
    Q_OBJECT
protected:
    // Main layout elements
    QVBoxLayout* mainLayout;

    // Header
    QFrame* headerFrame;
    QLabel* headerTitle;

    // Content area
    inframedWidget* contentWidget;
    QVBoxLayout* contentLayout;

    // Logo section
    QFrame* logoFrame;
    QLabel* logoIcon;
    QLabel* appNameLabel;
    QLabel* versionLabel;
    QWidget* logoContainer;

    // Login card
    QFrame* loginCard;
    QVBoxLayout* cardLayout;

    // Username field
    QLabel* loginLabel;
    QFrame* loginInputFrame;
    QHBoxLayout* loginInputLayout;
    QLabel* loginIconLabel;
    QLineEdit* loginField;

    // Password field
    QLabel* passwordLabel;
    QFrame* passwordInputFrame;
    QHBoxLayout* passwordInputLayout;
    QLabel* passwordIconLabel;
    QLineEdit* passwordField;
    QPushButton* togglePasswordBtn;

    // Footer buttons
    QWidget* footerWidget;
    QHBoxLayout* footerLayout;
    FooterButton* backButton;
    FooterButton* okButton;

    // Network tools
    RequestAwaiter* awaiter;
    QString pendingLogin;

    // Password visibility state
    bool passwordVisible;

    // Synchronization subbranch
    SyncMenuWidget* syncMenu;

    // Scrolling
    QScrollArea* scrollArea;

public:
    OnlineLoginWidget(QWidget* parent);

    // Sets focus on right field
    virtual void show() override;

    // Reloads text with actual translation
    void fillTexts();

protected slots:
    // Activated on password confirmation
    void passwordConfirmed();

    // Asserts response, continues to synchronization if successful
    void processResponse();

    // Activated on request timeouts
    void was_timeout();

    // Performs back navigation
    void hideCurrent();

    // Toggle password visibility
    void togglePasswordVisibility();

signals:
    void loginReady(QString);

private:
    void setupHeader();
    void setupContent();
    void setupLogo();
    void setupLoginCard();
    void setupFooter();
    void applyStyles();
    void updatePasswordToggleIcon();
};
