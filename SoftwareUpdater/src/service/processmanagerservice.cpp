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
    p_process.start("taskkill /IM " + m_applicationName + " /F");
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
#if defined(Q_OS_MAC)
    QProcess::startDetached("open " + m_applicationPath + "/" + m_applicationName + ".app");
  #else
    QProcess::startDetached(m_applicationPath + "\\" + m_applicationName + ".exe");
#endif
    return true;
}
