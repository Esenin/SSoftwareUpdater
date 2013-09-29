#pragma once

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QFileInfo>
#include <QDebug>

class Downloader : public QObject
{
	Q_OBJECT
public:
	class CreateFileException{};

	explicit Downloader(QObject *parent = 0);
	void getUpdateDetails(QUrl const url);
	void getUpdate(QUrl const url) throw(CreateFileException);

signals:
	void detailsDownloaded(QIODevice *reply);
	void updatesDownloaded(QString filePath);
	void detailsLoadError(QString error);
	void updatesLoadError(QString error);

protected:
	void sendRequest(QUrl const url);
	void startFileDownloading(QUrl const url);

	QNetworkAccessManager mManager;
	QNetworkReply *mReply;
	QFile *mFile;

protected slots:
	void detailsFileDownloaded(QNetworkReply *reply);
	void updatesFileDownloaded(QNetworkReply *reply);
	void fileReadyRead();
};

