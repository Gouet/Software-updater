#ifndef FILESMANAGERSERVICE_H
#define FILESMANAGERSERVICE_H

#include <QDir>
#include <QObject>
#include <QString>

class FilesManagerService : public QObject {
    Q_OBJECT

public:
    FilesManagerService(QString const &tmpApplication, QString const &destApplication, QObject *parent = 0);

    void moveFiles();
    void removeTmpFile();

signals:
    void filesMovedSuccess();
    void filesMovedFailed(QString);

private:
    QString m_tmpApplication;
    QString m_destApplication;
    QString m_tmpApplicationName;
};

#endif // FILESMANAGERSERVICE_H
