#include "updater.h"

Updater::Updater()
	: mCurAttempt(0)
	, mHardUpdate(false)
	, mUpdatesFolder("ForwardUpdates/")
	, mUpdateProcess(NULL)
{
	parseParams();
	mDownloader = new Downloader(this);
	mParser = new XmlDataParser();
	mUpdateInfo = new UpdateManager(mUpdatesFolder, this);
	initConnections();

	qDebug() << "start check updates";
	checkPreparedUpdates();

	if (mUpdateProcess == NULL) {
		qDebug() << "start new update cycle!";
		startUpdatingProcess();
	}
	qDebug() << "End Of Constructor";
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
	qDebug() << "start setup: " << filePath;
	mUpdateProcess = new QProcess(this);
	connect(mUpdateProcess, SIGNAL(finished(int, QProcess::ExitStatus))
	, this, SLOT(updateFinished(int, QProcess::ExitStatus)));
	mUpdateProcess->start(filePath, arguments);
}

void Updater::checkPreparedUpdates()
{
	QDir updatesDir(mUpdatesFolder);
	if (!updatesDir.exists()) {
		return;
	}

	qDebug() << "check for files";
	mUpdateInfo->loadUpdateInfo(mParams.value("-unit"));
	if (mUpdateInfo->fileName().isEmpty() || !hasNewUpdates(mUpdateInfo->version())) {
		return;
	}

	Communicator::writeQuitMessage();
	startSetupProgram(mUpdatesFolder + mUpdateInfo->fileName(), mUpdateInfo->arguments());
}

void Updater::detailsChanged()
{
	mParser->setUnitName(mParams.value("-unit"));
	if (!hasNewUpdates(mParser->version())) {
		qDebug() << "Has no new updates";
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

	mParser->setUnitName(mParams.value("-unit"));
	mUpdateInfo->saveFileForLater(mParser, filePath);
	QCoreApplication::quit();
}

void Updater::updateFinished(int exitCode, QProcess::ExitStatus status)
{
	qDebug() << "setup execution finished";
	mUpdateProcess->deleteLater();
	if (status == QProcess::NormalExit) {
		QFile::remove(mUpdatesFolder + mUpdateInfo->fileName());
		mParams.insert("-version", mUpdateInfo->version());
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

