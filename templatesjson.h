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

    bool isReady();
private:
    const QString IP_MASK = "(|[0-9]|[0-9]|[0-9]|[0-9][0-9]|[0-9][0-9]|[0-1][0-9][0-9]|2[0-4][0-9]|25[0-5])."
                            "(|[0-9]|[0-9]|[0-9]|[0-9][0-9]|[0-9][0-9]|[0-1][0-9][0-9]|2[0-4][0-9]|25[0-5])."
                            "(|[0-9]|[0-9]|[0-9]|[0-9][0-9]|[0-9][0-9]|[0-1][0-9][0-9]|2[0-4][0-9]|25[0-5])."
                            "(|[0-9]|[0-9]|[0-9]|[0-9][0-9]|[0-9][0-9]|[0-1][0-9][0-9]|2[0-4][0-9]|25[0-5])";

    QMap<QStringList, QString> _templates;
    QMap<QString, QString> _typeVar;

    QJsonObject _configTemplates;
    QJsonObject _configRM;

    void setTemplates();

    bool isCorrectValues(const QJsonObject dataJsObj);
    bool isValidFields();
    void readConfig();
    bool isValidIp(const QString &ip);
};

#endif // TEMPLATESJSON_H
