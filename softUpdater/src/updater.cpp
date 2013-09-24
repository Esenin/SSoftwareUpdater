#include "updater.h"

Updater::Updater()
	: mSoftUpdate(false)
	, mUpdater(NULL)
{
	parseParams();
	mDownloader = new Downloader(this);
	mParser = new XmlDataParser(mParams.value("-unit"));
	connect(mDownloader, SIGNAL(updatesDownloaded(QString)), this, SLOT(fileReady(QString)));
	connect(mDownloader, SIGNAL(detailsDownloaded(QIODevice*)), mParser, SLOT(parseDevice(QIODevice*)));
	connect(mParser, SIGNAL(parseFinished()), this, SLOT(detailsChanged()));
	mDownloader->getUpdateDetails(mParams.value("-url"));
}

Updater::~Updater()
{
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
	if (!hasNewUpdates(mParser->version()))
		return;

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
	qDebug() << exitCode;
	if (status == QProcess::NormalExit)
		QCoreApplication::quit();
	Q_UNUSED(exitCode);
}

