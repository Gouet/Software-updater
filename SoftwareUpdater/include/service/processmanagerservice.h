#ifndef PROCESSMANAGERSERVICE_H
#define PROCESSMANAGERSERVICE_H

#include <QProcess>
#include <QString>
#include <QObject>

class ProcessManagerService : public QObject {
    Q_OBJECT

public:
    ProcessManagerService(QString const &applicationName, QString const &applicationPath, QObject *parent = 0);

    void killApplication();
    bool applicationExist();
    bool launchApplication();

private:
    QString m_applicationName;
    QString m_applicationPath;
};

#endif // PROCESSMANAGERSERVICE_H
