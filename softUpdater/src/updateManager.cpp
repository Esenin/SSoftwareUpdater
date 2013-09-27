#include "updateManager.h"

UpdateManager::UpdateManager(QObject *parent)
	: QObject(parent)
	, settingsFile("updateInfo.dat")
{
	mUpdateInfo = new QSettings(settingsFile, QSettings::IniFormat, parent);
}

UpdateManager::~UpdateManager()
{
	delete mUpdateInfo;
}

void UpdateManager::saveFromParser(DetailsParser const *parser)
{
	updateInfo.setValue("fileName", parser->filename());
	updateInfo.setValue("version", parser->version());
	QStringList curArguments = parser->arguments();
	updateInfo.beginWriteArray("args", curArguments.size());
	for (int i = 0; i < curArguments.size(); i++) {
		updateInfo.setArrayIndex(i);
		updateInfo.setValue(parser->filename(), curArguments.at(i));
	}
	updateInfo.endArray();
}
