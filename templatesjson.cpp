#include "templatesjson.h"
#include "configs.h"

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
    const QMap<QString, QJsonObject> configs = Configs::Instance().getConfigs();

    if (!configs.contains("temaplates")) {
        qWarning() << "Can not find config's data for templates!";

        return;
    }

    _config = configs["templates"];
}

QString TemplatesJson::getNameTemplate(const QJsonObject dataJsObj) {
    const QStringList fields = dataJsObj.keys();

    if (!_templates.contains(fields)) {
        return "";
    }


    if (!isCorrectValues(dataJsObj)) {
        return "";
    }

    return _templates[dataJsObj.keys()];
}

bool TemplatesJson::isCorrectValues(const QJsonObject dataJsObj) {
    bool check = true;

    foreach(const QString &field, dataJsObj.keys()) {
        check = _typeVar[field] == dataJsObj.value(field).toVariant().typeName();

        if (!check) {
            return false;
        }
    }

    return true;
}

bool TemplatesJson::isValidFields()
{
    if (_config.isEmpty()) {
        return false;
    }

    const QStringList fields{"templates", "types"};

    foreach(const QString &field , fields) {
        if (!_config.contains(field)) {
            return false;
        }

        if (!_config.value(field).isObject()) {
            return false;
        }
    }

    foreach(const QJsonValue value, _config.value("templates").toObject()) {
        if (!value.isArray()) {
            return false;
        }
    }

    foreach(const QJsonValue value, _config.value("types").toObject()) {
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

    const QJsonObject templatesObj = _config.value("templates").toObject();

    foreach(const QString &key, templatesObj.keys()) {
        const QJsonArray fieldsArr = templatesObj[key].toArray();

        QStringList fieldsList;

        foreach(const QJsonValue &field, fieldsArr) {
            fieldsList.append(field.toString());
        }

        _templates.insert(fieldsList, key);
    }

    const QJsonObject typesObj = _config.value("types").toObject();

    foreach(const QString &key, typesObj.keys()) {
        _typeVar.insert(key, typesObj[key].toString());
    }
}
