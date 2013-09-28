/*
 * QRealSpecial branch: application was made for only setup-file usage
 */

#include <QApplication>

#include "updater.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	Updater *updater = new Updater();
	Q_UNUSED(updater);

	return app.exec();
}
