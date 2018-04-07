#include <QDebug>
#include "include/controller/updatercontroller.h"

UpdaterController::UpdaterController(QObject *parent)
    : QObject(parent),
      m_version(),
      m_prevVersion(),
      m_networkManagerService() {
    QObject::connect(&m_networkManagerService, SIGNAL(avancementChanged()),
                     this, SIGNAL(avancementChanged()));
}

//void UpdaterController::downloadFinished(QNetworkReply *reply) {
//    qDebug() << reply->readAll();
//}

void UpdaterController::start() {
    qDebug() << "START";
    m_networkManagerService.requestCommonFile("https://raw.githubusercontent.com/Gouet/DNAI_updaters/mac0.0.16/",
                                              "mac");
    /*QNetworkAccessManager *accessManager = new QNetworkAccessManager();
    QNetworkRequest request(QUrl());
    QObject::connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));

    accessManager->get(request);*/
}

void UpdaterController::cancel() {
    qDebug() << "CANCEL";
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
