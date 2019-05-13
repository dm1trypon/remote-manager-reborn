#include "manager.h"
#include "clientexecuter.h"
#include "inits.h"
#include "log.h"
#include "database.h"

#include <QJsonDocument>
#include <QUrl>
#include <QJsonArray>

Manager::Manager(QObject *parent) : QObject (parent)
{
    Configs *configs = Inits::Instance().getConfigs();
    _sshExecuter = Inits::Instance().getSshExecuter();

    connect(_sshExecuter, &SshExecuter::finished, this, &Manager::onSshFinished);

    const QJsonObject service = configs->getConfigs()["remote-manager"].value("service").toObject();

    _port = static_cast<int>(service.value("ex_port").toDouble());
}

void Manager::taskSwitch(const QString &type, const QJsonObject dataJsonObj)
{
    Parser *parser = Inits::Instance().getParser();

    const QString &method = dataJsonObj.value("method").toString();

    QJsonArray hosts;
    QJsonArray bashes;

    if (dataJsonObj.contains("bash")) {
        bashes = dataJsonObj.value("bash").toArray();
    } else {
        bashes = parser->getBash(method);
    }

    if (dataJsonObj.contains("hall")) {
        const QString hall = QString::number(dataJsonObj.value("hall").toDouble());

        hosts = getHostsList(hall);

        if (hosts.isEmpty()) {
            warnManager << "Can not find hosts in hall:" << hall;

            return;
        }
    }

    if (dataJsonObj.contains("ssh")) {
        if (dataJsonObj.value("ssh").toBool()) {
            sshTask(hosts, bashes);

            return;
        }
    }

    if (dataJsonObj.contains("host_ex")) {
        hosts = dataJsonObj.value("host_ex").toArray();
    }

    if (dataJsonObj.contains("host_ssh")) {
        sshTask(dataJsonObj.value("host_ssh").toArray(), bashes);

        return;
    }

    const QString &dataIn = parser->compare(dataJsonObj, bashes);

    executerTask(hosts, dataIn);
}

QJsonArray Manager::getHostsList(const QString &hallId) {
    DataBase *dataBase = Inits::Instance().getDataBase();

    return dataBase->getHostsList(hallId);
}

void Manager::onSshFinished(const QString &result)
{
    infoManager << "Finished ssh bash:" << result;
}

void Manager::executerTask(const QJsonArray hosts, const QString &dataIn)
{
    const QMap<QString, bool> hostsStatus = _sshExecuter->isOnline(hosts);

    foreach(const QString ip, hostsStatus.keys()) {
        if (!hostsStatus[ip]) {
            infoManager << "Host" << ip << "is offline";

            continue;
        }

        ClientExecuter clientExecuter(QUrl("wss://" + ip + ":"
                                           + QString::number(_port)),  dataIn, this);
    }
}

void Manager::sshTask(const QJsonArray hosts, const QJsonArray bashes)
{
    const QMap<QString, bool> hostsStatus = _sshExecuter->isOnline(hosts);

    foreach(const QString ip, hostsStatus.keys()) {
        if (!hostsStatus[ip]) {
            infoManager << "Host" << ip << "is offline";

            continue;
        }

        _sshExecuter->toSsh(ip, bashes);
    }
}
