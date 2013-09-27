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
