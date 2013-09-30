#pragma once

#include <QtCore/QObject>
#include <stdio.h>

class Communicator : public QObject
{
	Q_OBJECT
public:
	explicit Communicator(QObject *parent = 0);

	void readProgramPath();
	void writeQuitMessage();
	void writeResumeMessage();
	void writeHelpMessage();

	QString parentAppPath() const;
protected:
	QString mParentAppPath;
};

