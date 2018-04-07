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
      m_url(),
      m_software() {

}

void NetworkManagerService::requestCommonFile(QString const &url, QString const &software) {
    QNetworkRequest request(url + software + "_files.updater");
    m_url = url;
    m_software = software;
    QObject::connect(&m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadCommonFileFinished(QNetworkReply*)));

    m_manager.get(request);
}

void NetworkManagerService::downloadCommonFileFinished(QNetworkReply *reply) {
    //qDebug() << reply->readAll();

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
        filesNbr++;
        m_manager.get(request);
        }
    }
    qDebug() << "FILESNBR: " << filesNbr;
}

void NetworkManagerService::downloadApplicationFileFinished(QNetworkReply *reply) {
  //  qDebug() << reply->readAll();
    QDir dir = QDir::current();
    QString pathFile = m_map[reply->url().toEncoded()];
    while (pathFile[pathFile.size() - 1] != '/') {
        pathFile.remove(pathFile.size()  - 1, 1);
    }
    bool success = dir.mkpath(pathFile);
  //  qDebug() << m_map[reply->url().toEncoded()];
    qDebug() << success;
    QFile localFile(m_map[reply->url().toEncoded()]);

    fileDownloadedNbr++;

    qDebug() << fileDownloadedNbr;

   // localFile.setPermissions(0x7777);


        if (!localFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
            return;
        }

        localFile.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner | QFile::ReadUser
                                  | QFile::ExeUser  | QFile::ReadGroup
                                   | QFile::ExeGroup );

        localFile.write(reply->readAll());
        localFile.close();
}
