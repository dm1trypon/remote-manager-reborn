#ifndef CLIENTEXECUTER_H
#define CLIENTEXECUTER_H

#include "parser.h"

#include <QNetworkSession>
#include <QtWebSockets/QWebSocket>

QT_FORWARD_DECLARE_CLASS(QWebSocket)

class ClientExecuter : public QObject
{
    Q_OBJECT
public:
    explicit ClientExecuter(const QUrl &url, const QString &dataIn);
    ~ClientExecuter();

private slots:
    void onConnected();
    void onTextMessageReceived(const QString &message);
    void onSslErrors(const QList<QSslError> &errors);

private:
    const QString CONNECTION = "connection";

    Parser *_parser = nullptr;
    QString _dataIn;
    QWebSocket _webSocket;
    QString _url;
};

#endif // CLIENTEXECUTER_H

