#include "templatesjson.h"
#include "configs.h"
#include "inits.h"

#include <QVariant>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

TemplatesJson::TemplatesJson()
{
    readConfig();
    setTemplates();
}

void TemplatesJson::readConfig()
{
    Configs *configs = Inits::Instance().getConfigs();

    _configTemplates = configs->getConfigs()["templates"];
    _configRM = configs->getConfigs()["remote-manager"];
}

QString TemplatesJson::getNameTemplate(const QJsonObject dataJsObj) {
    if (!_configRM.value("commands").toObject().keys().contains(dataJsObj.value("method").toString())) {
        return "";
    }

    QStringList fields = dataJsObj.keys();
    fields.sort();

    if (!_templates.contains(fields)) {
        qDebug() << "Is not contains!";
        return "";
    }


    if (!isCorrectValues(dataJsObj)) {
        qDebug() << "Is incorrect value!";
        return "";
    }

    return _templates[fields];
}

bool TemplatesJson::isReady()
{
    if (_templates.isEmpty()) {
        return false;
    }

    if (_typeVar.isEmpty()) {
        return false;
    }

    return true;
}

bool TemplatesJson::isCorrectValues(const QJsonObject dataJsObj) {
    foreach(const QString &field, dataJsObj.keys()) {
        if (_typeVar[field] != dataJsObj.value(field).toVariant().typeName()) {
            return false;
        }

        bool isCheckIp = false;

        if (dataJsObj[field].isArray()) {
            if (_configTemplates.value("check_data").toObject().value("ip").toArray().contains(field)) {
                isCheckIp = true;
            }

            foreach(const QJsonValue value, dataJsObj.value(field).toArray()) {
                if (value.toVariant().typeName() != QStringLiteral("QString")) {
                    return false;
                }

                if (!isCheckIp) {
                    continue;
                }

                qDebug() << value.toString();

                if (!isValidIp(value.toString())) {
                    qWarning().noquote() << field << "data is invalid:" << value.toString();

                    return false;
                }
            }
        }
    }

    return true;
}

bool TemplatesJson::isValidFields()
{
    if (_configTemplates.isEmpty()) {
        return false;
    }

    const QStringList fields{"templates", "types", "check_data"};

    foreach(const QString &field , fields) {
        if (!_configTemplates.contains(field)) {
            return false;
        }

        if (!_configTemplates.value(field).isObject()) {
            return false;
        }
    }

    foreach(const QJsonValue value, _configTemplates.value("templates").toObject()) {
        if (!value.isArray()) {
            return false;
        }
    }

    foreach(const QJsonValue value, _configTemplates.value("types").toObject()) {
        if (!value.isString()) {
            return false;
        }
    }

    foreach(const QJsonValue value, _configTemplates.value("check_data").toObject()) {
        if (!value.isArray()) {
            return false;
        }
    }

    return true;
}

bool TemplatesJson::isValidIp(const QString &ip)
{
    const QStringList part = ip.split(".");

    if (part.contains("")) {
        return false;
    }

    if (part.count() != 4) {
        return false;
    }

    const QRegExp rx(IP_MASK);

    return rx.exactMatch(ip);
}

void TemplatesJson::setTemplates()
{
    if (!isValidFields()) {
        return;
    }

    const QJsonObject templatesObj = _configTemplates.value("templates").toObject();

    foreach(const QString &key, templatesObj.keys()) {
        const QJsonArray fieldsArr = templatesObj[key].toArray();

        QStringList fieldsList;

        foreach(const QJsonValue &field, fieldsArr) {
            fieldsList.append(field.toString());
        }

        fieldsList.sort();

        _templates.insert(fieldsList, key);
    }

    const QJsonObject typesObj = _configTemplates.value("types").toObject();

    foreach(const QString &key, typesObj.keys()) {
        _typeVar.insert(key, typesObj[key].toString());
    }
}
