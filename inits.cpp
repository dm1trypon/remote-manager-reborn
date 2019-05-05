#include "inits.h"

Inits &Inits::Instance()
{
    static Inits theSingleInstance;

    return theSingleInstance;
}

bool Inits::isInited()
{
    qDebug() << "[Init configs]";

    if (!isInitConfigs()) {
        qWarning() << "Init configs failed";
        return false;
    }

    qDebug() << "Success!";
    qDebug() << "[Init DB]";

    if (!isInitDB()) {
        qWarning() << "Init DB failed";
        return false;
    }

    qDebug() << "Success!";
    qDebug() << "[Init parser]";

    if (!isInitParser()) {
        qWarning() << "Init parser failed";
        return false;
    }

    qDebug() << "Success!";
    qDebug() << "[Init manager]";

    if (!isInitManager()) {
        qWarning() << "Init manager failed";
        return false;
    }

    qDebug() << "Success!";
    qDebug() << "[Init SSH executer]";

    if (!isInitSshExecuter()) {
        qWarning() << "Init SSH executer failed";
        return false;
    }

    qDebug() << "Success!";
    qDebug() << "[Init server]";

    if (!isInitServer()) {
        qWarning() << "Init server failed";
        return false;
    }

    qDebug() << "Success!";

    return true;
}

bool Inits::isInitConfigs()
{
    _configs = new Configs();

    if (!_configs) {
        return false;
    }

    if (!_configs->readConfigs("templates", ":/templates.json")) {
        return false;
    }

    if (!_configs->readConfigs("remote-manager", ":/remote-manager.cfg")) {
        return false;
    }

    if (!_configs->readConfigs("config", ":/config.json")) {
        return false;
    }

    if (!_configs->isValidMainConfig()) {
        qWarning() << "Config remote-manager.cfg is not valid!";

        return false;
    }

    return true;
}

bool Inits::isInitDB()
{
    _database = new DataBase(_configs->getConfigs()["remote-manager"].value("mariadb").toObject());

    if (!_database) {
        return false;
    }

    if (!_database->isConnected()) {
        qDebug() << "Can not connect to MariaDB!";
        return false;
    }

    return true;
}

bool Inits::isInitParser()
{
    _parser = new Parser();

    if (!_parser) {
        return false;
    }

    if (!_parser->isReadyTemplatesJson()) {
        return false;
    }

    return true;
}

bool Inits::isInitManager()
{
    _manager = new Manager();

    if (!_manager) {
        return false;
    }

    return true;
}

bool Inits::isInitServer()
{
    const QJsonObject configService = _configs->getConfigs()["remote-manager"].value("service").toObject();
    const quint16 port = static_cast<quint16>(configService.value("port").toDouble());

    _serverRM = new ServerRM(port, _parser);

    if (!_serverRM) {
        return false;
    }

    if (!_serverRM->isStarted()) {
        return false;
    }

    return true;
}

bool Inits::isInitSshExecuter()
{
    _sshExecuter = new SshExecuter();

    if (!_sshExecuter) {
        return false;
    }

    return true;
}

DataBase* Inits::getDataBase()
{
    return _database;
}

Parser* Inits::getParser()
{
    return _parser;
}

Configs* Inits::getConfigs()
{
    return _configs;
}

Manager* Inits::getManager()
{
    return _manager;
}

SshExecuter *Inits::getSshExecuter()
{
    return _sshExecuter;
}
