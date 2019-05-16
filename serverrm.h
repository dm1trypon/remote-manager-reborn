#ifndef SERVERRM_H
#define SERVERRM_H

#include "manager.h"

#include <QNetworkSession>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class ServerRM: public QObject
{
    Q_OBJECT
public:
    ServerRM(const quint16 nPort, QObject *parent = nullptr);
    ~ServerRM();

    bool isStarted();
private slots:
    void onReadyRead(const QString &data);
    void onNewConnection();
    void onDisconnect();
    void onSend(const QString &data, const QString &hostSender); // send data to clients

signals:
    void closed();

private:
    QString _pathKey;
    QString _pathCert;
    QWebSocketServer *_pWebSocketServer;
    QList<QWebSocket *> _clientList;
    Parser *_parser = nullptr;
    Manager *_manager = nullptr;

    bool _isStarted = true;


    void errorMessage();
};

#endif // SERVERRM_H
