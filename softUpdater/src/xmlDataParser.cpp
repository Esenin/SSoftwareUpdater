#include "xmlDataParser.h"

XmlDataParser::XmlDataParser(const QString platform, QIODevice *device)
	: DetailsParser(platform, device)
{
	xml = new QXmlStreamReader(device);
	readXml();
	selectLocalDetails();
}

XmlDataParser::~XmlDataParser()
{
	delete xml;
}

void XmlDataParser::readXml() throw(ReadError)
{
	if (!xml->readNextStartElement() || xml->name() != "updaterInformation") {
		throw (ReadError());
	}

	while (xml->readNextStartElement()) {
		if (xml->name() == "versionId") {
			mVersionId = xml->readElementText().toInt();
		} else if (xml->name() == "platformFileList") {
			xml->readNextStartElement();
			readPlatformFile();
		} else {
			xml->skipCurrentElement();
		}
	}
}

void XmlDataParser::readPlatformFile()
{
	Q_ASSERT(xml.isStartElement() && xml.name() == "platformFile");

	QString curPlatform;
	while (xml.readNextStartElement()) {
		if (xml.name() == "platform") {
			curPlatform = xml->readElementText();
		} else if (xml->name() == "url") {
			mFiles.insert(curPlatform, xml->readElementText());
		} else {
			xml.skipCurrentElement();
		}
	}
}

void XmlDataParser::selectLocalDetails()
{
	mDownloadUrl = mFiles.value(mPlatformName);
	QFileInfo fileInfo(mDownloadUrl.toString());
	mFileName = fileInfo.fileName();
}
