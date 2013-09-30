#pragma once

#include <QtCore/QIODevice>
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>
#include <QtCore/QMap>

#include "update.h"

//!
//! \brief The DetailsParser class
//! reads received file and provides fast access to main params
class DetailsParser : public QObject
{
	Q_OBJECT
public:
	DetailsParser();
	virtual ~DetailsParser(){}

	//! \brief setUnitName  checkout other module
	//! \param currentUnit other module name
	virtual void changeUnit(QString const unit);

	virtual bool hasErrors() const = 0;

	Update* currentUpdate() const;

	QStringList units() const;
	QString currentUnit() const;

signals:
	void parseFinished();

public slots:
	virtual void parseDevice(QIODevice *device) = 0;

protected:
	Update *mCurrentUpdate;
	//! unit <-> url
	QMap<QString, QUrl> mFileUrls;
	QMap<QString, QString> mParamStrings;
	QMap<QString, QString> mVersions;
};

