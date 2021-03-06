#include "updatesInstaller.h"

using namespace qrUpdater;

UpdatesInstaller::UpdatesInstaller(QList<Update *> updates)
{
	mUpdatesQueue.append(updates);
}

UpdatesInstaller::UpdatesInstaller()
{
}

void UpdatesInstaller::operator<<(Update *update)
{
	mUpdatesQueue.append(update);
}

void UpdatesInstaller::operator<<(QList<Update *> updates)
{
	mUpdatesQueue.append(updates);
}

void UpdatesInstaller::installAll()
{
	QTimer::singleShot(delay, this, SLOT(startInstallation()));
}

bool UpdatesInstaller::hasNoErrors() const
{
	return mHasNoErrors;
}

bool UpdatesInstaller::isEmpty() const
{
	return mUpdatesQueue.isEmpty();
}

void UpdatesInstaller::installNext()
{
	connect(mUpdatesQueue.first(), SIGNAL(installFinished(bool)), this, SLOT(singleInstallFinished(bool)));
	replaceExpressions(mUpdatesQueue.first());
	mUpdatesQueue.first()->installUpdate();
	if (mUpdatesQueue.first()->hasSelfInstallMarker()) {
		emit selfInstalling();
	}
}

void UpdatesInstaller::replaceExpressions(Update *update)
{
	typedef QString (*Function)(void);
	QMap<QString, Function> replacement;
	replacement.insert(QString("%qru:installdir%"), UpdatesInstaller::getInstallDir);

	QMutableListIterator<QString> iterator(update->arguments());
	while (iterator.hasNext()) {
		iterator.next();
		foreach (QString mask, replacement.keys()) {
			QString curKey = iterator.value();
			iterator.setValue(curKey.replace(mask, replacement.value(mask)()));
		}
	}
}

QString UpdatesInstaller::getInstallDir()
{
	QDir current(QCoreApplication::applicationDirPath());
	if (current.cdUp()) {
		return current.absolutePath();
	} else {
		return QCoreApplication::applicationDirPath();
	}
}

void UpdatesInstaller::singleInstallFinished(bool hasNoErrors)
{
	if (hasNoErrors) {
		if (mUpdatesQueue.first()->isInstalled()) {
			mUpdatesQueue.first()->clear();
		} else {
			mHasNoErrors = false;
		}
	}

	mUpdatesQueue.takeFirst();
	if (!mUpdatesQueue.isEmpty()) {
		installNext();
	} else
	{
		emit installsFinished(mHasNoErrors);
	}
}

void UpdatesInstaller::startInstallation()
{
	mHasNoErrors = true;
	if (!mUpdatesQueue.isEmpty()) {
		installNext();
	}
}
