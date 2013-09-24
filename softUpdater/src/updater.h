#pragma once

#include <QtCore/QCoreApplication>
#include <QtCore/QProcess>
#include <QtCore/QStringList>
#include <QtCore/QTimer>

#include "downloader.h"
#include "xmlDataParser.h"

class Updater : public QObject
{
	Q_OBJECT
public:
	Updater();
	~Updater();

protected:
	//! \brief parseParams
	//! \return True if arguments are correct, false otherwise
	bool parseParams();
	//! \brief hasNewUpdates
	//! \param newVersion
	//! \return True if new version is newer than current
	bool hasNewUpdates(QString const newVersion);

	static int const criticalParamsCount = 3;
	bool mSoftUpdate;
	QProcess *mUpdater;
	QMap<QString, QString> mParams;
	Downloader *mDownloader;
	DetailsParser *mParser;

protected slots:
	void detailsChanged();
	void fileReady(QString const filePath);
	void updateFinished(int exitCode, QProcess::ExitStatus status);

};

