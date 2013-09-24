#include "detailsParser.h"

DetailsParser::DetailsParser(QString const unit)
	: mUnitName(unit)
{
}

QString DetailsParser::version() const
{
	return mVersionId;
}

QUrl DetailsParser::downloadAdress() const
{
	return mFiles.value(mUnitName);
}

QStringList DetailsParser::arguments() const
{
	return mParamStrings.value(mUnitName).split(" ");
}

QString DetailsParser::filename() const
{
	return mFileName;
}

