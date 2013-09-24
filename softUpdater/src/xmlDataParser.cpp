#include "xmlDataParser.h"

XmlDataParser::XmlDataParser(const QString unit)
	: DetailsParser(unit)
	, mXml(NULL)
{
}

XmlDataParser::~XmlDataParser()
{
	delete mXml;
}

void XmlDataParser::parseDevice(QIODevice *device)
{
	mXml = new QXmlStreamReader(device);
	readXml();
	selectLocalDetails();
}

void XmlDataParser::readXml() throw(ReadError)
{
	if (!mXml->readNextStartElement() || mXml->name() != "updaterInformation") {
		throw (ReadError());
	}

	while (mXml->readNextStartElement()) {
		if (mXml->name() == "version") {
			mVersionId = mXml->readElementText();
		} else if (mXml->name() == "unitFile") {
			readunitFile();
		} else if (mXml->name() != "unitFileList") {
			mXml->skipCurrentElement();
		}
	}
}

void XmlDataParser::readunitFile()
{
	Q_ASSERT(mXml->isStartElement() && mXml->name() == "unitFile");

	QString curunit;
	while (mXml->readNextStartElement()) {
		if (mXml->name() == "unit") {
			curunit = mXml->readElementText();
		} else if (mXml->name() == "url") {
			mFiles.insert(curunit, mXml->readElementText());
		} else {
			mXml->skipCurrentElement();
		}
	}
}

void XmlDataParser::selectLocalDetails()
{
	mDownloadUrl = mFiles.value(munitName);
	QFileInfo fileInfo(mDownloadUrl.toString());
	mFileName = fileInfo.fileName();
}
