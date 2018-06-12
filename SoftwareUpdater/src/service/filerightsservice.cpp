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
#include "include/service/filerightsservice.h"

FileRightsService::FileRightsService() {

}

void FileRightsService::setPermission(QFile &file, QString const &permission) {
    QFile::Permissions perm;

    if (permission[0] == QChar('7')) {
        perm = QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner | QFile::ReadUser
                | QFile::ExeUser | QFile::WriteUser;
    }
    else if (permission[0] == QChar('6')) {
        perm = QFile::ReadOwner | QFile::ReadUser | QFile::WriteOwner | QFile::WriteUser;
    } else if (permission[0] == QChar('5')) {
        perm = QFile::ReadOwner |  QFile::ExeOwner | QFile::ReadUser
                | QFile::ExeUser;
    } else if (permission[0] == QChar('4')) {
        perm = QFile::ReadOwner | QFile::ReadUser;
    } else if (permission[0] == QChar('2')) {
        perm = QFile::WriteOwner | QFile::WriteUser;
    } else {
        perm = QFile::ExeOwner | QFile::ExeUser;
    }


    if (permission[1] == QChar('7')) {
        perm |= QFile::ReadGroup | QFile::ExeGroup | QFile::WriteGroup;

    } else if (permission[1] == QChar('6')) {
        perm = QFile::ReadGroup | QFile::WriteGroup;
    } else if (permission[1] == QChar('5')) {
        perm |= QFile::ReadGroup |  QFile::ExeGroup;
    } else if (permission[1] == QChar('4')) {
        perm |= QFile::ReadGroup;
    } else if (permission[1] == QChar('2')) {
        perm |= QFile::WriteGroup;
    } else {
        perm |= QFile::ExeGroup;
    }


    if (permission[2] == QChar('7')) {
        perm |= QFile::ReadOther | QFile::ExeOther | QFile::WriteOther;
    } else if (permission[2] == QChar('6')) {
        perm = QFile::ReadOther | QFile::WriteOther;
    } else if (permission[2] == QChar('5')) {
        perm |= QFile::ReadOther |  QFile::ExeOther ;
    } else if (permission[2] == QChar('4')) {
        perm |= QFile::ReadOther;
    } else if (permission[2] == QChar('2')) {
        perm |= QFile::WriteOther;
    } else {
        perm |= QFile::ExeOther;
    }
#ifdef Q_OS_MAC
    qDebug() << "permission set " << permission;
    file.setPermissions(perm);
#endif
}
