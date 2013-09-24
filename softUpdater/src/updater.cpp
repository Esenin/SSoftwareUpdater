#include "updater.h"

Updater::Updater()
	: mSoftUpdate(false)
{
	qDebug() << parseParams();
}

Updater::~Updater()
{
}

bool Updater::parseParams()
{
	qDebug() << QCoreApplication::arguments();
	int const argsCount = QCoreApplication::arguments().size();
	if (argsCount - 1 < criticalParamsCount * 2) {
		return false;
	}

	mSoftUpdate = QCoreApplication::arguments().contains("-soft", Qt::CaseInsensitive);
	QStringList params;
	params << "-unit" << "-version" << "-url";
	foreach (QString param, params) {
		int index = QCoreApplication::arguments().indexOf(param);
		if (index == -1 || index + 1 >= argsCount)
			return false;

		mParams.insert(param, QCoreApplication::arguments().at(index + 1));
	}

	return true;
}

bool Updater::hasNewUpdates(QString const newVersion)
{
	return newVersion > mParams.value("-version");
}
