#include "clientexecuter.h"
#include "inits.h"
#include "log.h"

#include <QtCore/QDebug>
#include <QtWebSockets/QWebSocket>
#include <QCoreApplication>

ClientExecuter::ClientExecuter(const QUrl &url, const QString &dataIn, QObject *parent):
    QObject(parent),
    _parser(Inits::Instance().getParser()),
    _dataIn(dataIn),
    _url(url.toDisplayString())
{
    infoClientEx << "Attemt connect to server:" << url << ". Data:" << dataIn;
    connect(&_webSocket, &QWebSocket::connected, this, &ClientExecuter::onConnected);

    typedef void (QWebSocket:: *sslErrorsSignal)(const QList<QSslError> &);

    _url = _url.remove("wss://");
    _url = _url.left(_url.indexOf(':'));

    connect(&_webSocket, static_cast<sslErrorsSignal>(&QWebSocket::sslErrors), this, &ClientExecuter::onSslErrors);
    connect(&_webSocket, static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
            [=](QAbstractSocket::SocketError) {
        warnClientEx << "Host:" << _url << ". Error:" << _webSocket.errorString();

        deleteLater();

        return;
    });

    _webSocket.open(QUrl(url));
}

ClientExecuter::~ClientExecuter()
{
    infoClientEx << "Connection to host" << _url << "has been closed.";
}

void ClientExecuter::onConnected()
{
    connect(&_webSocket, &QWebSocket::textMessageReceived, this, &ClientExecuter::onTextMessageReceived);

    infoClientEx << "Connection open to host:" << _url;
    _webSocket.sendTextMessage(_dataIn);
}

void ClientExecuter::onTextMessageReceived(const QString &dataOut)
{
    infoClientEx << "Message received:" << dataOut;

    if (_parser->isFinished(dataOut)) {
        deleteLater();

        return;
    }

    if (!_parser->isValidResult(dataOut, _dataIn)) {
        return;
    }
}

void ClientExecuter::onSslErrors(const QList<QSslError> &errors)
{
    Q_UNUSED(errors);
    _webSocket.ignoreSslErrors();
}
