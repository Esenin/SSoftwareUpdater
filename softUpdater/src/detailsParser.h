#pragma once

#include <QtCore/QIODevice>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QMap>

//!
//! \brief The DetailsParser class
//! reads received file and provides fast access to main params
class DetailsParser
{
public:
	DetailsParser(QString const platform, QIODevice *device);
	virtual ~DetailsParser(){}

	//! \brief version
	//! \return new version id
	int version() const;

	//! \brief downloadAdress
	//! \return url of setup file
	QUrl downloadAdress() const;
	QString filename() const;

protected:
	QString const mPlatformName;
	int mVersionId;
	QUrl mDownloadUrl;
	QString mFileName;
	//! platform <-> url
	QMap<QString, QUrl> mFiles;
};

