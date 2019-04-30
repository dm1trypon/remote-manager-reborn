#include "clientexecuter.h"
#include "inits.h"

#include <QtCore/QDebug>
#include <QtWebSockets/QWebSocket>
#include <QCoreApplication>

ClientExecuter::ClientExecuter(const QUrl &url, const QString &dataIn):
    _parser(Inits::Instance().getParser()),
    _dataIn(dataIn),
    _url(url.toDisplayString())
{
    qDebug().noquote() << "Attemt connect to server:" << url << ". Data:" << dataIn;
    connect(&_webSocket, &QWebSocket::connected, this, &ClientExecuter::onConnected);

    typedef void (QWebSocket:: *sslErrorsSignal)(const QList<QSslError> &);

    _url = _url.remove("wss://");
    _url = _url.left(_url.indexOf(':'));

    connect(&_webSocket, static_cast<sslErrorsSignal>(&QWebSocket::sslErrors), this, &ClientExecuter::onSslErrors);
    connect(&_webSocket, static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
            [=](QAbstractSocket::SocketError) {
        qWarning() << "Host:" << _url << ". Error:" << _webSocket.errorString();

        deleteLater();

        return;
    });

    _webSocket.open(QUrl(url));
    qDebug().noquote() << "Connection open to host:" << _url;
}

ClientExecuter::~ClientExecuter()
{
    qDebug().noquote() << "Connection to host" << _url << "has been closed.";
}

void ClientExecuter::onConnected()
{
    connect(&_webSocket, &QWebSocket::textMessageReceived, this, &ClientExecuter::onTextMessageReceived);

    _webSocket.sendTextMessage(_dataIn);
}

void ClientExecuter::onTextMessageReceived(const QString &dataOut)
{
    qDebug().noquote() << "Message received:" << dataOut;

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
