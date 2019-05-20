#ifndef MANAGER_H
#define MANAGER_H

#include "parser.h"
#include "sshexecuter.h"

#include <QObject>
#include <QVector>

class Manager : public QObject
{
    Q_OBJECT
private:
    Parser *_parser = nullptr;
    SshExecuter *_sshExecuter = nullptr;

    QJsonArray getHostsList(const QString &hallId);

    bool isExecuter(const QString &type);

    int _port;

    void executerTask(const QJsonArray hosts, const QMap<QString, QString> dataIn, const QString &hostSender, const QString &method);
    void sshTask(const QJsonArray hosts, const QJsonArray bashes, const QString &hostSender, const QString &kind,
                 const QString &method);

    QJsonArray compareBashes(QJsonArray bashes, const QString &kind, const QString &ip);

public:
    Manager(const int port, Parser *parser, SshExecuter *sshExecuter, QObject *parent);

    void taskSwitch(const QString &type, const QJsonObject dataJsonObj, const QString &hostSender);

private slots:
    void onSshFinished(QMap<QString, QString> finishedData);

signals:
    void sendBack(QString, QString);
};

#endif // MANAGER_H
