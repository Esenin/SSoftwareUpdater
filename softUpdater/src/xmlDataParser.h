#pragma once

#include <QtCore/QXmlStreamReader>
#include <QtCore/QFileInfo>

#include "detailsParser.h"

class XmlDataParser : public DetailsParser
{
	Q_OBJECT
public:
	class ReadError{};

	XmlDataParser(QString const unit);
	virtual ~XmlDataParser();
	virtual void parseDevice(QIODevice *device);

protected:
	void readXml() throw(ReadError);
	void readunitFile();
	void selectLocalDetails();

	QXmlStreamReader *mXml;
};

