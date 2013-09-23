#include "detailsParser.h"

DetailsParser::DetailsParser(QString const platform)
	: mPlatformName(platform)
	, mVersionId(0)
{
}

int DetailsParser::version() const
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

