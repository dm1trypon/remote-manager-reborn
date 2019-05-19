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

    _parser = Inits::Instance().getParser();

    _sshExecuter = Inits::Instance().getSshExecuter();

    connect(_sshExecuter, &SshExecuter::finished, this, &Manager::onSshFinished);

    const QJsonObject service = configs->getConfigs()["remote-manager"].value("service").toObject();

    _port = static_cast<int>(service.value("ex_port").toDouble());
}

void Manager::taskSwitch(const QString &type, const QJsonObject dataJsonObj, const QString &hostSender)
{


    const QString &method = dataJsonObj.value("method").toString();

    QJsonArray hosts;
    QJsonArray bashes;

    if (dataJsonObj.contains("bash")) {
        bashes = dataJsonObj.value("bash").toArray();
    } else {
        bashes = _parser->getBash(method);
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
            sshTask(hosts, bashes, hostSender, dataJsonObj.value("type").toString(), method);

            return;
        }
    }

    if (dataJsonObj.contains("host_ex")) {
        hosts = dataJsonObj.value("host_ex").toArray();
    }

    if (dataJsonObj.contains("host_ssh")) {
        sshTask(dataJsonObj.value("host_ssh").toArray(), bashes, hostSender, dataJsonObj.value("type").toString(), method);

        return;
    }

    const QString &dataIn = _parser->compare(dataJsonObj, bashes);

    executerTask(hosts, dataIn, hostSender);
}

QJsonArray Manager::compareBashes(QJsonArray bashes, const QString &kind, const QString &ip)
{
    const QJsonDocument jsDoc(bashes);

    return QJsonDocument::fromJson(_parser->bashReplacer(jsDoc.toJson(QJsonDocument::Compact), kind,
                                                         ip).toUtf8()).array();
}

QJsonArray Manager::getHostsList(const QString &hallId) {
    DataBase *dataBase = Inits::Instance().getDataBase();

    return dataBase->getHostsList(hallId);
}

void Manager::onSshFinished(const QStringList finishedData)
{
    infoManager << "Finished ssh bash:" << finishedData[0] << "Request host:" << finishedData[1]
                << "Host executer:" << finishedData[2] << "Method:" << finishedData[3];

    emit sendBack(_parser->toResultJson(finishedData[0], finishedData[3], finishedData[2]), finishedData[1]);
}

void Manager::executerTask(const QJsonArray hosts, const QString &dataIn, const QString &hostSender)
{
    const QMap<QString, bool> hostsStatus = _sshExecuter->isOnline(hosts, hostSender);

    foreach(const QString ip, hostsStatus.keys()) {
        if (!hostsStatus[ip]) {
            infoManager << "Host" << ip << "is offline." << "Request host:" << hostSender;

            continue;
        }

        ClientExecuter clientExecuter(QUrl("wss://" + ip + ":"
                                           + QString::number(_port)),  dataIn, this);
    }
}

void Manager::sshTask(const QJsonArray hosts, const QJsonArray bashes, const QString &hostSender, const QString &kind,
                      const QString &method)
{
    QMap<QString, bool> hostsStatus;

    foreach(const QJsonValue host, hosts) {
        hostsStatus.insert(host.toString(), true);
    }

//    const QMap<QString, bool> hostsStatus = _sshExecuter->isOnline(hosts, hostSender);

    foreach(const QString ip, hostsStatus.keys()) {
        if (!hostsStatus[ip]) {
            infoManager << "Host" << ip << "is offline." << "Request host:" << hostSender;

            continue;
        }

        _sshExecuter->toSsh(ip, method, compareBashes(bashes, kind, ip), hostSender);
    }
}
