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
    Parser();

    bool isValidResult(const QString &dataOut, const QString &dataIn);
    bool isFinished(const QString &dataOut);
    QJsonArray getBash(const QString &method);
    QString compare(const QJsonObject dataJsObj, const QJsonArray bashes);
    QPair<QJsonObject, QString> fromJson(const QString &data);
    bool isReadyTemplatesJson();
};

#endif // PARSER_H
