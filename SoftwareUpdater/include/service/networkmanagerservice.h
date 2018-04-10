#ifndef NETWORKMANAGERSERVICE_H
#define NETWORKMANAGERSERVICE_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkConfigurationManager>
#include <QList>
#include <QNetworkReply>
#include <QSslError>
#include <QBasicTimer>
#include <QQueue>

class QNetworkReply;

class NetworkManagerService : public QObject {
    Q_OBJECT

public:
    NetworkManagerService(QObject *parent = 0);

    void requestCommonFile(QString const &url, QString const &software);
    void cancel();

private:
    void requestSetup(QNetworkRequest &);

public slots:
    void downloadCommonFileFinished(QNetworkReply *);
    void downloadApplicationFileFinished(QNetworkReply *);

    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility);
    void onOnlineStateChanged(bool);
    void timeoutReply(QNetworkRequest);

public:
    double getAvancement() const;

signals:
    void avancementChanged();
    void internetFailed();
    void downloadSuccess();
    void copySuccess();

private:
    QNetworkAccessManager m_manager;
    QNetworkConfigurationManager m_configuration;
    QString m_url;
    QString m_software;
    QMap<QString, QString> m_map;
    int filesNbr;
    int fileDownloadedNbr;
    bool networkAccessible;
    QQueue<QNetworkRequest> m_reqQueue;

};

class ReplyTimeout : public QObject {
  Q_OBJECT
  QBasicTimer m_timer;

signals:
  void timeout(QNetworkRequest);

public:
 // enum HandleMethod { Abort, Close };

  ReplyTimeout(QNetworkReply* reply, const int timeout) :
    QObject(reply)
  {
    Q_ASSERT(reply);
    if (reply && reply->isRunning()) {
      m_timer.start(timeout, this);
      connect(reply, &QNetworkReply::finished, this, &QObject::deleteLater);
  }
  }
  static ReplyTimeout *set(QNetworkReply* reply, const int timeout) {
    return new ReplyTimeout(reply, timeout);
  }

protected:
  void timerEvent(QTimerEvent * ev) {
    if (!m_timer.isActive() || ev->timerId() != m_timer.timerId())
      return;
    auto reply = static_cast<QNetworkReply*>(parent());
        if (reply->isRunning()) {
            emit timeout(reply->request());
            reply->close();
        }
    m_timer.stop();
  }
};

#endif // NETWORKMANAGERSERVICE_H
