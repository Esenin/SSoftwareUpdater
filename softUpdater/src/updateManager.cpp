#include "updateManager.h"

UpdateManager::UpdateManager(QString updatesFolder, QObject *parent)
	: QObject(parent)
	, settingsFile("updateInfo.ini")
{
	mUpdateInfo = new QSettings(settingsFile, QSettings::IniFormat, parent);
}

UpdateManager::~UpdateManager()
{
	mUpdateInfo->sync();
}

void UpdateManager::saveFromParser(DetailsParser const *parser)
{
	mUpdateInfo->beginGroup(parser->currentUnit());
	mUpdateInfo->setValue("fileName", parser->filename());
	mUpdateInfo->setValue("version", parser->version());
	mUpdateInfo->setValue("args", parser->arguments());
	mUpdateInfo->endGroup();
}

void UpdateManager::loadUpdateInfo(QString const unit)
{
	mCurrentUnit = unit;
	mUpdateInfo->beginGroup(mCurrentUnit);
	mFileName = mUpdateInfo->value("fileName").toString();
	mVersion = mUpdateInfo->value("version").toString();
	mArguments = mUpdateInfo->value("args").toStringList();
	mUpdateInfo->endGroup();
}

QString UpdateManager::version() const
{
	return mVersion;
}

QString UpdateManager::fileName() const
{
	return mFileName;
}

QStringList UpdateManager::arguments() const
{
	return mArguments;
}
