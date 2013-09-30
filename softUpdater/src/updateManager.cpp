#include "updateManager.h"

UpdateManager::UpdateManager(QString updatesFolder, QObject *parent)
	: QObject(parent)
	, mUpdatesFolder(updatesFolder)
	, settingsFile(updatesFolder + "updateInfo.ini")
{
	mPreparedUpdate = new Update(this);
	mUpdateInfo = new QSettings(settingsFile, QSettings::IniFormat, parent);
}

UpdateManager::~UpdateManager()
{
	mUpdateInfo->sync();
	if (QDir(mUpdatesFolder).exists() && QFile::exists(settingsFile) && QFile(settingsFile).size() == 0) {
		QFile::remove(settingsFile);
		QDir(mUpdatesFolder).removeRecursively();
	}
}

void UpdateManager::saveInfoFromParser(DetailsParser const *parser)
{
	mUpdateInfo->beginGroup(parser->currentUnit());
	mUpdateInfo->setValue("fileName", parser->currentUpdate()->filePath());
	mUpdateInfo->setValue("version", parser->currentUpdate()->version());
	mUpdateInfo->setValue("args", parser->currentUpdate()->arguments());
	mUpdateInfo->endGroup();
}

void UpdateManager::saveFileForLater(DetailsParser const *parser, QString const filePath)
{
	QDir().mkdir(mUpdatesFolder);

	if (QFile::exists(mUpdatesFolder + QFileInfo(filePath).fileName())) {
		QFile::remove(mUpdatesFolder + QFileInfo(filePath).fileName());
	}

	QFile::rename(filePath, mUpdatesFolder + QFileInfo(filePath).fileName());

	saveInfoFromParser(parser);
}

void UpdateManager::removePreparedUpdate()
{
	mUpdateInfo->remove(mPreparedUpdate->unit());
	mPreparedUpdate->clear();
}

bool UpdateManager::hasPreparedUpdatesInfo()
{
	return QDir(mUpdatesFolder).exists() && QFile::exists(settingsFile);
}

void UpdateManager::loadUpdateInfo(QString const unit)
{
	if (!hasPreparedUpdatesInfo()) {
		return;
	}

	mUpdateInfo->beginGroup(unit);
	mPreparedUpdate->setData(
			mUpdatesFolder + mUpdateInfo->value("fileName").toString()
			, mUpdateInfo->value("args").toStringList()
			, mUpdateInfo->value("version").toString()
	);
	mPreparedUpdate->setUnitName(unit);
	mUpdateInfo->endGroup();
}

Update *UpdateManager::preparedUpdate()
{
	return mPreparedUpdate;
}

