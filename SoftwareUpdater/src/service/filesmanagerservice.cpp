#include <QDebug>
#include "include/service/filesmanagerservice.h"

FilesManagerService::FilesManagerService(QString const &tmpApplication, QString const & destApplication, QObject *parent)
    : QObject(parent),
    m_tmpApplication(tmpApplication),
    m_destApplication(destApplication),
#if defined(Q_OS_MAC)
    m_tmpApplicationName("tmpApp" + QString::number(qrand()) + ".app")
  #else
    m_tmpApplicationName("tmpApp" + QString::number(qrand()))
#endif

{
}

void FilesManagerService::moveFiles() {
    QDir dir;

    if (!dir.rename(m_destApplication, QDir::tempPath() + "/" + m_tmpApplicationName)) {
        emit filesMovedFailed("FIRST: " + m_destApplication + "   " + QDir::tempPath() + "/" + m_tmpApplicationName);
        return;
    }

    if ( !dir.rename(m_tmpApplication, m_destApplication) ) {
        if (!dir.rename(QDir::tempPath() + "/" + m_tmpApplicationName, m_destApplication)) {
        }
        emit filesMovedFailed("SECOND: " + m_tmpApplication + "   " + m_destApplication);
        return;
    } else {
        emit filesMovedSuccess();
        removeTmpFile();
    }
}

void FilesManagerService::removeTmpFile() {

#if defined(Q_OS_MAC)
    QDir dir;
    if (dir.remove(QDir::tempPath() + "/" + m_tmpApplicationName) == true) {
        qDebug() << "remove success";
    } else {
        qDebug() << "remove failed";
    }
#else
    QDir dir(QDir::tempPath() + "/" + m_tmpApplicationName);

    if (dir.removeRecursively() == true) {
        qDebug() << "remove success";
    } else {
        qDebug() << "remove failed";
    }
#endif
}
