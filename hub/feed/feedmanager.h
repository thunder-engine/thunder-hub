#ifndef FEEDMANAGER_H
#define FEEDMANAGER_H

#include <QAbstractListModel>

class QNetworkAccessManager;
class QDomNode;

struct RssItem {
    QString title;
    QString description;
    QString link;
    QString category;
    QString media;
};

class FeedManager : public QAbstractListModel {
    Q_OBJECT

    enum Roles {
        Title,
        Brief,
        Link,
        Media
    };

public:
    explicit FeedManager(QObject *parent = nullptr);
    ~FeedManager();

private slots:
    void onUpdateCheckFinished();

private:
    QString extractTextFromNode(const QDomNode &node);

    int rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

private:
    QNetworkAccessManager *m_manager;

    QList<RssItem> m_items;

};

#endif // FEEDMANAGER_H
