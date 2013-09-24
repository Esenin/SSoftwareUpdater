#include "detailsParser.h"

DetailsParser::DetailsParser(QString const unit)
	: munitName(unit)
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

