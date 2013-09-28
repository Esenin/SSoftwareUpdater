#include "detailsParser.h"

void DetailsParser::setUnitName(QString const currentUnit)
{
	mCurrentUnit = currentUnit;
	mDownloadUrl = mFileUrls.value(mCurrentUnit);
	mFileName = QFileInfo(mDownloadUrl.toString()).fileName();
	mVersionId = mVersions.value(mCurrentUnit);
	mArguments.clear();
	mArguments = mParamStrings.value(mCurrentUnit).split(" ");
}

QString DetailsParser::version() const
{
	return mVersionId;
}

QUrl DetailsParser::downloadAdress() const
{
	return mDownloadUrl;
}

QStringList DetailsParser::arguments() const
{
	return mArguments;
}

QString DetailsParser::filename() const
{
	return mFileName;
}

QStringList DetailsParser::units() const
{
	return mFileUrls.keys();
}

QString DetailsParser::currentUnit() const
{
	return mCurrentUnit;
}

