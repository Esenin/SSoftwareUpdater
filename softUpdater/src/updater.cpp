#include "updater.h"

Updater::Updater()
	: mCurAttempt(0)
	, mHardUpdate(false)
	, mUpdatesFolder("ForwardUpdates/")
	, mParamFile("updateArgs.dat")
	, mUpdater(NULL)
{
	parseParams();
	mDownloader = new Downloader(this);
	mParser = new XmlDataParser(mParams.value("-unit"));
	initConnections();

	findPreparedUpdates();

	startUpdatingProcess();
}

Updater::~Updater()
{
	delete mParser;
}

void Updater::startUpdatingProcess()
{
	if (mRetryTimer.isActive()) {
		mRetryTimer.stop();
	}
	mCurAttempt++;
	mDownloader->getUpdateDetails(mParams.value("-url"));
}

void Updater::initConnections()
{
	connect(&mRetryTimer, SIGNAL(timeout()), this, SLOT(startUpdatingProcess()));
	connect(mDownloader, SIGNAL(detailsLoadError(QString)), this, SLOT(downloadErrors(QString)));
	connect(mDownloader, SIGNAL(updatesLoadError(QString)), this, SLOT(downloadErrors(QString)));
	connect(mDownloader, SIGNAL(updatesDownloaded(QString)), this, SLOT(fileReady(QString)));
	connect(mDownloader, SIGNAL(detailsDownloaded(QIODevice*)), mParser, SLOT(parseDevice(QIODevice*)));
	connect(mParser, SIGNAL(parseFinished()), this, SLOT(detailsChanged()));
}

bool Updater::parseParams()
{
	int const argsCount = QCoreApplication::arguments().size();
	if (argsCount - 1 < criticalParamsCount * 2) {
		return false;
	}

	mHardUpdate = QCoreApplication::arguments().contains("-hard", Qt::CaseInsensitive);
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

void Updater::startSetupProgram(QString const filePath, QStringList const arguments)
{
	mUpdater = new QProcess(this);
	connect(mUpdater, SIGNAL(finished(int, QProcess::ExitStatus))
	, this, SLOT(updateFinished(int, QProcess::ExitStatus)));
	mUpdater->start(filePath, arguments);
}

void Updater::saveFileForLater(QString const filePath)
{
	QDir().mkdir(mUpdatesFolder);

	if (QFile::exists(mUpdatesFolder + QFileInfo(filePath).fileName())) {
		QFile::remove(mUpdatesFolder + QFileInfo(filePath).fileName());
	}

	QFile::rename(filePath, mUpdatesFolder + QFileInfo(filePath).fileName());

	QFile argsFile(mUpdatesFolder + mParamFile);
	argsFile.open(QIODevice::WriteOnly | QIODevice::Text);
	foreach (QString const argument, mParser->arguments()) {
		argsFile.write(QString(argument + " ").toLocal8Bit());
	}
	argsFile.close();
	QCoreApplication::quit();
}

void Updater::findPreparedUpdates()
{
	QDir updatesDir(mUpdatesFolder);
	if (!updatesDir.exists()) {
		return;
	}

	QStringList files = updatesDir.entryList(QDir::Files);
	foreach (QString const filePath, files) {
		startSetupProgram(mUpdatesFolder + filePath);
	}
}

void Updater::saveUpdateInfo()
{

}

QStringList Updater::loadUpdateInfo()
{
	QSettings updateInfo(mUpdatesFolder + "update.ini", QSettings::IniFormat, this);
	QString fileName = updateInfo.value("file").toString();
	QStringList result;
	int size = updateInfo.beginReadArray("args");
	for (int i = 0; i < size(); i++) {
		updateInfo.setArrayIndex(i);
		result << updateInfo.value(fileName).toString();
	}
	updateInfo.endArray();
	return result;
}

void Updater::detailsChanged()
{
	if (!hasNewUpdates(mParser->version())) {
		QCoreApplication::quit();
	}

	mDownloader->getUpdate(mParser->downloadAdress());
}

void Updater::fileReady(QString const filePath)
{
	if (mHardUpdate) {
		startSetupProgram(filePath, mParser->arguments());
		return;
	}

	saveFileForLater(filePath);
}

void Updater::updateFinished(int exitCode, QProcess::ExitStatus status)
{
	if (status == QProcess::NormalExit) {
		QCoreApplication::quit();

	}
	Q_UNUSED(exitCode);
}

void Updater::downloadErrors(QString error)
{
	if (mCurAttempt < maxAttemptsCount) {
		mRetryTimer.start(retryTimerout);
	} else {
		QCoreApplication::quit();
	}

	Q_UNUSED(error);
}

