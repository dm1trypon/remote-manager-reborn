#include "parser.h"
#include "configs.h"
#include "inits.h"
#include "log.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QPair>

Parser::Parser(QObject *parent) : QObject(parent)
{

}

bool Parser::isReadyTemplatesJson() {
    return _templatesJson.isReady();
}

QJsonArray Parser::getBash(const QString &method)
{
    Configs *configs = Inits::Instance().getConfigs();

    const QJsonValue valueMethod =
            configs->getConfigs()["remote-manager"].value("commands").toObject().value(method);

    return valueMethod.toArray();
}

QString Parser::compare(const QJsonObject dataJsObj, const QJsonArray bashes)
{
    const QString ip = dataJsObj.value("host_ex").toString();

    QJsonObject jsObj;
    jsObj.insert("method", dataJsObj.value("method").toString());
    jsObj.insert("host_ex", dataJsObj.value("host_ex").toString());
    jsObj.insert("bash", bashes);

    const QJsonDocument jsDoc(jsObj);

    QString data(jsDoc.toJson(QJsonDocument::Compact));

    return bashReplacer(data, dataJsObj.value("type").toString(), ip);
}

QString Parser::bashReplacer(QString data, const QString &type, const QString &ip)
{
    qDebug() << data << type << ip;

    if (data.contains("%kind%")) {
        data.replace("%kind%", type);
    }

    if (data.contains("%host%")) {
        data.replace("%host%", ip);
    }

    return data;
}

QPair<QJsonObject, QString> Parser::fromJson(const QString &data)
{
    const QJsonDocument dataJsonDoc = QJsonDocument::fromJson(data.toUtf8());

    if (!isValidJson(dataJsonDoc)) {
        return {{}, ""};
    }

    const QJsonObject dataJsonObj = dataJsonDoc.object();
    const QString &type = _templatesJson.getNameTemplate(dataJsonObj);

    if (type.isEmpty()) {
        return {{}, ""};
    }

    return QPair<QJsonObject, QString> {dataJsonObj, type};
}

bool Parser::isFinished(const QString &dataOut) {
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(dataOut.toUtf8());

    if (!isValidJson(jsonDoc)) {
        return false;
    }

    if (jsonDoc.object().value("method") != "connection") {
        return false;
    }

    return true;
}

bool Parser::isValidResult(const QString &dataOut, const QString &dataIn)
{
    QList<QJsonDocument> _data;

    _data.append(QJsonDocument::fromJson(dataOut.toUtf8()));
    _data.append(QJsonDocument::fromJson(dataIn.toUtf8()));

    foreach(const QJsonDocument dataDoc, _data) {
        if (!isValidJson(dataDoc)) {
            return false;
        }
    }

    if (_data.first().object().value("method") != _data.last().object().value("method")) {
        return false;
    }

    if (!_data.first().object().contains("result")) {
        return false;
    }

    return true;
}

bool Parser::isValidJson(const QJsonDocument jsonDoc) {
    if (jsonDoc.isNull()) {
        return false;
    }

    if (!jsonDoc.isObject()) {
        return false;
    }

    return true;
}

QString Parser::toResultJson(const QString &result, const QString &method, const QString &ip)
{
    QJsonObject jsObj {
        {"method", method},
        {"result", result},
        {"host", ip},
    };

    const QJsonDocument jsDoc(jsObj);

    return jsDoc.toJson(QJsonDocument::Compact);
}
