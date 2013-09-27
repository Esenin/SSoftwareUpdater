#pragma once

#include <QtCore/QObject>
#include <iostream>

class Communicator : public QObject
{
	Q_OBJECT
public:
	explicit Communicator(QObject *parent = 0);

	void writeQuitMessage();
};

