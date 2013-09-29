
#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QFileInfo>
#include <QtCore/QStringList>

class Update : public QObject
{
	Q_OBJECT
public:
	explicit Update(QObject *parent = 0);
	Update(QString const filePath, QStringList const args, QString const version, QObject *parent = 0);
	void installUpdate();

	QString filePath() const;
	QString fileName() const;
	QString version() const;
	QStringList arguments() const;

signals:
	//! @return True if process finished correctly
	void installFinished(bool);

protected:
	QString mFilePath;
	QStringList mArguments;
	QString mVersion;
	QProcess *mProcess;
};

