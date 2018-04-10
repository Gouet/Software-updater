#include <QDebug>
#include "include/service/filesmanagerservice.h"

FilesManagerService::FilesManagerService(QString const &tmpApplication, QString const & destApplication, QObject *parent)
    : QObject(parent),
    m_tmpApplication(tmpApplication),
    m_destApplication(destApplication),
#if defined(Q_OS_MAC)
    m_tmpApplicationName("DNAI" + QString::number(qrand()) + ".app")
  #else
    m_tmpApplicationName("DNAItmp" + QString::number(qrand()))
#endif

{
    qDebug() << m_tmpApplication;
    qDebug() << m_destApplication;
}

void FilesManagerService::moveFiles() {
    QDir dir;

    if (!dir.rename(m_destApplication, QDir::tempPath() + "/" + m_tmpApplicationName)) {
        qDebug() << "FILES FAILED 1";
        emit filesMovedFailed();
        return;
    }
    if ( !dir.rename(m_tmpApplication, m_destApplication) ) {
        qDebug() << "FILES FAILED 2";
        if (!dir.rename(QDir::tempPath() + "/" + m_tmpApplicationName, m_destApplication)) {
            qDebug() << "FAILED MASSIF";
        }
        emit filesMovedFailed();
        return;
    } else {
        qDebug() << "FILES SUCCESS";
        emit filesMovedSuccess();
        removeTmpFile();
    }
}

void FilesManagerService::removeTmpFile() {
    QDir dir;

    if (dir.remove(QDir::tempPath() + "/" + m_tmpApplicationName) == true) {
        qDebug() << "remove success";
    } else {
        qDebug() << "remove failed";
    }
}
