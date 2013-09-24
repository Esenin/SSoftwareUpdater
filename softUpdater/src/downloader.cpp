#include "downloader.h"

Downloader::Downloader(QObject *parent)
	: QObject(parent)
	, mReply(NULL)
{
}

void Downloader::getUpdateDetails(QUrl const url)
{
	connect(&mManager, SIGNAL(finished(QNetworkReply*)), SLOT(detailsFileDownloaded(QNetworkReply*)));
	sendRequest(url);
	connect(mReply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors(QList<QSslError>)));
}

void Downloader::getUpdate(QUrl const url)
{
	QFileInfo fileInfo(url.path());
	QString fileName = fileInfo.fileName();
	if (fileName.isEmpty())
		fileName = "update";

	if (QFile::exists(fileName)) {
		QFile::remove(fileName);
	}

	mFile = new QFile(fileName);
	if (!mFile->open(QIODevice::WriteOnly)) {
		delete mFile;
		mFile = NULL;
		qDebug() << "unable to save file:" << fileName << mFile->errorString();
		return;
	}

	startFileDownloading(url);
}

void Downloader::detailsFileDownloaded(QNetworkReply *reply)
{
	if (reply->error())
		qDebug() << "details download failed:" << qPrintable(reply->errorString());

	disconnect(this, SLOT(sslErrors(QList<QSslError>)));
	emit detailsDownloaded(reply);
}

void Downloader::updatesFileDownloaded(QNetworkReply *reply)
{
	if (mReply->error()) {
		qDebug() << "updates download failed:" << qPrintable(reply->errorString());
		mFile->remove();
	} else {
		mFile->flush();
		mFile->close();
	}

	mReply->deleteLater();
	mReply = 0;
	emit updatesDownloaded(QFileInfo(*mFile).filePath());
	delete mFile;
	mReply = 0;
}

void Downloader::sslErrors(QList<QSslError> const &errors)
{
#ifndef QT_NO_OPENSSL
	foreach (const QSslError &error, errors)
		qDebug() << "SSL error:" << qPrintable(error.errorString());
#endif
}

void Downloader::fileReadyRead()
{
	if (mFile) {
		mFile->write(mReply->readAll());
	}
}

void Downloader::sendRequest(QUrl const url)
{
	QNetworkRequest request(url);
	mReply = mManager.get(request);
}

void Downloader::startFileDownloading(const QUrl url)
{
	connect(&mManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updatesFileDownloaded(QNetworkReply*)));
	sendRequest(url);
	connect(mReply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrors(QList<QSslError>)));
	connect(mReply, SIGNAL(readyRead()), this, SLOT(fileReadyRead()));
}
