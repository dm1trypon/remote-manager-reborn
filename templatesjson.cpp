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
    if (!_configRM.keys().contains(dataJsObj.value("method").toString())) {
        return "";
    }

    QStringList fields = dataJsObj.keys();
    fields.sort();

    qDebug() << fields << _templates.keys();

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

bool TemplatesJson::isCorrectValues(const QJsonObject dataJsObj) {
    foreach(const QString &field, dataJsObj.keys()) {
        qDebug() << _typeVar[field] << dataJsObj.value(field).toVariant().typeName();
        if (_typeVar[field] != dataJsObj.value(field).toVariant().typeName()) {
            return false;
        }

        if (field == "bash") {
            foreach(const QVariant var, dataJsObj.value(field).toArray()) {
                if (var.typeName() != QStringLiteral("QString")) {
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

    const QStringList fields{"templates", "types"};

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

    return true;
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
