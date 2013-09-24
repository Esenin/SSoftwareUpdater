#include "MainWindow.h"
#include <QApplication>

#include "updater.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	Updater *upd = new Updater();
	Q_UNUSED(upd);

	return app.exec();
}
