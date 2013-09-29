#pragma once

#include <QtCore/QSettings>
#include <QtCore/QObject>
#include <QtCore/QDir>

#include "detailsParser.h"

class UpdateManager : public QObject
{
	Q_OBJECT
public:
	UpdateManager(QString updatesFolder, QObject *parent);
	~UpdateManager();

	void saveFileForLater(DetailsParser const *parser, QString const filePath);
	void loadUpdateInfo(QString const unit);

	QString version() const;
	QString fileName() const;
	QStringList arguments() const;

protected:
	void saveInfoFromParser(DetailsParser const *parser);

	QString const mUpdatesFolder;
	QString const settingsFile;
	QSettings *mUpdateInfo;
	QString mCurrentUnit;
	QString mVersion;
	QString mFileName;
	QStringList mArguments;
};

