#include "SyncInfoWidget.h"
#include "Widgets/utils/GlobalAppSettings.h"
#include "Widgets/utils/ElementsStyles.h"
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif


const QColor freshLogin(Qt::green);
const QColor oldLogin(Qt::red);
void SyncInfoWidget::setDate()
{
	int diff = AppSettings->lastSyncDate.daysTo(QDate::currentDate());
	lastSyncDate->setStyleSheet(makeGradientStylesheet(freshLogin, oldLogin, 10, diff));
	lastSyncDate->setText(AppSettings->lastSyncDate.toString());
}

SyncInfoWidget::SyncInfoWidget(QWidget* parent)
	: QWidget(parent), mainLayout(new QGridLayout(this)),
	currentLogin(new QLabel(this)), previousLogin(new QLabel(this)),
	lastSyncDate(new QLabel(this)), pendingChanges(new QLabel(this)),
	errorLog(new QLabel(this)), downloadInfo(new QLabel(this)),
	uploadInfo(new QLabel(this)), downloadProgress(new QProgressBar(this)),
	uploadProgress(new QProgressBar(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(currentLogin, 0, 0);
	mainLayout->addWidget(previousLogin, 0, 1);
	mainLayout->addWidget(lastSyncDate, 1, 0);
	mainLayout->addWidget(pendingChanges, 1, 1);
	mainLayout->addWidget(errorLog, 2, 0, 0, 1);
	mainLayout->addWidget(downloadInfo, 3, 0, 1, 0);
	mainLayout->addWidget(downloadProgress, 4, 0, 1, 0);
	mainLayout->addWidget(uploadInfo, 5, 0, 1, 0);
	mainLayout->addWidget(uploadProgress, 6, 0, 1, 0);

	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	previousLogin->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	
	pendingChanges->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	currentLogin->setStyleSheet(FRAMED_LABEL_STYLESHEET);
	currentLogin->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	previousLogin->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	previousLogin->setStyleSheet(FRAMED_LABEL_STYLESHEET);
	lastSyncDate->setStyleSheet(FRAMED_LABEL_STYLESHEET);
	lastSyncDate->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	pendingChanges->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	pendingChanges->setStyleSheet(FRAMED_LABEL_STYLESHEET);
	errorLog->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	errorLog->setStyleSheet(ERROR_TEXT_STYLESHEET);
	downloadInfo->setAlignment(Qt::AlignCenter);
	downloadInfo->setText(tr("Download progress"));
	uploadInfo->setAlignment(Qt::AlignCenter);
	uploadInfo->setText(tr("Upload progress"));
	downloadProgress->setValue(0);
	downloadProgress->setMaximum(100);
	downloadProgress->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	downloadProgress->setAlignment(Qt::AlignCenter);
	uploadProgress->setMaximum(100);
	uploadProgress->setValue(0);
	uploadProgress->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	uploadProgress->setAlignment(Qt::AlignCenter);
	setDate();
}

void SyncInfoWidget::setInfopack(QString& newLogin, QString& pendingChange)
{
	currentLogin->setText(tr("Current user: ") + newLogin);
	previousLogin->setText(tr("Old user :") + AppSettings->localLogin);
	if (newLogin != AppSettings->localLogin)
		previousLogin->setStyleSheet(ERROR_TEXT_STYLESHEET);
	pendingChanges->setText(tr("Pending changes: ") + pendingChange);
}

void SyncInfoWidget::setErrorLog(const QString& error)
{
	errorLog->setText(error);
}

void SyncInfoWidget::reload()
{
	setDate();
}

void SyncInfoWidget::setProgress(int step)
{
	if (step < downloadStart)
	{
		uploadProgress->setValue(step);
	}
	else
	{
		downloadProgress->setValue(step - 100);
	}
}
