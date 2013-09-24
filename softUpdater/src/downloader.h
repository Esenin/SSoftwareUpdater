#pragma once

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSslError>
#include <QtCore/QFileInfo>
#include <QDebug>

class QSslError;

QT_USE_NAMESPACE


class Downloader : public QObject
{
	Q_OBJECT
public:
	explicit Downloader(QObject *parent = 0);
	void getUpdateDetails(QUrl const url);
	void getUpdate(QUrl const url);

signals:
	void detailsDownloaded(QIODevice *reply);
	void updatesDownloaded(QString filePath);

protected:
	void sendRequest(QUrl const url);
	void startFileDownloading(QUrl const url);

	QNetworkAccessManager mManager;
	QNetworkReply *mReply;
	QFile *mFile;

protected slots:
	void detailsFileDownloaded(QNetworkReply *reply);
	void updatesFileDownloaded(QNetworkReply *reply);
	void sslErrors(QList<QSslError> const &errors);
	void fileReadyRead();
};

