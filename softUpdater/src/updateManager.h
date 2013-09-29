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

	bool hasPreparedUpdates();
	void loadUpdateInfo(QString const unit);

	Update* preparedUpdate();

protected:
	void saveInfoFromParser(DetailsParser const *parser);

	Update *mPreparedUpdate;
	QString const mUpdatesFolder;
	QString const settingsFile;
	QSettings *mUpdateInfo;
};

