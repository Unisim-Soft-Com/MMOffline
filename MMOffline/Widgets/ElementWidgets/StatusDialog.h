#pragma once
#include <QDialog>
#include <QFrame>

class QLabel;
class QPushButton;

class StatusDialog : public QDialog {
    Q_OBJECT
public:
    explicit StatusDialog(const QString& message, QWidget* parent = nullptr);

    // Accepted  => OK/Retry
    // Rejected  => Cancel

    void setTitle(const QString& t);
    void setOkText(const QString& t);
    void setCancelText(const QString& t);
    void hideCancelButton();
    void setIconText(const QString& t);
    void setDangerMode(bool on);
    void setWarningMode(bool on);
    void setSuccessMode(bool on);
    int smartExec(QWidget* host = nullptr);

protected:
    void resizeEvent(QResizeEvent* e) override;

private:
    QLabel* titleLbl;
    QLabel* iconLbl;
    QLabel* msgLbl;
    QPushButton* cancelBtn;
    QPushButton* okBtn;
    QFrame* card;
    QWidget* backdrop = nullptr;

    void buildUi(const QString& message);
    void applyStyle();
};
