#include "communicator.h"

using std::cout;

Communicator::Communicator(QObject *parent)
	: QObject(parent)
{
}

void Communicator::writeQuitMessage()
{
	cout << "quit";
}

void Communicator::writeHelpMessage()
{
	cout << "usage:\n"
			"updater -unit [unit-name] -version [version] -url [url-to-file] | [-hard]\n"
			"example:\n updater -unit windows -version 0.3.2 -url http://127.0.0.1/updates.xml"
			"or\n"
			"updater -unit cool-module-name -version 2.8a -url http://example.com/updates.xml -hard\n";
}


