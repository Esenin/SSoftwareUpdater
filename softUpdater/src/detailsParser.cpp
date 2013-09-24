#include "detailsParser.h"

DetailsParser::DetailsParser(QString const platform)
	: mPlatformName(platform)
{
}

QString DetailsParser::version() const
{
	return mVersionId;
}

QUrl DetailsParser::downloadAdress() const
{
	return mDownloadUrl;
}

QString DetailsParser::filename() const
{
	return mFileName;
}

