#ifndef UPDATERCONTROLLER_H
#define UPDATERCONTROLLER_H

#include "include/service/filesmanagerservice.h"
#include "include/service/networkmanagerservice.h"
#include "include/service/processmanagerservice.h"
#include <QString>
#include <QObject>

class QNetworkReply;

class UpdaterController : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(QString prevVersion READ prevVersion NOTIFY prevVersionChanged)
    Q_PROPERTY(double avancement READ avancement NOTIFY avancementChanged)

public:
    UpdaterController(QObject *parent = 0);

public:
    Q_INVOKABLE void start();
    Q_INVOKABLE void cancel();
    Q_INVOKABLE void launchApplication();
    Q_INVOKABLE bool isApplicationLaunch();
    Q_INVOKABLE void stopApplication();
    Q_INVOKABLE void quit();

    QString version();
    QString prevVersion();
    double avancement();

public slots:
    void onDownloadSuccess();

signals:
    void internetFailed();
    void versionChanged();
    void prevVersionChanged();
    void avancementChanged();
    void downloadSuccess();
    void filesMovedSuccess();
    void filesMovedFailed();

public:
    void setVersion(QString const &version);
    void setPrevVersion(QString const &prevVersion);

private:
    QString m_version;
    QString m_prevVersion;
    QString m_url;
    NetworkManagerService m_networkManagerService;
    double m_avancement;
    FilesManagerService m_filesManagerService;
    ProcessManagerService m_processManagerService;
};

#endif // UPDATERCONTROLLER_H
