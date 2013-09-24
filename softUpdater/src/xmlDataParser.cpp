#include "xmlDataParser.h"

XmlDataParser::XmlDataParser(const QString platform)
	: DetailsParser(platform)
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
		} else if (mXml->name() == "platformFile") {
			readPlatformFile();
		} else if (mXml->name() != "platformFileList") {
			mXml->skipCurrentElement();
		}
	}
}

void XmlDataParser::readPlatformFile()
{
	Q_ASSERT(mXml->isStartElement() && mXml->name() == "platformFile");

	QString curPlatform;
	while (mXml->readNextStartElement()) {
		if (mXml->name() == "platform") {
			curPlatform = mXml->readElementText();
		} else if (mXml->name() == "url") {
			mFiles.insert(curPlatform, mXml->readElementText());
		} else {
			mXml->skipCurrentElement();
		}
	}
}

void XmlDataParser::selectLocalDetails()
{
	mDownloadUrl = mFiles.value(mPlatformName);
	QFileInfo fileInfo(mDownloadUrl.toString());
	mFileName = fileInfo.fileName();
}
