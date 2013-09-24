#pragma once

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QTimer>
#include <QDebug>

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
	QMap<QString, QString> mParams;
	Downloader *mDownloader;
	DetailsParser *mParser;

};

