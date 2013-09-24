#include "updater.h"

Updater::Updater()
	: mSoftUpdate(false)
	, mUpdater(NULL)
{
	parseParams();
	mDownloader = new Downloader(this);
	mParser = new XmlDataParser(mParams.value("-unit"));
	initConnections();

	startUpdatingProcess();
}

Updater::~Updater()
{
	delete mParser;
}

void Updater::startUpdatingProcess()
{
	mDownloader->getUpdateDetails(mParams.value("-url"));
}

void Updater::initConnections()
{
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

void Updater::detailsChanged()
{
	if (!hasNewUpdates(mParser->version())) {
		QCoreApplication::quit();
	}

	mDownloader->getUpdate(mParser->downloadAdress());
}

void Updater::fileReady(QString const filePath)
{
	mUpdater = new QProcess(this);
	mUpdater->start(filePath, mParser->arguments());
	connect(mUpdater, SIGNAL(finished(int, QProcess::ExitStatus))
			, this, SLOT(updateFinished(int,QProcess::ExitStatus)));
}

void Updater::updateFinished(int exitCode, QProcess::ExitStatus status)
{
	if (status == QProcess::NormalExit)
		QCoreApplication::quit();
	Q_UNUSED(exitCode);
}

void Updater::downloadErrors(QString error)
{
	QCoreApplication::quit();
	Q_UNUSED(error);
}

