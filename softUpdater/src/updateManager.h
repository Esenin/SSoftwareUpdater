#pragma once

#include <QtCore/QSettings>
#include <QtCore/QObject>

#include "detailsParser.h"

class UpdateManager : public QObject
{
	Q_OBJECT
public:
	UpdateManager(QString updatesFolder, QObject *parent);
	~UpdateManager();
	void saveFromParser(DetailsParser const *parser);
	void loadUpdateInfo(QString const unit);

	QString version() const;
	QString fileName() const;
	QStringList arguments() const;

protected:
	QString const settingsFile;
	QSettings *mUpdateInfo;
	QString mCurrentUnit;
	QString mVersion;
	QString mFileName;
	QStringList mArguments;
};

