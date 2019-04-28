#ifndef TEMPLATESJSON_H
#define TEMPLATESJSON_H

#include <QObject>
#include <QStringList>
#include <QMap>
#include <QJsonObject>

class TemplatesJson : public QObject
{
    Q_OBJECT
public:
    TemplatesJson();

    QString getNameTemplate(const QJsonObject dataJsObj);

private:
    QMap<QStringList, QString> _templates;
    QMap<QString, QString> _typeVar;

    QJsonObject _config;

    void setTemplates();

    bool isCorrectValues(const QJsonObject dataJsObj);
    bool isValidFields();
    void readConfig();
};

#endif // TEMPLATESJSON_H
