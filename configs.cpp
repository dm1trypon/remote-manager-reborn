#include "configs.h"
#include "log.h"

#include <QFile>
#include <QJsonArray>

Configs::Configs(QObject *parent) : QObject(parent)
{

}

bool Configs::readConfigs(const QString &name, const QString &path)
{
    QFile config(path);
    QJsonDocument docJsonConfig;

    if (!(config.open(QIODevice::ReadOnly | QIODevice::Text))) {
        warnConfigs << "Can not open" << name << "config on path:" << path;

        return false;
    }

    docJsonConfig = QJsonDocument::fromJson(config.readAll());

    config.close();

    if (!isValidConfig(docJsonConfig)) {
        warnConfigs << "Wrong" << name << "config file:" << path;

        return false;
    }

    _configs.insert(name, docJsonConfig.object());

    return true;
}

bool Configs::isCorrectSchemeConfig(const QJsonObject checkConf)
{
    foreach(const QString field, checkConf.keys()) {
        if (field == "types") {
            if (!checkConf[field].isObject()) {
                errorConfigs << field << "is not an object";
                return false;
            }

            foreach(const QJsonValue fieldTypes, checkConf[field].toObject()) {
                if (!fieldTypes.isString()) {
                    errorConfigs << fieldTypes << "is not a string";
                    return false;
                }
            }
        } else {
            if (!checkConf[field].isArray()) {
                errorConfigs << field << "is not an array";
                return false;
            }

            foreach(const QJsonValue fieldTypes, checkConf[field].toArray()) {
                if (!fieldTypes.isString()) {
                    errorConfigs << fieldTypes << "is not a string";
                    return false;
                }
            }
        }
    }

    return true;
}

bool Configs::isValidMainConfig()
{
    const QJsonObject mainConf = _configs["remote-manager"];
    const QJsonObject checkConf = _configs["config"];

    if (!isCorrectSchemeConfig(checkConf)) {
        errorConfigs << "Schema config failed";
        return false;
    }

    const QJsonArray fieldsArr = checkConf.value("main").toArray();

    if (!isValidFields(mainConf, fieldsArr)) {
        errorConfigs << "Is not valid fields:" << mainConf << ". Template:" << fieldsArr;
        return false;
    }

    const QJsonObject types = checkConf.value("types").toObject();

    foreach(const QString field, mainConf.keys()) {
        if (mainConf[field].toVariant().typeName() != types.value(field).toString()) {
            return false;
        }

        if (mainConf[field].isObject()) {
            if (!isValidObject(mainConf, checkConf, field)) {
                errorConfigs << field << "is not valid object";
                return false;
            }
        }

        if (mainConf[field].isArray()) {
            if (!isValidArray(mainConf[field].toArray())) {
               errorConfigs << field << "is not valid array";
                return false;
            }
        }
    }

    return true;
}

bool Configs::isValidArray(const QJsonArray fieldArr)
{
    foreach(const QJsonValue valueConf, fieldArr) {
        if (!valueConf.isString()) {
            return false;
        }
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
    const QJsonArray fieldCheck = checkConf.value(name).toArray();

    QJsonArray fieldConfArr;
    QJsonObject fieldConfObj;

    if (mainConf.value(name).isObject()) {
        fieldConfObj = mainConf.value(name).toObject();

        if (!isValidFields(fieldConfObj, fieldCheck)) {
            return false;
        }
    }

    if (mainConf.value(name).isArray()) {
        fieldConfArr = mainConf.value(name).toArray();
    }

    const QJsonObject types = checkConf.value("types").toObject();

    if (!fieldConfArr.isEmpty()) {
        foreach(const QJsonValue val, fieldConfArr) {
            if (!val.isString()) {
                return false;
            }
        }

        return true;
    }

    foreach(const QString &key, fieldConfObj.keys()) {
        if (fieldConfObj[key].toVariant().typeName() != types.value(key).toString()) {
            return false;
        }

        if (fieldConfObj[key].isArray()) {
            if (!isValidArray(fieldConfObj[key].toArray())) {
                return false;
            }
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
