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
    Configs();

    bool readConfigs(const QString &name, const QString &path);
    QMap<QString, QJsonObject> getConfigs();

    bool isValidMainConfig();
private:
    QMap <QString, QJsonObject> _configs;

    bool isValidConfig(const QJsonDocument docJsonConfig);

    bool isValidObject(const QJsonObject mainConf, const QJsonObject checkConf, const QString &name);
    bool isValidFields(const QJsonObject fieldConf, const QJsonArray fieldsArr);
};

#endif // CONFIGS_H
