#include "MainWindow.h"
#include <QApplication>

#include "downloader.h"
#include "xmlDataParser.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
//	Downloader downloader;
//	XmlDataParser parser("windows");

//	QObject::connect(&downloader, SIGNAL(detailsDownloaded(QIODevice*)), &parser, SLOT(parseDevice(QIODevice*)));
//	downloader.getUpdateDetails(QUrl("http://127.0.0.1:8080/updates.xml"));

	return app.exec();
}
