#include "update.h"

Update::Update(QObject *parent)
	: QObject(parent)
{
}

Update::Update(QString const filePath, QStringList const args, QString const version, QObject *parent = 0)
{

}

void Update::installUpdate()
{

}

QString Update::filePath() const
{
	return mFilePath;
}

QString Update::fileName() const
{
	return QFileInfo(mFilePath).fileName();
}

QString Update::version() const
{
	return mVersion;
}

QStringList Update::arguments() const
{
	return mArguments;
}
