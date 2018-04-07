#ifndef UPDATERCONTROLLER_H
#define UPDATERCONTROLLER_H

#include "include/service/networkmanagerservice.h"
#include <QString>
#include <QObject>

class QNetworkReply;

class UpdaterController : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(QString prevVersion READ prevVersion NOTIFY prevVersionChanged)

public:
    UpdaterController(QObject *parent = 0);

public:
    Q_INVOKABLE void start();
    Q_INVOKABLE void cancel();

    QString version();
    QString prevVersion();

public slots:
   // void downloadFinished(QNetworkReply *reply);

signals:
    void internetFailed();
    void versionChanged();
    void prevVersionChanged();

public:
    void setVersion(QString const &version);
    void setPrevVersion(QString const &prevVersion);

private:
    QString m_version;
    QString m_prevVersion;
    NetworkManagerService m_networkManagerService;
};

#endif // UPDATERCONTROLLER_H
