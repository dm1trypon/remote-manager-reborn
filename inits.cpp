#include "inits.h"
#include "log.h"

Inits &Inits::Instance()
{
    static Inits theSingleInstance;

    return theSingleInstance;
}

bool Inits::isInited()
{
    infoInitSrv << "[Init configs]";

    if (!isInitConfigs()) {
        errorInitSrv << "Init configs failed";
        return false;
    }

    infoInitSrv << "[Init dependence]";

    if (!isInitDependence()) {
        errorInitSrv << "Init dependence failed";
        return false;
    }

    infoInitSrv << "Success!";
    infoInitSrv << "[Init DB]";

    if (!isInitDB()) {
        errorInitSrv << "Init DB failed";
        return false;
    }

    infoInitSrv << "Success!";
    infoInitSrv << "[Init parser]";

    if (!isInitParser()) {
        errorInitSrv << "Init parser failed";
        return false;
    }

    infoInitSrv << "Success!";
    infoInitSrv << "[Init SSH executer]";

    if (!isInitSshExecuter()) {
        errorInitSrv << "Init SSH executer failed";
        return false;
    }

    infoInitSrv << "Success!";
    infoInitSrv << "[Init manager]";

    if (!isInitManager()) {
        errorInitSrv << "Init manager failed";
        return false;
    }

    infoInitSrv << "Success!";
    infoInitSrv << "[Init server]";

    if (!isInitServer()) {
        errorInitSrv << "Init server failed";
        return false;
    }

    infoInitSrv << "Success!";

    return true;
}

bool Inits::isInitDependence()
{
    _dependence = new Dependence(_configs->getConfigs()["remote-manager"].value("dependence").toArray(), nullptr);

    if (!_dependence) {
        return false;
    }

    if (!_dependence->isExists()) {
        return false;
    }

    return true;
}

bool Inits::isInitConfigs()
{
    _configs = new Configs(nullptr);

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
        errorInitSrv << "Config remote-manager.cfg is not valid!";

        return false;
    }

    return true;
}

bool Inits::isInitDB()
{
    _database = new DataBase(_configs->getConfigs()["remote-manager"].value("mariadb").toObject(), nullptr);

    if (!_database) {
        return false;
    }

    if (!_database->isConnected()) {
        infoInitSrv << "Can not connect to MariaDB!";
        return false;
    }

    return true;
}

bool Inits::isInitParser()
{
    _parser = new Parser(nullptr);

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
    _manager = new Manager(nullptr);

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
    _sshExecuter = new SshExecuter(nullptr);

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

SshExecuter* Inits::getSshExecuter()
{
    return _sshExecuter;
}
