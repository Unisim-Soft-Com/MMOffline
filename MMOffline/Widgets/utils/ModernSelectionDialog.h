#pragma once
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QFrame>
#include <QScroller>
#include <QScrollerProperties>
#include <QEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QElapsedTimer>

/*
    ModernSelectionDialog - Full-screen overlay dialog with centered
    card for item selection. Replaces native QComboBox on Android.
*/

class ModernSelectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ModernSelectionDialog(const QString& title,
                                   const QStringList& items,
                                   int currentIndex = 0,
                                   QWidget* parent = nullptr);

    int selectedIndex() const;
    QString selectedText() const;

protected:
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void onSearchTextChanged(const QString& text);
    void onItemClicked(QListWidgetItem* item);

private:
    void setupUI(const QString& title);
    void positionCard();
    void populateList();

    QStringList m_items;
    int m_selectedIndex;

    QFrame* cardFrame;
    QVBoxLayout* cardLayout;
    QFrame* headerFrame;
    QLabel* titleLabel;
    QPushButton* closeButton;
    QLineEdit* searchField;
    QListWidget* listWidget;

    QElapsedTimer m_scrollTimer;
    QPoint m_pressPos;
    bool m_wasScrolling;

    int m_loadedCount;
    static const int BATCH_SIZE = 15;
    void loadMoreItems();
};
