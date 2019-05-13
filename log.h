#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QDebug>

#define infoDataBase qDebug().noquote() << Log::getDataTime() << "[DataBase][Debug] >>"
#define warnDataBase qWarning().noquote() << Log::getDataTime() << "[DataBase][Warning] >>"
#define errorDataBase qCritical().noquote() << Log::getDataTime() << "[DataBase][Error] >>"

#define infoClientEx qDebug().noquote() << Log::getDataTime() << "[ClientExecuter][Debug] >>"
#define warnClientEx qWarning().noquote() << Log::getDataTime() << "[ClientExecuter][Warning] >>"
#define errorClientEx qCritical().noquote() << Log::getDataTime() << "[ClientExecuter][Error] >>"

#define infoConfigs qDebug().noquote() << Log::getDataTime() << "[Configs][Debug] >>"
#define warnConfigs qWarning().noquote() << Log::getDataTime() << "[Configs][Warning] >>"
#define errorConfigs qCritical().noquote() << Log::getDataTime() << "[Configs][Error] >>"

#define infoInitSrv qDebug().noquote() << Log::getDataTime() << "[Inits][Debug] >>"
#define warnInitSrv qWarning().noquote() << Log::getDataTime() << "[Inits][Warning] >>"
#define errorInitSrv qCritical().noquote() << Log::getDataTime() << "[Inits][Error] >>"

#define infoManager qDebug().noquote() << Log::getDataTime() << "[Manager][Debug] >>"
#define warnManager qWarning().noquote() << Log::getDataTime() << "[Manager][Warning] >>"
#define errorManager qCritical().noquote() << Log::getDataTime() << "[Manager][Error] >>"

#define infoParser qDebug().noquote() << Log::getDataTime() << "[Parser][Debug] >>"
#define warnParser qWarning().noquote() << Log::getDataTime() << "[Parser][Warning] >>"
#define errorParser qCritical().noquote() << Log::getDataTime() << "[Parser][Error] >>"

#define infoServerRM qDebug().noquote() << Log::getDataTime() << "[ServerRM][Debug] >>"
#define warnServerRM qWarning().noquote() << Log::getDataTime() << "[ServerRM][Warning] >>"
#define errorServerRM qCritical().noquote() << Log::getDataTime() << "[ServerRM][Error] >>"

#define infoSshEx qDebug().noquote() << Log::getDataTime() << "[SshExecuter][Debug] >>"
#define warnSshEx qWarning().noquote() << Log::getDataTime() << "[SshExecuter][Warning] >>"
#define errorSshEx qCritical().noquote() << Log::getDataTime() << "[SshExecuter][Error] >>"

#define infoTemplates qDebug().noquote() << Log::getDataTime() << "[Templates][Debug] >>"
#define warnTemplates qWarning().noquote() << Log::getDataTime() << "[Templates][Warning] >>"
#define errorTemplates qCritical().noquote() << Log::getDataTime() << "[Templates][Error] >>"

class Log : public QObject
{
    Q_OBJECT
public:
    Log(QObject *parent);

    static QString getDataTime();
};

#endif // LOG_H
