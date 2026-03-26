#ifndef UPDATE_H
#define UPDATE_H

#include <QObject>

class QNetworkAccessManager;

class Update : public QObject {
    Q_OBJECT

public:
    Update();

    Q_INVOKABLE void checkUpdate();

signals:
    void updateAvailable(const QString &version);

private slots:
    void onUpdateCheckFinished();

private:
    QNetworkAccessManager *m_manager;

};

#endif // UPDATE_H
