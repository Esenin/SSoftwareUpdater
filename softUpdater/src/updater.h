#pragma once

#include <QtCore/QCoreApplication>

#include "downloader.h"
#include "xmlDataParser.h"

class Updater : public QObject
{
	Q_OBJECT
public:
	Updater();
	~Updater();

protected:


	Downloader *mDownloader;
	DetailsParser *mParser;

};

