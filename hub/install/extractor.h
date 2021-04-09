#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <QObject>

class Extractor : public QObject {
    Q_OBJECT
public:
    explicit Extractor(QObject *parent = 0);

    int extract(const QString &archive_name, const QString &output_path);

signals:
    void threadStarted(int result);
    void operationExtract(const QString archive_name, const QString output_path);
    void operationResult(const QString archive_filename, int result);

    void extractFileSize(const quint64 filesize);
    void extractCompleteValue(const quint64 completeValue);
    void extractingFilename(const QString filename);

public slots:
    void threadStarted(void);
    void onExtract(const QString archive_name, const QString output_path);

private:
    QLibrary m_7zLib;
};

#endif // EXTRACTOR_H
