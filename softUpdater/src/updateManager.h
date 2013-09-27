#pragma once

#include <QtCore/QSettings>
#include <QtCore/QObject>

#include "detailsParser.h"

class UpdateManager : public QObject
{
	Q_OBJECT
public:
	explicit UpdateManager(QObject *parent);
	~UpdateManager();
	void saveFromParser(DetailsParser const *parser);

protected:
	QString const settingsFile;
	QSettings *mUpdateInfo;

};

