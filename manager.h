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
    bool isExecuter(const QString &type);
    QJsonArray getHostsList(const QString &hallId);
    int _port;
    void executerTask(const QJsonArray hosts, const QString &dataIn, const QString &hostSender);
    void sshTask(const QJsonArray hosts, const QJsonArray bashes, const QString &hostSender, const QString &kind, const QString &method);
    QJsonArray compareBashes(QJsonArray bashes, const QString &kind, const QString &ip);
public:
    Manager(QObject *parent);
    void taskSwitch(const QString &type, const QJsonObject dataJsonObj, const QString &hostSender);
private slots:
    void onSshFinished(const QStringList finishedData);
signals:
    void sendBack(QString, QString);
};

#endif // MANAGER_H
