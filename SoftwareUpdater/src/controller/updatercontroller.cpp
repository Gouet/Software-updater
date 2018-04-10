#include <QDebug>
#include "include/controller/updatercontroller.h"

UpdaterController::UpdaterController(QObject *parent)
    : QObject(parent),
      m_version(),
      m_prevVersion(),
      m_url("https://raw.githubusercontent.com/Gouet/DNAI_updaters"),
      m_networkManagerService(),
      #if defined(Q_OS_MAC)
        m_filesManagerService(QDir::tempPath() + "/DNAI.app", "/Applications/DNAI.app")
      #elif defined(Q_OS_WIN)
        m_filesManagerService(QDir::tempPath() + "/DNAI", "/Applications/DNAI")
      #else
        m_filesManagerService(QDir::tempPath() + "/DNAI.app", "/Applications/DNAI.app")
      #endif
{
    QObject::connect(&m_networkManagerService, SIGNAL(avancementChanged()),
                     this, SIGNAL(avancementChanged()));
    QObject::connect(&m_networkManagerService, SIGNAL(internetFailed()),
                     this, SIGNAL(internetFailed()));
    QObject::connect(&m_networkManagerService, SIGNAL(downloadSuccess()),
                     this, SLOT(onDownloadSuccess()));
    QObject::connect(&m_filesManagerService, SIGNAL(filesMovedSuccess()),
                     this, SIGNAL(filesMovedSuccess()));
    QObject::connect(&m_filesManagerService, SIGNAL(filesMovedFailed()),
                     this, SIGNAL(filesMovedFailed()));
}

//void UpdaterController::downloadFinished(QNetworkReply *reply) {
//    qDebug() << reply->readAll();
//}

void UpdaterController::start() {
    QString softwares;
    QString downloadVersion;

#if defined(Q_OS_MAC)
    softwares = "mac";
    downloadVersion = softwares + m_version;
#elif defined(Q_OS_WIN)
    softwares = "windows";
    downloadVersion = softwares + m_version;
#endif


    m_networkManagerService.requestCommonFile(m_url + "/" + downloadVersion + "/",
                                              softwares);
}

void UpdaterController::cancel() {
    qDebug() << "CANCEL";
    m_networkManagerService.cancel();
}

void UpdaterController::setVersion(QString const &version) {
    this->m_version = version;
}

void UpdaterController::setPrevVersion(QString const &prevVersion) {
    this->m_prevVersion = prevVersion;
}

QString UpdaterController::version() {
    return this->m_version;
}

QString UpdaterController::prevVersion() {
    return this->m_prevVersion;
}

double UpdaterController::avancement() {
    return (this->m_networkManagerService.getAvancement());
}

void UpdaterController::onDownloadSuccess() {
    m_filesManagerService.moveFiles();
    emit downloadSuccess();
}
