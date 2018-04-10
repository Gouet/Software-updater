#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QNetworkReply>
#include <QList>
#include <QByteArray>
#include <QFile>
#include <QDir>
#include "include/service/networkmanagerservice.h"

NetworkManagerService::NetworkManagerService(QObject *parent)
    : QObject(parent),
      m_manager(),
      m_configuration(),
      m_url(),
      m_software(),
      m_map(),
      filesNbr(0),
      fileDownloadedNbr(0),
      networkAccessible(true),
      m_reqQueue(){
    networkAccessible = m_manager.networkAccessible() == QNetworkAccessManager::Accessible;
    qDebug() << networkAccessible;
    QObject::connect(&m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    QObject::connect(&m_manager, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)),
                     this, SLOT(onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)));
    QObject::connect(&m_configuration, SIGNAL(onlineStateChanged(bool)), this, SLOT(onOnlineStateChanged(bool)));

}

void NetworkManagerService::cancel() {
    m_reqQueue.clear();
    filesNbr = 0;
    fileDownloadedNbr = 0;
}

void NetworkManagerService::requestCommonFile(QString const &url, QString const &software) {
    m_url = url;
    m_software = software;

    if (!m_reqQueue.empty()) {
        QNetworkRequest req = m_reqQueue.dequeue();
        requestSetup(req);
    } else {
        QObject::disconnect(&m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadApplicationFileFinished(QNetworkReply*)));
        QObject::connect(&m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadCommonFileFinished(QNetworkReply*)));

        QNetworkRequest request(url + software + "_files.updater");
        requestSetup(request);
    }
}

void NetworkManagerService::downloadCommonFileFinished(QNetworkReply *reply) {
    //qDebug() << reply->readAll();
    if (reply->error() == QNetworkReply::OperationCanceledError) {
        qDebug() << "OperationCanceledError" << " ligne 50 NetworkManagerService";
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {

    QObject::disconnect(&m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadCommonFileFinished(QNetworkReply*)));
    QObject::connect(&m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadApplicationFileFinished(QNetworkReply*)));

    QList<QByteArray> lines = reply->readAll().split('\n');
    filesNbr = 0;
    fileDownloadedNbr = 0;
    foreach ( const QByteArray &line, lines)
    {
        QByteArray ref = line;
        if (line.size() > 4) {
        if (m_software == "mac") {
            ref.remove(0, 4);
        }
        QString urlReq(m_url + m_software + "/" + ref);

        m_map[urlReq] = ref;
        QNetworkRequest request(urlReq);
        m_reqQueue.enqueue(request);

        filesNbr++;
        }

    }

    if (!m_reqQueue.empty()) {
        QNetworkRequest req = m_reqQueue.dequeue();
        requestSetup(req);
    }

    } else {
        m_reqQueue.enqueue(reply->request());
        emit internetFailed();
        qDebug() << reply->errorString();
    }
    qDebug() << "FILESNBR: " << filesNbr;
}

void NetworkManagerService::downloadApplicationFileFinished(QNetworkReply *reply) {
  //  qDebug() << reply->readAll();

    if (reply->error() == QNetworkReply::OperationCanceledError) {
        qDebug() << "OperationCanceledError" << " ligne 96 NetworkManagerService";
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        if (!m_reqQueue.empty()) {
            QNetworkRequest req = m_reqQueue.dequeue();
            requestSetup(req);
        }


    QDir dir = QDir::temp();
   // qDebug() << "DIR: " << dir.path();
    QString pathFile = m_map[reply->url().toEncoded()];
    while (pathFile[pathFile.size() - 1] != '/') {
        pathFile.remove(pathFile.size()  - 1, 1);
    }
    bool success = dir.mkpath(pathFile);

    QFile localFile(dir.path() + "/" + m_map[reply->url().toEncoded()]);
 //   qDebug() << "FILE: " << m_map[reply->url().toEncoded()];
 //   qDebug() << "--end--";

    fileDownloadedNbr++;

    emit avancementChanged();

        if (!localFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            return;
        }

        localFile.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner | QFile::ReadUser
                                  | QFile::ExeUser  | QFile::ReadGroup
                                   | QFile::ExeGroup );

        localFile.write(reply->readAll());
        localFile.close();

    } else {
        m_reqQueue.enqueue(reply->request());
        emit internetFailed();
        qDebug() << reply->errorString();
    }

    if (fileDownloadedNbr >= filesNbr) {
        emit downloadSuccess();
    }
}

double NetworkManagerService::getAvancement() const {
    if (filesNbr != 0) {
        qDebug() << fileDownloadedNbr << " / " << filesNbr;
        qDebug() << (((double)fileDownloadedNbr / (double)filesNbr));
        return (((double)fileDownloadedNbr / (double)filesNbr));
    }
    return (0);
}

void NetworkManagerService::requestSetup(QNetworkRequest &req) {
    //qDebug() << "ADDED";
    QNetworkReply* reply = m_manager.get(req);
    ReplyTimeout *replyT = ReplyTimeout::set(reply, 20000);
    connect(replyT, SIGNAL(timeout(QNetworkRequest)), this, SLOT(timeoutReply(QNetworkRequest)));
  //  connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onRequestReplyError(QNetworkReply::NetworkError)));
}

 void NetworkManagerService::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {
     qDebug() << "ERROR";
    foreach (QSslError const &error, errors) {
        qDebug() << error;
        if (error.error() != QSslError::SslError::NoError) {
            emit internetFailed();
            return;
        }
    }
 }

 void NetworkManagerService::onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible) {
    networkAccessible = accessible == QNetworkAccessManager::Accessible;
    qDebug() << "enter";
    if (!networkAccessible) {
        emit internetFailed();
    }
 }

 void NetworkManagerService::onOnlineStateChanged(bool state) {
    qDebug() << state << " CHANGED";
    if (!state) {
        networkAccessible = false;
        emit internetFailed();
    }
 }

 /*void NetworkManagerService::onRequestReplyError(QNetworkReply::NetworkError error) {
    if (error != QNetworkReply::NetworkError::NoError && error != QNetworkReply::NetworkError::OperationCanceledError) {
        qDebug() << "error";
        qDebug() << error;
        emit internetFailed();
    }
 }*/

 void NetworkManagerService::timeoutReply(QNetworkRequest req){
    qDebug() << "error TIMEOUT";
    m_reqQueue.enqueue(req);
    emit internetFailed();
 }
