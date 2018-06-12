#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QNetworkReply>
#include <QList>
#include <QByteArray>
#include <QFile>
#include <QDir>
#include <QtNetwork>
#include <QSslSocket>
#include <iterator>
#include "include/service/networkmanagerservice.h"
#include "include/service/filerightsservice.h"

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
      m_reqQueue(),
      m_rights() {
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

#ifdef Q_OS_MAC
        QDir dir(QDir::tempPath() + "/" + "DNAI.app");
#else
        QDir dir(QDir::tempPath() + "/" + "DNAI");
#endif
        dir.removeRecursively();

        QNetworkRequest request(url + software + "_files.updater");
        m_rights.clear();
        requestSetup(request);
    }
}

void NetworkManagerService::downloadCommonFileFinished(QNetworkReply *reply) {

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
            QByteArray copyRight = ref.left(3);
            ref.remove(0, 4);
            m_rights[QString(ref.toStdString().c_str())] = QString(copyRight.toStdString().c_str());
        } else if (m_software == "windows") {
            ref.replace("\\", "/");
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
        emit internetFailed(reply->url().toString());
        qDebug() << reply->errorString();
    }
    qDebug() << "FILESNBR: " << filesNbr;
}

void NetworkManagerService::downloadApplicationFileFinished(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError) {
        if (!m_reqQueue.empty()) {
            QNetworkRequest req = m_reqQueue.dequeue();
            requestSetup(req);
        }


    QDir dir = QDir::temp();
    QString pathFile = m_map[reply->url().toEncoded()];
    while (pathFile[pathFile.size() - 1] != '/') {
        pathFile.remove(pathFile.size()  - 1, 1);
    }
    bool success = dir.mkpath(pathFile);

    QFile localFile(dir.path() + "/" + m_map[reply->url().toEncoded()]);

    QFileInfo fileInfo(localFile.fileName());
    QString filename(fileInfo.absolutePath());
    QString rights = "777";

    qDebug() << filename;
    qDebug() << pathFile;
  //  qDebug() << "rly ? => " << m_rights.find(m_map[reply->url().toEncoded()]);
    QMap<QString, QString>::iterator it = m_rights.find(m_map[reply->url().toEncoded()]);
    if (it != m_rights.end()) {
        qDebug() << it.value();
        rights = it.value();
    }

    fileDownloadedNbr++;

    emit avancementChanged();

        if (!localFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            qDebug() << "truncate";
            if (!localFile.open(QIODevice::WriteOnly)) {
                qDebug() << "failed for " << localFile.fileName();
                return;
            }
            //return;
        }

        FileRightsService rightsService;

        rightsService.setPermission(localFile, rights);


     /*   localFile.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner | QFile::ReadUser
                                  | QFile::ExeUser  | QFile::ReadGroup
                                   | QFile::ExeGroup ); */

        localFile.write(reply->readAll());
        localFile.close();

    } else {
        m_reqQueue.enqueue(reply->request());
        emit internetFailed(reply->url().toString());
        qDebug() << reply->errorString();
    }

    qDebug() << "download ?";
    if (fileDownloadedNbr >= filesNbr && fileDownloadedNbr != 0 && filesNbr != 0) {
        qDebug() << "DOWNLOAD Sucess ???";
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
    QNetworkReply* reply = m_manager.get(req);
    ReplyTimeout *replyT = ReplyTimeout::set(reply, 60000);
    connect(replyT, SIGNAL(timeout(QNetworkRequest)), this, SLOT(timeoutReply(QNetworkRequest)));
}

 void NetworkManagerService::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {
    foreach (QSslError const &error, errors) {
        qDebug() << error;
        if (error.error() != QSslError::SslError::NoError) {
            emit internetFailed(reply->url().toString());
            return;
        }
    }
 }

 void NetworkManagerService::onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible) {
    networkAccessible = accessible == QNetworkAccessManager::Accessible;
    if (!networkAccessible) {
        emit internetFailed("");
    }
 }

 void NetworkManagerService::onOnlineStateChanged(bool state) {
    if (!state) {
        networkAccessible = false;
        emit internetFailed("");
    }
 }

 void NetworkManagerService::timeoutReply(QNetworkRequest req){
    m_reqQueue.enqueue(req);
    emit internetFailed("");
 }
