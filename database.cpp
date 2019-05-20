#include "database.h"
#include "configs.h"
#include "log.h"

#include <QDateTime>
#include <QMetaObject>

DataBase::DataBase(const QJsonObject configDB, QObject *parent) : QObject(parent)
{
    setDataBase(configDB);
    connect(&_tCheckDbConnection, &QTimer::timeout, this, &DataBase::checkConnection);
}

QString DataBase::getDataTime()
{
    return QDateTime::currentDateTime().toTimeSpec(Qt::OffsetFromUTC).toString("[dd.mm.yy hh:mm:ss]");
}

void DataBase::setDataBase(const QJsonObject configDB)
{
    _db = QSqlDatabase::addDatabase("QMYSQL");

    if (configDB.contains("db_options")) {
        _db.setConnectOptions(configDB.value("db_options").toString());
    }

    _db.setHostName(configDB.value("db_host").toString());
    _db.setUserName(configDB.value("db_user").toString());
    _db.setPassword(configDB.value("db_pass").toString());
    _db.setPort(static_cast<int>(configDB.value("db_port").toDouble()));

    infoDataBase << "Config for MySql connection:"
             << "\nDataBase host:" << configDB.value("db_host").toString()
             << "\nDataBase user:" << configDB.value("db_user").toString()
             << "\nDataBase port:" << configDB.value("db_port").toDouble();

    _db.open();

    _tCheckDbConnection.start(5000);
}

void DataBase::checkConnection()
{
    if (_db.isOpen()) {
        return;
    }

    errorDataBase << "Connection to database is lost, reconnect...";

    _db.open();
}

QString DataBase::queryGetIp(const QString &hallId)
{
    return "SELECT ip FROM db_game.clients WHERE hall_id=" + hallId;
}

QJsonArray DataBase::getHostsList(const QString &hallId)
{
    if (!_db.isOpen()) {
        return {};
    }

    QSqlQuery query(queryGetIp(hallId));
    QJsonArray hosts;

    while (query.next()) {
        if (!query.value(0).isValid()) {
            continue;
        }

        if (query.value(0).isNull()) {
            continue;
        }

        hosts.append(query.value(0).toString());
    }

    return hosts;
}

bool DataBase::isConnected() {
    return _db.isOpen();
//    return true;
}
