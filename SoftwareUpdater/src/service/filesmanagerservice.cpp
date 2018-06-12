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

static bool rmDir(const QString &dirPath)
{
    QDir dir(dirPath);
    if (!dir.exists())
        return true;
    foreach(const QFileInfo &info, dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        if (info.isDir()) {
            if (!rmDir(info.filePath()))
                return false;
        } else {
            if (!dir.remove(info.fileName()))
                return false;
        }
    }
    QDir parentDir(QFileInfo(dirPath).path());
    return parentDir.rmdir(QFileInfo(dirPath).fileName());
}

static QString toto = "";
static bool cpDir(const QString &srcPath, const QString &dstPath, bool willRmDir)
{
    if (willRmDir)
        rmDir(dstPath);

    QDir srcDir(srcPath);
    foreach(const QFileInfo &info, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        QString srcItemPath = srcPath + "/" + info.fileName();
        QString dstItemPath = dstPath + "/" + info.fileName();
        //QDir dir = QDir::temp();
        QString pathFile = dstItemPath;
        while (pathFile[pathFile.size() - 1] != '/') {
            pathFile.remove(pathFile.size()  - 1, 1);
        }
        QDir dir(dstPath);
        qDebug() << " --- ";
        qDebug() << dstPath;
        qDebug() << pathFile;
        bool success = dir.mkpath(pathFile);
        qDebug() << success << "  " << pathFile;

        if (info.isDir()) {
            if (!cpDir(srcItemPath, dstItemPath, willRmDir)) {
                toto = "toto: " + srcItemPath + "  " + dstItemPath;
               return false;
            }
        } else if (info.isFile()) {
            if (!QFile::copy(srcItemPath, dstItemPath)) {
                toto = "tutu: " + srcItemPath + "  " + dstItemPath;
               return false;
            }
        } else {
            qDebug() << "Unhandled item" << info.filePath() << "in cpDir";
        }
    }
    return true;
}

void FilesManagerService::moveFiles() {
    QDir dir;

#if defined(Q_OS_MAC)
    if (!dir.rename(m_destApplication, QDir::tempPath() + "/" + m_tmpApplicationName)) {
      //  emit filesMovedFailed("FIRST: " + m_destApplication + "   " + QDir::tempPath() + "/" + m_tmpApplicationName);
   //     return;
    }

    if ( !dir.rename(m_tmpApplication, m_destApplication) ) {
        if (!dir.rename(QDir::tempPath() + "/" + m_tmpApplicationName, m_destApplication)) {}
        emit filesMovedFailed("SECOND: " + m_tmpApplication + "   " + m_destApplication);
    } else {
        qDebug() << "move other";
        emit filesMovedSuccess();
      //  removeTmpFile();
    }
#else
    bool pDirGood = cpDir(m_destApplication, QDir::tempPath() + "/" + m_tmpApplicationName, false);
    if (pDirGood) {
        qDebug() << "OK";
    } else {
        emit filesMovedFailed(toto);
        return;
    }
    if ( !cpDir(m_tmpApplication, m_destApplication, true) ) {
        emit filesMovedFailed(toto);
        if (!cpDir(QDir::tempPath() + "/" + m_tmpApplicationName, m_destApplication, true)) {
        }
//        emit filesMovedFailed(toto);
        return;
    } else {
        emit filesMovedSuccess();
      //  removeTmpFile();
    }

#endif
        QDir dir2(QDir::tempPath() + "/" + m_tmpApplicationName);
        dir2.removeRecursively();
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
    QDir dir2(m_tmpApplication);

    if (dir2.removeRecursively() == true) {
        qDebug() << "remove success";
    } else {
        qDebug() << "remove failed";
    }

    QDir dir3(QDir::tempPath() + "/DNAI_UPDATER");

    if (dir3.removeRecursively() == true) {
        qDebug() << "remove success";
    } else {
        qDebug() << "remove failed";
    }

#endif
}
