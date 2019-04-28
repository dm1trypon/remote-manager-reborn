#include "configs.h"

#include <QFile>
#include <QJsonArray>
#include <QDebug>

Configs &Configs::Instance()
{
    static Configs theSingleInstance;

    return theSingleInstance;
}


bool Configs::readConfigs(const QString &name, const QString &path)
{
    QFile config(path);
    QJsonDocument docJsonConfig;

    if (!(config.open(QIODevice::ReadOnly | QIODevice::Text))) {
        qWarning().noquote() << "Can not open" << name << "config on path:" << path;

        return false;
    }

    docJsonConfig = QJsonDocument::fromJson(config.readAll());

    config.close();

    if (!isValidConfig(docJsonConfig)) {
        qWarning().noquote() << "Wrong" << name << "config file:" << path;

        return false;
    }

    _configs.insert(name, docJsonConfig.object());

    return true;
}

bool Configs::isValidConfig(const QJsonDocument docJsonConfig)
{
    return !docJsonConfig.isNull() || !docJsonConfig.isEmpty() || docJsonConfig.isObject();
}

QMap<QString, QJsonObject> Configs::getConfigs()
{
    return _configs;
}
