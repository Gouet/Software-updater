#ifndef FILERIGHTSSERVICE_H
#define FILERIGHTSSERVICE_H

#include <QDir>
#include <QObject>
#include <QString>

class FileRightsService : public QObject {
    Q_OBJECT

public:
    FileRightsService();

    void setPermission(QFile &file, QString const &permission);

private:
};

#endif // FILERIGHTSSERVICE_H
