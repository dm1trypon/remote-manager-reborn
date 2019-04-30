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

    void sendToClient(const QString &data, QWebSocket *pClient); // send data to clients
    bool isStarted();
private slots:
    void slotReadyRead(const QString &data);
    void slotNewConnection();
    void slotDisconnected();

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
