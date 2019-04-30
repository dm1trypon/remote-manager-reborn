#include "configs.h"

#include <QFile>
#include <QJsonArray>
#include <QDebug>

Configs::Configs()
{

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

bool Configs::isValidMainConfig()
{
    const QJsonObject mainConf = _configs["remote-manager"];
    const QJsonObject checkConf = _configs["config"];

    if (!checkConf.value("main").isArray()) {
        return false;
    }

    const QJsonArray fieldsArr = checkConf.value("main").toArray();

    if (!isValidFields(mainConf, fieldsArr)) {
        return false;
    }

    foreach(const QJsonValue value, mainConf) {
        if (!value.isObject()) {
            return false;
        }
    }

    if (!isValidObject(mainConf, checkConf, "mariadb")) {
        return false;
    }

    if (!isValidObject(mainConf, checkConf, "service")) {
        return false;
    }

    return true;
}

bool Configs::isValidFields(const QJsonObject fieldConf, const QJsonArray fieldsArr)
{
    QStringList fields;
    QStringList fieldConfKeys = fieldConf.keys();

    foreach(const QJsonValue field, fieldsArr) {
        fields.prepend(field.toString());
    }

    fields.sort();
    fieldConfKeys.sort();

    if (fields != fieldConfKeys) {
        return false;
    }

    return true;
}

bool Configs::isValidObject(const QJsonObject mainConf, const QJsonObject checkConf, const QString &name)
{
    if (!mainConf.value(name).isObject()) {
        return false;
    }

    const QJsonObject fieldConf = mainConf.value(name).toObject();

    if (!checkConf.value(name).isArray()) {
        return false;
    }

    const QJsonArray fieldCheck = checkConf.value(name).toArray();

    if (!isValidFields(fieldConf, fieldCheck)) {
        return false;
    }

    if (!checkConf.value("types").isObject()) {
        return false;
    }

    const QJsonObject types = checkConf.value("types").toObject();

    foreach(const QString &key, fieldConf.keys()) {
        if (fieldConf[key].toVariant().typeName() != types.value(key).toString()) {
            return false;
        }
    }

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
