#ifndef PARSER_H
#define PARSER_H

#include "templatesjson.h"

#include <QObject>
#include <QVector>

class Parser : public QObject
{
    Q_OBJECT
private:
    TemplatesJson _templatesJson;

    bool isValidJson(const QJsonDocument jsonDoc);

public:
    Parser(QObject *parent);

    bool isValidResult(const QString &dataOut, const QString &dataIn);
    bool isFinished(const QString &dataOut);
    QJsonArray getBash(const QString &method);
    QString compare(const QJsonObject dataJsObj, const QJsonArray bashes);
    QPair<QJsonObject, QString> fromJson(const QString &data);
    bool isReadyTemplatesJson();
    QString bashReplacer(QString data, const QString &type, const QString &ip);
    QString toResultJson(const QString &result, const QString &method, const QString &ip);
};

#endif // PARSER_H
