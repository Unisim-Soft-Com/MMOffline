#pragma once
#include "Widgets/parents/inframedWidget.h"
#include "Dataprovider/SqliteDataProvider.h"
#include "Widgets/utils/GlobalAppSettings.h"
#include <QtWidgets/QLabel>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFrame>
#include "Widgets/ModeDefining/AssertionLabel.h"

/*
    This widget represents screen for setting up system variables. It
    also allows user to load database from file. Here are displayed version
    and it's prefix. Here is possible to switch debugging mode which echoes
    all queries into log file.

    REDESIGNED: Modern card-based UI with header and footer buttons.

    Affected tables:
    EV			All outer
*/

class SettingsScreen : public inframedWidget
{
    Q_OBJECT
protected:
    // Main layout
    QVBoxLayout* mainLayout;

    // Header bar
    QFrame* headerBar;
    QLabel* headerTitle;

    // Content area with scroll capability
    QWidget* contentWidget;
    QVBoxLayout* contentLayout;

    // Card 1: Sync settings
    QFrame* cardSync;
    QLabel* lblSyncCaption;
    QLabel* urlBaseInfo;
    QComboBox* urlBaseField;
    QLabel* lblPathCaption;
    QLineEdit* filepathField;

    QFrame* makeLoadFromFile;
    QLabel* loadFileIcon;
    QLabel* loadFileText;

    // Card 2: Language & Timeout
    QFrame* cardLocale;
    QLabel* lblLangCaption;
    QLabel* langValueLabel;
    QPushButton* translateButton;
    QLabel* timeoutInfo;
    QSpinBox* timeoutField;

    // Card 3: Debug
    QFrame* cardDebug;
    QPushButton* debugMode;

    // Error log
    AssertionLabel* errorLog;

    // Footer bar
    QFrame* footerBar;
    QHBoxLayout* footerLayout;
    QPushButton* backButton;
    QPushButton* okButton;

    // Version info
    QLabel* appInfo;

    // Language icons (kept for compatibility, but using emoji now)
    QIcon langIcons[3];
    // Currently selected language
    Translations::Languages currentLanguage;

    // Language display names
    QString langDisplayNames[3];
    // Language emoji flags
    QString langFlags[3];

    // Translates all text on this page
    void fillTexts();

    // Apply stylesheet to this widget
    void applyStyles();

    // Create a styled card frame
    QFrame* createCard();

    bool eventFilter(QObject* obj, QEvent* event) override;

public:
    SettingsScreen(QWidget* parent);

private slots:
    // Saves everything
    void allConfirmed();
    // Attempts to create CSVParser and load database from provided path
    void loadFromFile();
    // Saves url picked in combobox
    void urlPicked(QString url);
    // Changes language button state, translates application and emits retranslation signal
    void translateAll();

signals:
    // Notifies outer branches that retranslation required
    void translating(int lang);
};
