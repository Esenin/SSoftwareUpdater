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
	device->deleteLater();
	emit parseFinished();
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

	QString curModule;
	QUrl curUrl;
	QString argument;
	while (mXml->readNextStartElement()) {
		if (mXml->name() == "unit") {
			curModule = mXml->readElementText();
		} else if (mXml->name() == "url") {
			curUrl = QUrl(mXml->readElementText());
		} else if (mXml->name() == "paramStr") {
			argument = mXml->readElementText();
		} else {
			mXml->skipCurrentElement();
		}
	}
	mFiles.insert(curModule, curUrl);
	mParamStrings.insert(curModule, argument);
}

void XmlDataParser::selectLocalDetails()
{
	mDownloadUrl = mFiles.value(mUnitName);
	QFileInfo fileInfo(mDownloadUrl.toString());
	mFileName = fileInfo.fileName();
}
