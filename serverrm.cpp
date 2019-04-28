#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include "serverrm.h"

ServerRM::ServerRM(const quint16 nPort, QObject *parent) :
    QObject(parent),
    _pWebSocketServer(new QWebSocketServer(QStringLiteral("Service remote-manager server"),
                                           QWebSocketServer::NonSecureMode, this))
{
    qDebug() << "Starting Remote Manager server...";
    connect(_pWebSocketServer, &QWebSocketServer::newConnection, this, &ServerRM::slotNewConnection);
    connect(_pWebSocketServer, &QWebSocketServer::closed, this, &ServerRM::closed);

    if (!_pWebSocketServer->listen(QHostAddress::Any, nPort)) {
        errorMessage();
    } else {
        qDebug() << "Success!";
        qDebug().noquote() << "Service server listening on port:" << nPort;
    }
}

ServerRM::~ServerRM()
{
    qWarning() << "WebSocket closed. Terminating service...";

    _pWebSocketServer->close();
}

bool ServerRM::isStarted()
{
    return _isStarted;
}

void ServerRM::slotReadyRead(const QString &data)
{
    QWebSocket *clientSocket = qobject_cast<QWebSocket *>(sender());

    qDebug().noquote() << "Data from client:" << data;
}

void ServerRM::errorMessage()
{
    qWarning() << "Server Error:" << "Unable to start the server:"
               << _pWebSocketServer->errorString();

    _pWebSocketServer->close();
    _isStarted = false;

    return;
}

void ServerRM::slotNewConnection()
{
    QWebSocket *clientSocket = _pWebSocketServer->nextPendingConnection();

    _clientList << clientSocket;

    qDebug() << "Client" << clientSocket << "has been connected!";
    qDebug() << "Connected clients:" << _clientList;

    connect(clientSocket, &QWebSocket::textMessageReceived, this, &ServerRM::slotReadyRead);
    connect(clientSocket, &QWebSocket::disconnected, this, &ServerRM::slotDisconnected);
}

void ServerRM::sendToClient(const QString &data, QWebSocket *pClient)
{
    pClient->sendTextMessage(data);
}

void ServerRM::slotDisconnected()
{
    QWebSocket* clientSocket = qobject_cast<QWebSocket*>(QObject::sender());

    _clientList.removeOne(clientSocket);

    qDebug() << "Client" << clientSocket << "has been disconnected!";
    qDebug() << "Connected clients" << _clientList;

    clientSocket->deleteLater();
}
