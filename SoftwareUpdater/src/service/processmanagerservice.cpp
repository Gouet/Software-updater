#include <QDebug>
#include "include/service/processmanagerservice.h"

ProcessManagerService::ProcessManagerService(const QString &applicationName, QString const &applicationPath, QObject *parent)
    : QObject(parent),
      m_applicationName(applicationName),
      m_applicationPath(applicationPath) {

}

void ProcessManagerService::killApplication() {
    QProcess p_process;

#if defined(Q_OS_MAC)
    p_process.start("pkill " + m_applicationName);
  #else
    p_process.start("taskkill /f /im " + m_applicationName );
    p_process.waitForFinished();
    p_process.start("taskkill /f /im CoreDaemon.exe");
    p_process.waitForFinished();
    p_process.start("taskkill /f /im Server.exe");
    p_process.waitForFinished();
#endif

    p_process.waitForFinished();
}

bool ProcessManagerService::applicationExist() {
    QProcess tasklist;

#if defined(Q_OS_MAC)
    tasklist.start("pgrep -l " + m_applicationName);
  #else
    tasklist.start("tasklist /FI \"IMAGENAME eq  " + m_applicationName + "\"");
#endif

    tasklist.waitForFinished();
    QString output = tasklist.readAllStandardOutput();
    return output.contains(m_applicationName);
}

bool ProcessManagerService::launchApplication() {
    bool success = false;

#if defined(Q_OS_MAC)
    success = QProcess::startDetached("open " + m_applicationPath + "/" + m_applicationName + ".app");
  #else
    success = QProcess::startDetached(m_applicationPath + "/" + m_applicationName);
#endif
    if (!success) {
        qDebug() << "Failed to launch application";
    } else {
        qDebug() << "SUCESS";
    }
    return true;
}
