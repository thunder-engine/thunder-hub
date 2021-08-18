#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <QObject>
#include <QThread>

class Extractor : public QObject {
    Q_OBJECT
public:
    explicit Extractor(QObject *parent = 0);
    ~Extractor();

    void extract(const QString archiveName, const QString outputPath);

signals:
    void extractProgress(qint64 filesExtracted, qint64 filesTotal);
    void extractFinished();

private slots:
    void doWork();

private:
    QThread m_Thread;

    QString m_archiveName;
    QString m_outputPath;

};

#endif // EXTRACTOR_H
