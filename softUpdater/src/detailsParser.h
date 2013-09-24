#pragma once

#include <QtCore/QIODevice>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QMap>

//!
//! \brief The DetailsParser class
//! reads received file and provides fast access to main params
class DetailsParser : public QObject
{
	Q_OBJECT
public:
	DetailsParser(QString const unit);
	virtual ~DetailsParser(){}

	//! \brief version
	//! \return new version id
	QString version() const;

	//! \brief downloadAdress
	//! \return url of setup file
	QUrl downloadAdress() const;
	QStringList arguments() const;
	QString filename() const;

signals:
	void parseFinished();

public slots:
	virtual void parseDevice(QIODevice *device) = 0;

protected:
	QString const mUnitName;
	QString mVersionId;
	QUrl mDownloadUrl;
	QString mFileName;
	//! unit <-> url
	QMap<QString, QUrl> mFiles;
	QMap<QString, QString> mParamStrings;
};

