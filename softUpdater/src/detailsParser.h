#pragma once

#include <QtCore/QObject>
#include <QtCore/QIODevice>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QMap>

//!
//! \brief The DetailsParser class
//! reads received file and provides fast access to main params
class DetailsParser : public QObject
{
	Q_OBJECT
public:
	DetailsParser(QString const platform);
	virtual ~DetailsParser(){}

	//! \brief version
	//! \return new version id
	QString version() const;

	//! \brief downloadAdress
	//! \return url of setup file
	QUrl downloadAdress() const;
	QString filename() const;

public slots:
	virtual void parseDevice(QIODevice *device) = 0;

protected:
	QString const mPlatformName;
	QString mVersionId;
	QUrl mDownloadUrl;
	QString mFileName;
	//! platform <-> url
	QMap<QString, QUrl> mFiles;
};

