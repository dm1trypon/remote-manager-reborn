#ifndef CONFIGS_H
#define CONFIGS_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>

class Configs
{
public:
    Configs(){}
    static Configs& Instance();

    bool readConfigs(const QString &name, const QString &path);
    QMap<QString, QJsonObject> getConfigs();

private:
    Configs(const Configs& root) = delete;
    Configs& operator = (const Configs&) = delete;

    QMap <QString, QJsonObject> _configs;

    bool isValidConfig(const QJsonDocument docJsonConfig);

};

#endif // CONFIGS_H
