#include "manager.h"
#include "clientexecuter.h"
#include "inits.h"
#include "database.h"

#include <QJsonDocument>
#include <QUrl>
#include <QJsonArray>

Manager::Manager()
{
    Configs *configs = Inits::Instance().getConfigs();
    _sshExecuter = Inits::Instance().getSshExecuter();

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
            qWarning() << "Can not find hosts in hall:" << hall;

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

void Manager::executerTask(const QJsonArray hosts, const QString &dataIn)
{
    foreach(const QJsonValue ip, hosts) {
        ClientExecuter clientExecuter(QUrl("wss://" + ip.toString() + ":"
                                           + QString::number(_port)),  dataIn);
    }
}

void Manager::sshTask(const QJsonArray hosts, const QJsonArray bashes)
{
    foreach(const QJsonValue ip, hosts) {
        _sshExecuter->toSsh(ip.toString(), bashes);
    }
}


//"hallEx": ["method", "hall", "ssh"],
//"hallSsh": ["method", "hall", "ssh"],
//"ssh": ["method", "host_ssh"],
//"hostPing": ["method", "host_ping"],
//"sshShell": ["method", "host_ssh", "bash"],
//"hallExShell": ["method", "hall", "bash", "ssh"],
//"hallSshShell": ["method", "hall", "bash", "ssh"],
//"sshKind": ["method", "type", "host_ssh"],
//"hallExKind": ["method", "type", "hall", "ssh"],
//"hallSshKind": ["method", "type", "hall", "ssh"]
