#ifndef CONFIGS_H
#define CONFIGS_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QObject>

class Configs : public QObject
{
    Q_OBJECT
public:
    Configs(QObject *parent);

    bool readConfigs(const QString &name, const QString &path);
    QMap<QString, QJsonObject> getConfigs();

    bool isValidMainConfig();
private:
    QMap <QString, QJsonObject> _configs;

    bool isValidConfig(const QJsonDocument docJsonConfig);

    bool isValidObject(const QJsonObject mainConf, const QJsonObject checkConf, const QString &name);
    bool isValidFields(const QJsonObject fieldConf, const QJsonArray fieldsArr);
    bool isCorrectSchemeConfig(const QJsonObject checkConf);
    bool isValidArray(const QJsonArray fieldArr);
};

#endif // CONFIGS_H
