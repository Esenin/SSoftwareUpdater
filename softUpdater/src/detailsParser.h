#pragma once

#include <QtCore/QIODevice>
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>
#include <QtCore/QUrl>
#include <QtCore/QMap>

//!
//! \brief The DetailsParser class
//! reads received file and provides fast access to main params
class DetailsParser : public QObject
{
	Q_OBJECT
public:
	DetailsParser(){}
	virtual ~DetailsParser(){}

	virtual setUnitName(QString const currentUnit);

	//! \brief version
	//! \return new version id
	QString version() const;

	//! \brief downloadAdress
	//! \return url of setup file
	QUrl downloadAdress() const;
	QStringList arguments() const;
	QString filename() const;
	QStringList units() const;

signals:
	void parseFinished();

public slots:
	virtual void parseDevice(QIODevice *device) = 0;

protected:
	QString mCurrentUnit;
	QString mVersionId;
	QUrl mDownloadUrl;
	QString mFileName;
	QStringList mArguments;
	//! unit <-> url
	QMap<QString, QUrl> mFileUrls;
	QMap<QString, QString> mParamStrings;
	QMap<QString, QString> mVersions;
};

