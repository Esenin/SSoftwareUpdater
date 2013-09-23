#include "detailsParser.h"

DetailsParser::DetailsParser(QString const platform, QIODevice *device)
	: mPlatformName(platform)
	, mVersionId(0)
{
	Q_UNUSED(device);
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
