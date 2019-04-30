#include "manager.h"
#include "clientexecuter.h"
#include "inits.h"
#include "database.h"

#include <QJsonDocument>
#include <QUrl>
#include <QJsonArray>

Manager::Manager()
{

}

void Manager::taskSwitch(const QString &type, const QJsonObject dataJsonObj)
{
    Parser *parser = Inits::Instance().getParser();
    Configs *configs = Inits::Instance().getConfigs();

    const QJsonObject service = configs->getConfigs()["remote-manager"].value("service").toObject();
    const int port = static_cast<int>(service.value("ex_port").toDouble());
    const QString &method = dataJsonObj.value("method").toString();

    if (type == "ex") {
        const QJsonArray bashes = parser->getBash(method);
        const QString &dataIn = parser->compare(dataJsonObj, bashes);

        ClientExecuter clientExecuter(QUrl("wss://" + dataJsonObj.value("host_ex").toString() + ":"
                                           + QString::number(port)),  dataIn);

        return;
    }

    if (type == "exKind") {
        const QJsonArray bashes = parser->getBash(method);
        const QString &dataIn = parser->compare(dataJsonObj, bashes);

        ClientExecuter clientExecuter(QUrl("wss://" + dataJsonObj.value("host_ex").toString() + ":"
                                           + QString::number(port)),  dataIn);

        return;
    }

    if (type == "exShell") {
        const QJsonArray bashes = dataJsonObj.value("bash").toArray();
        const QString &dataIn = parser->compare(dataJsonObj, bashes);

        ClientExecuter clientExecuter(QUrl("wss://" + dataJsonObj.value("host_ex").toString() + ":"
                                           + QString::number(port)),  dataIn);

        return;
    }

    if (type == "hallEx") {
        const QStringList hosts = getHostsList(QString::number(dataJsonObj.value("hall").toDouble()));
        const QJsonArray bashes = parser->getBash(method);
        const QString &dataIn = parser->compare(dataJsonObj, bashes);

        if (hosts.isEmpty()) {
            return;
        }

        foreach(const QString &host, hosts) {
            ClientExecuter clientExecuter(QUrl("wss://" + host + ":"
                                               + QString::number(port)),  dataIn);
        }
    }
}

QStringList Manager::getHostsList(const QString &hallId) {
    DataBase *dataBase = Inits::Instance().getDataBase();

    return dataBase->getHostsList(hallId);
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
