#pragma once

#include <QtCore/QXmlStreamReader>
#include <QtCore/QFileInfo>

#include "detailsParser.h"

class XmlDataParser : public DetailsParser
{
public:
	class ReadError{};

	XmlDataParser(QString const platform, QIODevice *device);
	virtual ~XmlDataParser();

protected:
	void readXml() throw(ReadError);
	void readPlatformFile();
	void selectLocalDetails();

	QXmlStreamReader *xml;

};

