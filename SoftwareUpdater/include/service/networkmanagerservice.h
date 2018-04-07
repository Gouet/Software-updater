#ifndef NETWORKMANAGERSERVICE_H
#define NETWORKMANAGERSERVICE_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QNetworkAccessManager>

class QNetworkReply;

class NetworkManagerService : public QObject {
    Q_OBJECT

public:
    NetworkManagerService(QObject *parent = 0);

    Q_INVOKABLE void requestCommonFile(QString const &url, QString const &software);

public slots:
    void downloadCommonFileFinished(QNetworkReply *);
    void downloadApplicationFileFinished(QNetworkReply *);

public:
    double getAvancement() const;

signals:
    void avancementChanged();

private:
    QNetworkAccessManager m_manager;
    QString m_url;
    QString m_software;
    QMap<QString, QString> m_map;
    int filesNbr;
    int fileDownloadedNbr;
};

#endif // NETWORKMANAGERSERVICE_H
