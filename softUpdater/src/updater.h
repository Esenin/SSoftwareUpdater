#pragma once

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QSettings>
#include <QtCore/QProcess>
#include <QtCore/QTimer>
#include <QtCore/QDir>
#include <QDebug>

#include "downloader.h"
#include "xmlDataParser.h"
#include "updateManager.h"
#include "communicator.h"

class Updater : public QObject
{
	Q_OBJECT
public:
	Updater();
	~Updater();

public slots:
	void startUpdatingProcess();

protected:
	void initConnections();
	//! \brief parseParams
	//! \return True if arguments are correct, false otherwise
	bool parseParams();
	//! \brief hasNewUpdates
	//! \param newVersion
	//! \return True if new version is newer than current
	bool hasNewUpdates(QString const newVersion);
	void startSetupProgram(QString const filePath, QStringList const arguments);
	void saveFileForLater(QString const filePath);
	void checkPreparedUpdates();

	static int const criticalParamsCount = 3;
	static int const retryTimerout = 10 * 60 * 1000;
	static int const maxAttemptsCount = 3;
	int mCurAttempt;
	bool mHardUpdate;
	QString mUpdatesFolder;
	QTimer mRetryTimer;
	QProcess *mUpdateProcess;
	QMap<QString, QString> mParams;
	Downloader *mDownloader;
	DetailsParser *mParser;
	UpdateManager *mUpdateInfo;

protected slots:
	void detailsChanged();
	void fileReady(QString const filePath);
	void updateFinished(int exitCode, QProcess::ExitStatus status);
	void downloadErrors(QString error);
};

