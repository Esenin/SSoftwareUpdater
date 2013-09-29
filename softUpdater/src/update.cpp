#include "update.h"

Update::Update(QObject *parent)
	: QObject(parent)
	, mProcess(NULL)
{
}

Update::Update(QString const filePath, QStringList const args, QString const version, QObject *parent)
	: QObject(parent)
	, mProcess(NULL)
{
	setData(filePath, args, version);
}

void Update::setUrl(const QUrl link)
{
	mDownloadUrl = link;
}

void Update::setUnitName(const QString unit)
{
	mModule = unit;
}

void Update::setData(const QString filePath, const QStringList args, const QString version, const QUrl link)
{
	mFilePath = filePath;
	mArguments = args;
	mVersion = version;
	mDownloadUrl = link;
}

void Update::clear()
{
	setData(QString(), QStringList(), QString());
	setUnitName("");
	if (isInstalling()) {
		mProcess->terminate();
	}
	mProcess->deleteLater();
	mProcess = NULL;
}

void Update::installUpdate()
{
	mProcess = new QProcess(this);
	connect(mProcess, SIGNAL(finished(int, QProcess::ExitStatus))
			, this , SLOT(installingFinished(int, QProcess::ExitStatus)));
	mProcess->start(mFilePath, mArguments);
}

bool Update::isEmpty() const
{
	return mFilePath.isEmpty();
}

bool Update::isInstalling() const
{
	return (mProcess != NULL) && (mProcess->pid() > 0);
}

QUrl Update::url() const
{
	return mDownloadUrl;
}

QString Update::unit() const
{
	return mModule;
}

QString Update::filePath() const
{
	return mFilePath;
}

QString Update::fileName() const
{
	return QFileInfo(mFilePath).fileName();
}

QString Update::version() const
{
	return mVersion;
}

QStringList Update::arguments() const
{
	return mArguments;
}

void Update::installingFinished(int exitCode, QProcess::ExitStatus status)
{
	emit installFinished(status == QProcess::NormalExit);
	Q_UNUSED(exitCode);
}
