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
    SshExecuter *_sshExecuter = nullptr;
    bool isExecuter(const QString &type);
    QJsonArray getHostsList(const QString &hallId);
    int _port;
    void executerTask(const QJsonArray hosts, const QString &dataIn);
    void sshTask(const QJsonArray hosts, const QJsonArray bashes);
public:
    Manager(QObject *parent);
    void taskSwitch(const QString &type, const QJsonObject dataJsonObj);
private slots:
    void onSshFinished(const QString &result);
};

#endif // MANAGER_H
