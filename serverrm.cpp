#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"

#include "serverrm.h"
#include "inits.h"
#include "log.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPair>

ServerRM::ServerRM(const quint16 nPort, QObject *parent) :
    QObject(parent),
    _pWebSocketServer(new QWebSocketServer(QStringLiteral("Service remote-manager server"),
                                           QWebSocketServer::NonSecureMode, this)),
    _parser(Inits::Instance().getParser()),
    _manager(Inits::Instance().getManager())
{
    infoServerRM << "Starting Remote Manager server...";
    connect(_pWebSocketServer, &QWebSocketServer::newConnection, this, &ServerRM::onNewConnection);
    connect(_pWebSocketServer, &QWebSocketServer::closed, this, &ServerRM::closed);

    if (!_pWebSocketServer->listen(QHostAddress::Any, nPort)) {
        _isStarted = false;
        errorMessage();

        return;
    }

    infoServerRM << "Service server listening on port:" << nPort;
}

ServerRM::~ServerRM()
{
    errorServerRM << "WebSocket closed. Terminating service...";

    _pWebSocketServer->close();
}

bool ServerRM::isStarted()
{
    return _isStarted;
}

void ServerRM::onReadyRead(const QString &data)
{
    infoServerRM << "Data from client:" << data;

    const QPair<QJsonObject, QString> dataPair = _parser->fromJson(data);

    if (dataPair.first.isEmpty() || dataPair.second.isEmpty()) {
        errorServerRM << "Incorrect input data:" << data;

        return;
    }

    _manager->taskSwitch(dataPair.second, dataPair.first);
}

void ServerRM::errorMessage()
{
    errorServerRM << "Server Error:" << "Unable to start the server:" << _pWebSocketServer->errorString();

    _pWebSocketServer->close();

    return;
}

void ServerRM::onNewConnection()
{
    QWebSocket *clientSocket = _pWebSocketServer->nextPendingConnection();

    _clientList << clientSocket;

    infoServerRM << "Client" << clientSocket->requestUrl().host() << "has been connected!";
    infoServerRM << "Connected clients:" << _clientList;

    connect(clientSocket, &QWebSocket::textMessageReceived, this, &ServerRM::onReadyRead);
    connect(clientSocket, &QWebSocket::disconnected, this, &ServerRM::onDisconnect);
}

void ServerRM::onSend(const QString &data, QWebSocket *pClient)
{
    pClient->sendTextMessage(data);
}

void ServerRM::onDisconnect()
{
    QWebSocket* clientSocket = qobject_cast<QWebSocket*>(QObject::sender());

    _clientList.removeOne(clientSocket);

    infoServerRM << "Client" << clientSocket << "has been disconnected!";
    infoServerRM << "Connected clients" << _clientList;

    clientSocket->deleteLater();
}
