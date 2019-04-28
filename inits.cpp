#include "inits.h"

Inits::Inits()
{

}

bool Inits::isInited()
{
    bool success = true;

    success = Configs::Instance().readConfigs("templates", ":/templates.json");
    success = Configs::Instance().readConfigs("remote-manager", ":/remote-manager.cfg");

    if (!success) {
        return false;
    }

    const QJsonObject configRM = Configs::Instance().getConfigs()["remote-manager"];

    if (!configRM.contains("service")) {
        return false;
    }

    if (!configRM.value("service").isObject()) {
        return false;
    }

    const QJsonObject configService = configRM.value("service").toObject();

    if (!configService.contains("port")) {
        return false;
    }

    if (!configService.value("port").isDouble()) {
        return false;
    }

    const quint16 port = static_cast<quint16>(configService.value("port").toDouble());

    _serverRM = new ServerRM(port);

    if (!_serverRM->isStarted()) {
        return false;
    }

    return success;
}
