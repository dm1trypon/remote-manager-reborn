#include "database.h"
#include "configs.h"

DataBase::DataBase(const QJsonObject configDB)
{
    setDataBase(configDB);
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

    qDebug().noquote() << "Config for MySql connection:"
             << "\nDataBase host:" << configDB.value("db_host").toString()
             << "\nDataBase user:" << configDB.value("db_user").toString()
             << "\nDataBase port:" << configDB.value("db_port").toDouble();
}

QString DataBase::queryGetIp(const QString &hallId)
{
    return "SELECT ip FROM db_game.clients WHERE hall_id=" + hallId;
}

QStringList DataBase::getHostsList(const QString &hallId)
{
    QSqlQuery query(queryGetIp(hallId));

    QStringList hosts;

    while (query.next()) {
        if (!query.value(0).isValid()) {
            continue;
        }

        if (!query.value(0).isNull()) {
            continue;
        }

        hosts.append(query.value(0).toString());
    }

    return hosts;
}

bool DataBase::isConnected() {
    return _db.open();
}
