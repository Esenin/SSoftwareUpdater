#include "updateProcessor.h"

UpdateProcessor::UpdateProcessor()
	: mCurAttempt(0)
	, mHardUpdate(false)
	, mUpdatesFolder("ForwardUpdates/")
{
	parseParams();
	mDownloader = new Downloader(this);
	mParser = new XmlDataParser();
	mUpdateInfo = new UpdateManager(mUpdatesFolder, this);
	initConnections();
}

UpdateProcessor::~UpdateProcessor()
{
	delete mParser;
}

void UpdateProcessor::startUpdateControl()
{
	qDebug() << "checking prepared updates";
	checkoutPreparedUpdates();

	if (!mUpdateInfo->preparedUpdate()->isInstalling()) {
		startUpdatingProcess();
	} else {
		mRetryTimer.start(retryTimerout);
	}
}

void UpdateProcessor::startUpdatingProcess()
{
	qDebug() << "Getting new update!";
	if (mRetryTimer.isActive()) {
		mRetryTimer.stop();
	}
	mCurAttempt++;
	mDownloader->getUpdateDetails(mParams.value("-url"));
}

void UpdateProcessor::initConnections()
{
	connect(&mRetryTimer, SIGNAL(timeout()), this, SLOT(startUpdatingProcess()));
	connect(mDownloader, SIGNAL(detailsLoadError(QString)), this, SLOT(downloadErrors(QString)));
	connect(mDownloader, SIGNAL(updatesLoadError(QString)), this, SLOT(downloadErrors(QString)));
	connect(mDownloader, SIGNAL(updatesDownloaded(QString)), this, SLOT(fileReady(QString)));
	connect(mDownloader, SIGNAL(detailsDownloaded(QIODevice*)), mParser, SLOT(parseDevice(QIODevice*)));
	connect(mParser, SIGNAL(parseFinished()), this, SLOT(detailsChanged()));
}

bool UpdateProcessor::parseParams()
{
	int const criticalParamsCount = 3;
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

bool UpdateProcessor::hasNewUpdates(QString const newVersion)
{
	return newVersion > mParams.value("-version");
}

void UpdateProcessor::startSetupProgram(Update *update)
{
	qDebug() << "start setup: " << update->filePath();
	connect(update, SIGNAL(installFinished(bool)), this, SLOT(updateFinished(bool)));
	update->installUpdate();
}

void UpdateProcessor::checkoutPreparedUpdates()
{
	if (!mUpdateInfo->hasPreparedUpdates()) {
		qDebug() << "no prepared updates there";
		return;
	}

	mUpdateInfo->loadUpdateInfo(mParams.value("-unit"));
	if (!hasNewUpdates(mUpdateInfo->preparedUpdate()->version())) {
		qDebug() << "update is outdated";
		return;
	}

	Communicator::writeQuitMessage();
	qDebug() << "starting setup process";
	startSetupProgram(mUpdateInfo->preparedUpdate());
}

void UpdateProcessor::detailsChanged()
{
	mParser->changeUnit(mParams.value("-unit"));
	if (!hasNewUpdates(mParser->currentUpdate()->version())) {
		qDebug() << "Server has no new updates";
		QCoreApplication::quit();
	}

	qDebug() << "start downloading update";
	mDownloader->getUpdate(mParser->currentUpdate()->url());
}

void UpdateProcessor::fileReady(QString const filePath)
{
	if (mHardUpdate) {
		qDebug() << "stating hard update process";
		startSetupProgram(mParser->currentUpdate());
		return;
	}

	qDebug() << "Saving file for later usage";
	mParser->changeUnit(mParams.value("-unit"));
	mUpdateInfo->saveFileForLater(mParser, filePath);
	QCoreApplication::quit();
}

void UpdateProcessor::updateFinished(bool hasSuccess)
{
	qDebug() << "setup execution finished. Success:" + hasSuccess;
	if (hasSuccess) {
		QFile::remove(mUpdateInfo->preparedUpdate()->filePath());
		mParams.insert("-version", mUpdateInfo->preparedUpdate()->version());
	}
}

void UpdateProcessor::downloadErrors(QString error)
{
	if (mCurAttempt < maxAttemptsCount) {
		mRetryTimer.start(retryTimerout);
	} else {
		QCoreApplication::quit();
	}

	Q_UNUSED(error);
}

